#include "mainCPP.h"

#ifdef __APPLE__

#ifdef __cplusplus
extern "C"
#endif 

int SDL_main(int argc, char **argv)
{
    mainCPP(argc, argv);
    return 0;
}

#else

int main()
{
    return mainCPP(NULL, NULL);
}

#endif