#include "yflow_random.h"

unsigned long _seed_(int _seed)          
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
        return nowtime;
    }
    else
    {
        return (unsigned long) 1314 +_seed;
    }   
}

unsigned long int _MiddleSquare_(int seed)
{
    int num = seed * seed;
    int number[10], inc = 0;
    //将整数的每一位保存在数组中
    while(num != 0)
    {
        number[inc] = num%10;
        num /= 10;
        inc++;
    }
    int min = inc/2;
    int max = inc / 2 + min;
    int temp[10];

    //Assign the middle square part to another table
    for(int i = min; i <= max; i++)
    {
        temp[i-min] = number[i];
    }

    for(int i=0; i < max-min; i++)
    {
        //Check if there is a "0" "0" sequence, if TRUE - replace with current time (seconds)
        if(temp[i] == 0 && temp[i+1] == 0)
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
            temp[i] = local.tm_sec/10;
            temp[i + 1] = local.tm_sec%10;
        }
    }

    //Transform the squared array into an integer
    unsigned long int k = 0;
    for (int i = 0; i <= max-min; i++)
        k = 10 * k + temp[i];


    return k;
}
