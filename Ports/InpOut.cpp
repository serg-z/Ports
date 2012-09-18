#include "InpOut.h"

#ifdef __linux__
#include <sys/io.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/mman.h>
#endif

#include <iostream>

#ifdef __linux__
const char *InpOut::s_shmName = "/inpout_ports_shared_memory";
#endif

unsigned short InpOut::Read(unsigned short address) {
    return getInstance().ReadFromPort(address);
}

void InpOut::Write(unsigned short address, unsigned short data) {
    getInstance().WriteToPort(address, data);
}

bool InpOut::Initialized()
{
    return getInstance().m_initialized;
}

bool InpOut::Simulated()
{
    return getInstance().m_simulated;
}

InpOut::InpOut() :
    m_initialized(false),
    m_simulated(false)
{
#ifdef WIN32
    m_initialized = true;

    if (!(hLib = LoadLibraryA("inpout32.dll"))) {
        std::cerr << "LoadLibrary failed." << std::endl;

        m_initialized = false;
    } else {
        if (!(inp32fp = (inpfuncPtr) GetProcAddress(hLib, "Inp32"))) {
            std::cerr << "GetProcAddress for Inp32 Failed." << std::endl;

            m_initialized = false;
        }

        if (!(out32fp = (outfuncPtr) GetProcAddress(hLib, "Out32"))) {
            std::cerr << "GetProcAddress for Oup32 Failed." << std::endl;
            m_initialized = false;
        }
    }
#elif __linux__
    if (!(m_initialized = !ioperm(0x378, 3, 1)))
        std::cerr << "Can't initialize. Try running as root." << std::endl;
#endif

    // check is port works
    if (m_initialized) {
        // reset, write and check is value changed
        WriteToPort(0x378, 0);
        unsigned short v1 = ReadFromPort(0x378);
        WriteToPort(0x378, 1);
        m_initialized = v1 != ReadFromPort(0x378);
    }

    // run in simulation mode (using shared memory object)
    if (!m_initialized) {
#ifdef __linux__
        // TODO: currently linux only
        std::cout << "Opening shared memory" << std::endl
                  << "Name: " << s_shmName << ", size: " << m_shmSize << std::endl;

        m_shmSize = 3 * sizeof(unsigned short);

        int shmFd = shm_open(s_shmName, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

        if (shmFd == -1) {
            perror("shm_open");
        } else {
            if (ftruncate(shmFd, m_shmSize)) {
                perror("ftruncate");
            } else {
                m_shmAddr = (unsigned short*)mmap(0, m_shmSize, PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);

                if (m_shmAddr == MAP_FAILED) {
                    perror("mmap");
                } else {
                    std::cout << "Shared memory initialization success" << std::endl;

                    if (!close(shmFd))
                        perror("close");

                    // LPT1
                    m_shmData.insert(std::make_pair(0x378, ShmData(m_shmAddr, 0x1, true))); // data
                    m_shmData.insert(std::make_pair(0x379, ShmData(m_shmAddr + 1, 0x78))); // state
                    m_shmData.insert(std::make_pair(0x37a, ShmData(m_shmAddr + 2, 0xc, true))); // control

                    m_simulated = true;
                }
            }

            if (!m_simulated) {
                if (!close(shmFd))
                    perror("close");
            }
        }
#else
#warning "Simulation mode not implemented on this platform"
#endif
    }
}

InpOut::~InpOut() {
    if (m_initialized) {
#ifdef WIN32
        (void) FreeLibrary(hLib);
#endif

        m_initialized = false;
    }

    if (m_simulated) {
#ifdef __linux__
        m_shmData.clear();

        if (munmap(m_shmAddr, m_shmSize))
            perror("munmap");

#ifdef UNLINK_SHARED_MEMORY
        std::cout << "Unlinking shared memory" << std::endl;

        if (shm_unlink(s_shmName))
            perror("shm_unlink");
#endif // UNLINK_SHARED_MEMORY

#endif // __linux__

        m_simulated = false;
    }
}

InpOut& InpOut::getInstance() {
    static InpOut instance;
    return instance;
}

unsigned short InpOut::ReadFromPort(unsigned short address) {
    if (m_initialized) {
#ifdef WIN32
        return (inp32fp)(address);
#elif __linux__
        return inb(address);
#endif
    } else if (m_simulated) {
        auto cit = m_shmData.find(address);

        if (cit == m_shmData.cend()) {
            std::cerr << "Failed to read shared memory at address 0x" << std::hex << address
                      << std::endl;
        } else {
            return cit->second;
        }
    }

    return 0;
}

void InpOut::WriteToPort(unsigned short address, unsigned short data) {
    if (m_initialized) {
#ifdef WIN32
        (out32fp)(address, data);
#elif __linux__
        outb(data, address);
#endif
    } else if (m_simulated) {
        auto cit = m_shmData.find(address);

        if (cit == m_shmData.cend()) {
            std::cerr << "Failed to write shared memory at address 0x" << std::hex << address
                      << std::endl;
        } else {
            cit->second = data;
#ifdef __linux__
            if (msync(m_shmAddr, m_shmSize, MS_SYNC))
                std::cerr << "msync failed" << std::endl;
#endif
        }
    }
}
