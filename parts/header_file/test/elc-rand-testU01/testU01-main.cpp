//test my rand from dll with TestU01

#include <cstdio>
#include <cstdint>
#include <cstddef>

extern "C" {
    #include "TestU01.h"
}

typedef uint32_t (*rand_func_ptr)(void);

#include <windows.h>//for LoadLibrary
#include <vector>//for argv
#include <string>//for argv

int main(int argc, char**_argv)
{
    std::vector<std::string> argv(_argv, _argv + argc);
    if (argc < 2) {
        printf("Usage: %s <dll_path> test_type[small_crush|crush|big_crush]", argv[0].c_str());
        return 1;
    }
    HMODULE h = LoadLibrary(argv[1].c_str());
    if (!h) {
        printf("LoadLibrary failed");
        return 1;
    }
    rand_func_ptr rand_func = (rand_func_ptr)GetProcAddress(h, "rand_for_test");
    if (!rand_func) {
        printf("GetProcAddress failed");
        return 1;
    }
    unif01_Gen* gen = unif01_CreateExternGenBits("rand_for_test", rand_func);
    if (!gen) {
        printf("unif01_CreateExternGenBits failed");
        return 1;
    }
    if(argc < 3) {
        bbattery_Crush(gen);
    } else {
        if(argv[2] == "small_crush") {
            bbattery_SmallCrush(gen);
        } else if(argv[2] == "crush") {
            bbattery_Crush(gen);
        } else if(argv[2] == "big_crush") {
            bbattery_BigCrush(gen);
        } else {
            printf("Unknown test type: %s", argv[2].c_str());
            unif01_DeleteExternGenBits(gen);
            return 1;
        }
    }
    unif01_DeleteExternGenBits(gen);
    FreeLibrary(h);
    return 0;
}
