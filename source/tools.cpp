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

double binpow(double a, long long b) 
{   
    double res = 1.0;
    while (b > 0) 
    {
        if (b & 1) res = res * a; /**<将二进制位为 1 时对应的幂累乘到结果中*/
        a = a * a;
        b >>= 1;
    }
  return res;
}

int IsBigEndian()
{
	short int a = 0x1122;//十六进制，一个数值占4位
	char b =  *(char *)&a;  //通过将short(2字节)强制类型转换成char单字节，b指向a的起始字节（低字节）
	if( b == 0x11)//低字节存的是数据的高字节数据
	{
		return 1; //大端
	}
	else
	{
		return 0; //小端
	}
}

void swap(int *x, int *y)
{   
    //声明变量temp为一个整型变量，并且它的内容是x所指向的地址的内容
    int temp = *x;
    //将y指向的地址中的内容存储到x所指向的地址
    *x = *y;
    //将temp中的内容存储到y所指向的地址
    *y = temp;
}

void selection_sort(int list [], int n)
{
    int min;
    for(int i = 0; i < n-1; i++)
    {
        min = i;
        for(int j = i+1; j < n; j++)
        {
            if(list[j] < list[min])
            {
                min = j;
            }
        }
        swap(&list[i],&list[min]);
    }
}

void bubble_sort(int list[], int n)
{
    for (int i = 0;i < n;i++)
    {
        for (int j = 0;j < n-(i+1);j++)
        {
            if (list[j] > list[j+1])
            {
                swap(&list[j],&list[j+1]);
            }
        }
    }
}
