#include "Random.h"

unsigned long seed_time(int _seed)          
{
    if (_seed == 0)
    {
        time_t _time;
        time(&_time);   ///函数返回从1970年1月1日到当前时间系统所经历的秒数
        struct tm local;
        /**
         * @code
         * ///将time所指的时间转换为当地标准时间
         * errno_t localtime_s(
         *                      struct tm* _tm,           ///指向要填充的时间结构的指针
         *                      const time_t *time        ///指向存储的时间
         *  );
         * @endcode
         */
        /// Defined in header <time.h>
        /// struct tm *localtime  ( const time_t *timer );
        /// (1)	
        /// struct tm *localtime_r( const time_t *timer, struct tm *buf );
        /// (2)	(since C23)
        /// struct tm *localtime_s( const time_t *restrict timer, struct tm *restrict buf );
        /// (3)	(since C11)
        /// 1) Converts given time since epoch (a time_t value pointed to by timer) into calendar time, expressed in local time, in the struct tm format. The result is stored in static storage and a pointer to that static storage is returned.
        /// 2) Same as (1), except that the function uses user-provided storage buf for the result.
        /// 3) Same as (1), except that the function uses user-provided storage buf for the result and that the following errors are detected at runtime and call the currently installed constraint handler function:
        /// timer or buf is a null pointer
        /// As with all bounds-checked functions, localtime_s is only guaranteed to be available if __STDC_LIB_EXT1__ is defined by the implementation and if the user defines __STDC_WANT_LIB_EXT1__ to the integer constant 1 before including time.h.
        /// 通过预编译宏来判断当前系统：https://www.jianshu.com/p/c92e8b81ad04
        #ifdef _WIN64
            localtime_s(&local,&_time);
        #elif __APPLE__
            #include "TargetConditionals.h"
            #if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
                // define something for simulator   
            #elif TARGET_OS_IPHONE
                // define something for iphone  
            #else
                #define TARGET_OS_OSX 1
                // define something for OSX
                localtime_r(&_time, &local);
            #endif
        #endif
        
        local.tm_mon++;
        unsigned long ssecond = (long)local.tm_sec*100+local.tm_sec+36923;
        unsigned long nowtime = local.tm_sec + local.tm_min*100 + local.tm_hour*10000 + local.tm_mday*1000000+local.tm_mon*100000000; 
        /// 由系统时间生成的随机数种子为9位整数
        /// 平方后已超过了long类型数字上限，故取其后四位作为随机数种子
        return ((double)nowtime / 10000 - nowtime / 10000) * 10000;
    }
    else
    {
        return (unsigned long) 1314 +_seed;
    }   
}

int len_number(unsigned long number)
{   
    /// 只计算整数的数字位数
    const unsigned long SUP_VALUE[] = { 9, 99, 999, 9999, 99999, 999999, 9999999,99999999, 999999999,2147483647};
    for (int i=0; ; i++)
    {
        if (number <= SUP_VALUE[i])
        {
            return i+1;
        }
    }
}

int split_number(unsigned long number,int m,int n)
{
    int len = len_number(number);
    // if(len < n)
    // {
    //     printf("ERROR:split of number is error!\n");
    // }
    number = number / binpow(10,m-1);
    int pow = binpow(10,n+1-m);
    return ((double)number / pow-number / pow)*pow;
}

float middle_square(void)
{
    /// 生成随机数量
    const int MAX_NUMBER = 9;
    /// 随机数种子作为迭代的初值
    unsigned long random = seed_time(0);
    int start_len = len_number(random);
    unsigned long square = random * random;
       
    unsigned int random_number[MAX_NUMBER];
    for (int i = 0;i < MAX_NUMBER;i++)
    {  
        std::cout << start_len/2+1 << std::endl << start_len/2+start_len << std::endl; 
        random_number[i] = split_number(square,start_len/2+1,start_len/2+start_len);
        std::cout << square <<std::endl << random_number[i] << std::endl;
        square = random_number[i]*random_number[i] ;
        start_len = len_number(square);
    }
    return 0.0f;
}
