#include "loader.h"
#include "functions.h"
#include <stdio.h>

#ifndef WIN32
#include <dlfcn.h>
#else
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
    
    void printString(const char * const s) {
        printf("String from library: %s\n", s);
    }
    
    void loadRun(const char * const s) {
        void * lib;
        void (*fun)(void);
#ifndef WIN32
        lib = dlopen(s, RTLD_LAZY);
#else
        lib = LoadLibrary(s);
#endif
        if (!lib) {
            printf("cannot open library '%s'\n", s);
            return;
        }
#ifndef WIN32
        fun = (void (*)(void))dlsym(lib, "run");
#else
        fun = (void (*)(void))GetProcAddress((HINSTANCE)lib, "run");
#endif
        if (fun == NULL) {
            printf("cannot load function run\n");
        } else {
            fun();
        }
#ifndef WIN32
        dlclose(lib);
#else
        FreeLibrary((HINSTANCE)lib);
#endif
    }
    
#ifdef __cplusplus
}
#endif
