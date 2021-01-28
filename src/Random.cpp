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
        localtime_s(&local,&_time);
        local.tm_mon++;
        unsigned long ssecond = (long)local.tm_sec*100+local.tm_sec+36923;
        unsigned long nowtime = local.tm_sec + local.tm_min*100 + local.tm_hour*10000 + local.tm_mday*1000000+local.tm_mon*100000000; 
        return nowtime;
    }
    else
    {
        return (unsigned long) 314159 +_seed;
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
    if(len < n)
    {
        printf("ERROR:split or number is error!\n");
    }
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
    int end_len = len_number(square);
       
    unsigned int random_number[MAX_NUMBER];
    for (int i = 0;i < MAX_NUMBER;i++)
    {  
        random_number[i] = split_number(square,start_len/2+1,start_len/2+start_len);
        std::cout << square <<std::endl << random_number[i] << std::endl;
        square *=  square;
    }
    
    return 0.0f;

}
