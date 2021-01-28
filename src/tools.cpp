#include "tools.h"

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