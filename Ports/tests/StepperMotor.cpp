#include "InpOutUtils.h"

void halfstep(int cycles, int delay) {
    unsigned short v;
    
    for (int i = 0; i < cycles; ++i) {
        v = 1;
        for (int k = 0; k < 8; ++k) {                
            InpOut::Write(0x378, v);
            Utils::Wait(delay);
            
            if (k == 6) {
                v = 9;
            } else if (k > 0) {
                v = k % 2 ? v | v << 1 : v & v << 1;
            }
        }
    }
}

void wave(int cycles, int delay) {
    unsigned short v;
    
    for (int i = 0; i < cycles; ++i) {
        v = 1;
        for (int k = 0; k < 4; ++k) {
            InpOut::Write(0x378, v);
            Utils::Wait(delay);
            v <<= 1;
        }
    }
}

void fullstep(int cycles, int delay) {
    unsigned short v;
    
    for (int i = 0; i < cycles; ++i) {
        v = 3;
        for (int k = 0; k < 4; ++k) {                
            InpOut::Write(0x378, v);
            Utils::Wait(delay);
            v = k == 2 ? 9 : v << 1;
        }
    }
}

int main(int argc, char **argv) {
    Utils::CheckArgsCount(argc, 2, "mode (0 = half step, 1 = wave, 2 = fullstep) loops [delay(ms)=200]");
    Utils::CheckInit();

    int mode = std::abs(Utils::FromString(argv[1]));
    int loops = std::abs(Utils::FromString(argv[2]));
    int delay = 200;
    if (argc > 3)
        delay = std::abs(Utils::FromString(argv[3]));
    
    switch (mode) {
        case 0: halfstep(loops, delay); break;
        case 1: wave(loops, delay); break;
        case 2: fullstep(loops, delay); break;
    }
    
    InpOut::Write(0x378, 0);

    return 0;
}
