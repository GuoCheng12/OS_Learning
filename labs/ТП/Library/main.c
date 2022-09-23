#include "loader.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    int main(int argc, char *argv[]) {
        if (argc > 1) {
            loadRun(argv[1]);
        }
        return 0;
    }
    
#ifdef __cplusplus
}
#endif

