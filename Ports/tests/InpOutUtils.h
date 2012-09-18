#ifndef __INPOUTUTILS_H__
#define __INPOUTUTILS_H__

#include "InpOut.h"

#include <iostream>
#include <sstream>
#include <cstdlib>

namespace Utils {
    void CheckInit() {
        if (InpOut::Initialized()) {
            return;
        } else {
            std::cerr << "InpOut initialization failed: Run as root" << std::endl;
        }
        
        if (InpOut::Simulated()) {
            return;
        } else {
            std::cerr << "InpOut simulation failed" << std::endl;
            exit(-1);
        }
    }
    
    void CheckArgsCount(int argc, int required, const std::string& note) {
        if (argc - 1 < required) {
            std::cerr << "Required arguments count: " << required << std::endl
                << "Note: " << note << std::endl;
            exit(-1);
        }
    }
    
    void Wait(unsigned short millisec) {
#ifdef __linux__
        usleep(millisec * 1000);
#else
#error "Not implemented"
#endif
    }

    template<class T = int> T FromString(const std::string& s) {
        std::istringstream iss(s);
        T res;
        iss >> res;
        return res;
    }
}

#endif // __INPOUTUTILS_H__
