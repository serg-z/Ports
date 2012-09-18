#include "InpOutUtils.h"

void forwardBack(int loops) {
    bool back = false;
    short s;

    for (int i = 0; i < loops; i += !back) {
        s = back ? 0 : 7;
        
        do {
            InpOut::Write(0x378, 0x80 >> s);
            s += back ? 1 : -1;
            Utils::Wait(200);
        } while (s > 0 && s < 8);
        
        back = !back;
    }
}

void strobe(int loops) {
    unsigned short v = 0x55;
    for (int i = 0; i < loops; ++i) {
        InpOut::Write(0x378, v = ~v);
        Utils::Wait(200);
    }
}

void fill(int loops) {
    bool back = false;
    short s;

    for (int i = 0; i < loops; i += !back) {
        s = back ? 0 : 8;
        
        do {
            InpOut::Write(0x378, 0xff >> (s += back ? 1 : -1));
            Utils::Wait(200);
        } while (s > 0 && s < 8);
        
        back = !back;
    }
}

int main(int argc, char **argv) {
    Utils::CheckArgsCount(argc, 2, "mode (0 = forward-back, 1 = strobe, 2 = fill) loops");
    Utils::CheckInit();

    int mode = std::abs(Utils::FromString(argv[1]));
    int loops = std::abs(Utils::FromString(argv[2]));
    
    switch (mode) {
        case 0: forwardBack(loops); break;
        case 1: strobe(loops); break;
        case 2: fill(loops); break;
    }
    
    InpOut::Write(0x378, 0);

    return 0;
}
