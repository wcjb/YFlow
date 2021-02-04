#include "tools.h"

int sys(void)
{
    #ifdef _WIN64
   //define something for Windows (64-bit)
        return 0;
    #elif _WIN32
    //define something for Windows (32-bit)
    #elif __APPLE__
        #include "TargetConditionals.h"
        #if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
            // define something for simulator   
        #elif TARGET_OS_IPHONE
            // define something for iphone  
        #else
            #define TARGET_OS_OSX 1
            // define something for OSX
            return 1;
        #endif
    #elif __linux
        // linux
    #elif __unix // all unices not caught above
        // Unix
    #elif __posix
        // POSIX
    #endif
}
float InvSqrt (float x)
{
    float xhalf = 0.5f*x;
    int i = *(int*)&x;
    i = 0x5f3759df - (i>>1);
    x = *(float*)&i;
    x = x*(1.5f - xhalf*x*x);
    return x; 
}

long long binpow(long long a, long long b) 
{   
    long long res = 1;
    while (b > 0) 
    {
        if (b & 1) res = res * a;
        a = a * a;
        b >>= 1;
    }
  return res;
}