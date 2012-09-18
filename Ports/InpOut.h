#ifndef __INPOUT_H__
#define __INPOUT_H__

#ifdef WIN32
#include <windows.h>
#elif __linux__
#else
#error "Other systems not implemented"
#endif

#include <map>

class InpOut {
public:
    static unsigned short Read(unsigned short address);
    static void Write(unsigned short address, unsigned short data);
    static bool Initialized();
    static bool Simulated();

private:

#ifdef WIN32
    typedef short (_stdcall *inpfuncPtr)(short address);
    typedef void (_stdcall *outfuncPtr)(short address, short data);

    inpfuncPtr inp32fp;
    outfuncPtr out32fp;

    HINSTANCE hLib;
#elif  __linux__
    static const char *s_shmName;
    int m_shmSize;
    unsigned short *m_shmAddr;
#endif

    struct ShmData {
        ShmData(unsigned short *addr, unsigned short value, bool writable = false) :
            m_addr(addr),
            m_writable(writable)
        {
            *m_addr = value;
        }

        operator unsigned short() { return *m_addr; }

        unsigned short operator=(unsigned short a) {
            if (m_writable)
                *m_addr = a;

            return a;
        }

        unsigned short *m_addr;
        bool m_writable;
    };

    InpOut();
    virtual ~InpOut();

    InpOut& operator=(const InpOut&);

    static InpOut& getInstance();

    unsigned short ReadFromPort(unsigned short address);
    void WriteToPort(unsigned short address, unsigned short data);

    bool m_initialized;
    bool m_simulated;
    std::map<unsigned short, ShmData> m_shmData; // address, data
};

#endif // __INPOUT_H__
