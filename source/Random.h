/**
 * @file Random.h
 * @brief 随机数生成器实现
 * @author 殉道者 (wcjbyjx@gmail.com)
 * @version 0.0.1
 * @date 2021-01-26
 * @copyright Copyright © 2014-2021 weichijunbo.
 * @details     随机数生成器的实现，实现了绝大部分的随机数生成算法包括冯·诺伊曼平方取中法及其改良算法、
 * 线性同余发生器、梅森旋转演算法，FSR发生器、组合发生器（因为效率原因，未作实现）。由于需要生成符合常见分布的随机数，
 * 故采用随机数生成算法生成符合均匀分布(即一个周期内所有数据出现是等概率)的随机数，再利用均匀分布的随机数生成符合其它分布的随
 * 机数。
 * @see [随机数的前世今生](https://www.nkdacs.com/share/Random%20number/Random%20number/#6)
 * @see [漫谈正态分布的生成](https://cosx.org/2015/06/generating-normal-distr-variates)
 * *********************************************************************************
 * @par ChangeLog:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2021-01-26 <td>0.0.1    <td>殉道者     <td>内容
 * </table>
 * *********************************************************************************
 */
#ifndef __RANDOM__
#define __RANDOM__

#pragma once

#include <time.h>
#include "tools.h"
#include <iostream> 
#include <stdint.h>

const double __TWO32__ = 4294967296.0; ///2^32
const unsigned long int __TWO31__ = 2147483647; ///2^31

/**
 * @brief 根据系统时间获取随机数种子，作为随机数生成算法中的初值
 * @param  _seed            初始化随机数种子，0表示使用时间种子，其余整数则直接使用314159+_seed作为随机数种子
 */
unsigned long _seed_(int _seed);

/**
 * @brief 冯·诺依曼平方取中法用于生成符合均匀分布的随机序列,不建议使用
 * @details     基本思想为：将数列中的第a(i)项（假设其有 m 位）平方，
 * 取得到的2m位数（若不足 2m 位，在最高位前补 0）中间部分的m位数字
 * 10^{[m/2]+1})至10^{[m/2]+m}的数作为a(i)的下一项 a(i+1)，由此产
 * 生一个伪随机数数列。
 * @section 算法描述
 * -# 选择一个m位数N(i)作为种子；
 * -# 计算N(i)的平方
 * @section C++数据类型
 *  数据类                   字节大小	        数值范围
 *  short int                2 字节	     -32768 〜+32767 (-2^15~2^15-1)
 *  unsigned short int       2 字节	     0 〜+65535 (0~2^16-1)
 *  int                      4 字节	     -2147483648 〜+2147483647 (-2^32-1~2^32-1)
 *  unsigned int 	         4 字节	     0 〜4294967295 (0~2^32-1)
 *  long int                 4 字节	     -2147 483 648 〜+2 147 483 647  (-2^31~2^31-1)
 *  unsigned long int	     4 字节 	 0 〜4294967295 (0~2^32-1)
 *  long long int 	         8 字节	     -9223372036854775808~9223372036854775807 (-2^63~2^63-1)
 *  unsigned long long int   8 字节	     18446744073709551615 (0~2^64 - 1)
 * @return float* @c 
 */
unsigned long int _MiddleSquare_(int seed);

/**
 * @brief 基于Wely等分布定理的平方取中法实现
 * @see [Wely序列](https://www.wikiwand.com/en/Weyl_sequence)
 * @attention &emsp;&emsp;需要说明的是，在计算机中，Wely序列的整数形式通常用于生成离散的
 * 均匀分布，而不是连续的分布。因为无法使用计算机表示无理数，所以使用有理数来进行近似，而所
 * 有有理数都可以表示为分数即两个整数的比率。因此选择整数k，它相对于整数模数m为质数。在m为2
 * 的幂的常见情况下，这等于要求k为奇数。此时，序列0,k,2k,3k,...是模m的均匀分布。即每个项的
 * 余数的序列在被m除时将均匀地分布在间隔[0，m）中
 * @return T @c 
 */
template<class T>
inline static T _WelySquare_() 
{
    uint64_t x = 0, w = 0, s = 0xb5ad4eceda1ce2a9;
    x *= x; 
    x += (w += s); 
    return x = (x>>32) | (x<<32);
}

/**************************************************************************\
*                                                                          *
*  Four round counter-based middle square                                  *
*                                                                          *
*  squares(ctr,key) - returns a 32-bit unsigned int [0,0xffffffff]         *
*                                                                          *
*  The parameters are patterned after Philox parameters. The first is      *
*  the counter.  This is an arbitrary 64-bit unsigned integer.  The        *
*  second is the key.  The key should be an entry from the keys.h file.    *
*  The keys.h file contains keys that have been created with different     *
*  hexadecimal digits. This assures sufficient change in the ctr*key       *
*  computation.                                                            *
*                                                                          *
*  This generator would be used in a similar way as Philox.  One would     *
*  increment a counter and then call the RNG to generate a random number.  *
*                                                                          *
*  Four rounds of squaring are performed and the result is returned.       *
*  For the first three rounds, the result is rotated right 32 bits.        *
*  This places the random data in the best position for the next round.    *
*  y = ctr*key or z = (ctr+1)*key is added on each round.  For keys        *
*  generated by the key utility, either ctr*key or (ctr+1)*key will        *
*  have non-zero digits.  This improves randomization and also provides    *
*  for a uniform output.                                                   *
*                                                                          *
*  Note:  The squares RNG was based on ideas derived from Middle Square    *
*  Weyl Sequence RNG.  One of the ideas was to obtain uniformity by adding *
*  the Weyl sequence after squaring.  Richard P. Brent (creator of the     *
*  xorgens RNG) suggested this method.  It turns out that adding ctr*key   *
*  is equivalent.  Brent's idea provides the basis for uniformity in this  *
*  generator.                                                              *                                            *
\**************************************************************************/
/**
 * @brief  基于Wely序列的四轮平方取中随机数生成器的实现，是满周期的（即生成的随机数在周期内不重复） 
 * @param  ctr              生成的随机数的序号，保证每次生成的随机数是不同的
 * @param  key              有keys.h存储的密钥，每个密钥可生成2^32个不重复的随机数，
 * @see 2020-11-23 [Squares: A Fast Counter-Based RNG] (https://arxiv.org/abs/2004.06278v3)
 * @attention &emsp;&emsp;2020-05-19 论文[https://arxiv.org/abs/1704.00358v5](https://arxiv.org/abs/1704.00358v5) 
 * 提出了一种基于Wely序列的平方取中改良算法，克服了平方取中的"Zero mechanism"等问题。
 * @return uint32_t @c 
 */
template<class T>
inline static T _FourRoundWelySquares_(uint64_t ctr) 
{
    uint64_t key[] = {
        #include "keys.h"
    };
    uint64_t x, y, z;
    y = x = ctr * key[0]; z = y + key[0];
   
    x = x*x + y; x = (x>>32) | (x<<32);       /* round 1 */

    x = x*x + z; x = (x>>32) | (x<<32);       /* round 2 */

    x = x*x + y; x = (x>>32) | (x<<32);       /* round 3 */

    return ((x*x + z) >> 32) / __TWO32__ ;                   /* round 4 */
}

/**
 * @brief   Kobayashi混合同余发生器的实现，该生成器为满周期 2^31 的混合同余发生器，其周期较长，
 * 统计性质比较好，迭代式如下：
 * x(n) = (314159269*x(n-1)+453806245) mod 2^31
 * @see https://www.nkdacs.com/share/Random%20number/Random%20number/#6
 * @attention &emsp;&emsp;同余发生器的原理是不断进行同余运算来迭代生成满足均匀分布的随机数，
 * 其生成的随机数统计性质较好，但是由于其生成机制的缺陷，导致随机数相邻项之前存在强依赖，本实
 * 现只用于学习使用，不做实际使用。其它类型的线性同余生成器如乘同余和素数模乘同余只是加强了生
 * 成算法的约束，在此不做实现。
 * @tparam T 
 * @param  seed           随机数种子
 * @return T @c 
 */
template<class T>
inline static T _LCG_(unsigned long seed)
{
    unsigned long x_n = (314159269 * seed +453806245) % __TWO31__;
    return (T) x_n  / __TWO31__;
}
//MT19937 64位实现
//http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/VERSIONS/C-LANG/mt19937-64.c
void MT19937();
/* 
   A C-program for MT19937-64 (2004/9/29 version).
   Coded by Takuji Nishimura and Makoto Matsumoto.

   This is a 64-bit version of Mersenne Twister pseudorandom number
   generator.

   Before using, initialize the state by using init_genrand64(seed)  
   or init_by_array64(init_key, key_length).

   Copyright (C) 2004, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   References:
   T. Nishimura, ``Tables of 64-bit Mersenne Twisters''
     ACM Transactions on Modeling and 
     Computer Simulation 10. (2000) 348--357.
   M. Matsumoto and T. Nishimura,
     ``Mersenne Twister: a 623-dimensionally equidistributed
       uniform pseudorandom number generator''
     ACM Transactions on Modeling and 
     Computer Simulation 8. (Jan. 1998) 3--30.

   Any feedback is very welcome.
   http://www.math.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove spaces)
*/

//#include <stdio.h>
// #define NN 312
// #define MM 156
// #define MATRIX_A 0xB5026F5AA96619E9ULL
// #define UM 0xFFFFFFFF80000000ULL /* Most significant 33 bits */
// #define LM 0x7FFFFFFFULL /* Least significant 31 bits */


// /* The array for the state vector */
// static unsigned long long mt[NN]; 
// /* mti==NN+1 means mt[NN] is not initialized */
// static int mti=NN+1; 

// /* initializes mt[NN] with a seed */
// void init_genrand64(unsigned long long seed)
// {
//     mt[0] = seed;
//     for (mti=1; mti<NN; mti++) 
//         mt[mti] =  (6364136223846793005ULL * (mt[mti-1] ^ (mt[mti-1] >> 62)) + mti);
// }

// /* initialize by an array with array-length */
// /* init_key is the array for initializing keys */
// /* key_length is its length */
// void init_by_array64(unsigned long long init_key[],
// 		     unsigned long long key_length)
// {
//     unsigned long long i, j, k;
//     init_genrand64(19650218ULL);
//     i=1; j=0;
//     k = (NN>key_length ? NN : key_length);
//     for (; k; k--) {
//         mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 62)) * 3935559000370003845ULL))
//           + init_key[j] + j; /* non linear */
//         i++; j++;
//         if (i>=NN) { mt[0] = mt[NN-1]; i=1; }
//         if (j>=key_length) j=0;
//     }
//     for (k=NN-1; k; k--) {
//         mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 62)) * 2862933555777941757ULL))
//           - i; /* non linear */
//         i++;
//         if (i>=NN) { mt[0] = mt[NN-1]; i=1; }
//     }

//     mt[0] = 1ULL << 63; /* MSB is 1; assuring non-zero initial array */ 
// }
// /* generates a random number on [0, 2^64-1]-interval */
// unsigned long long genrand64_int64(void)
// {
//     int i;
//     unsigned long long x;
//     static unsigned long long mag01[2]={0ULL, MATRIX_A};

//     if (mti >= NN) { /* generate NN words at one time */

//         /* if init_genrand64() has not been called, */
//         /* a default initial seed is used     */
//         if (mti == NN+1) 
//             init_genrand64(5489ULL); 

//         for (i=0;i<NN-MM;i++) {
//             x = (mt[i]&UM)|(mt[i+1]&LM);
//             mt[i] = mt[i+MM] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
//         }
//         for (;i<NN-1;i++) {
//             x = (mt[i]&UM)|(mt[i+1]&LM);
//             mt[i] = mt[i+(MM-NN)] ^ (x>>1) ^ mag01[(int)(x&1ULL)];
//         }
//         x = (mt[NN-1]&UM)|(mt[0]&LM);
//         mt[NN-1] = mt[MM-1] ^ (x>>1) ^ mag01[(int)(x&1ULL)];

//         mti = 0;
//     }
  
//     x = mt[mti++];

//     x ^= (x >> 29) & 0x5555555555555555ULL;
//     x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
//     x ^= (x << 37) & 0xFFF7EEE000000000ULL;
//     x ^= (x >> 43);

//     return x;
// }

// /* generates a random number on [0, 2^63-1]-interval */
// long long genrand64_int63(void)
// {
//     return (long long)(genrand64_int64() >> 1);
// }

// /* generates a random number on [0,1]-real-interval */
// double genrand64_real1(void)
// {
//     return (genrand64_int64() >> 11) * (1.0/9007199254740991.0);
// }

// /* generates a random number on [0,1)-real-interval */
// double genrand64_real2(void)
// {
//     return (genrand64_int64() >> 11) * (1.0/9007199254740992.0);
// }

// /* generates a random number on (0,1)-real-interval */
// double genrand64_real3(void)
// {
//     return ((genrand64_int64() >> 12) + 0.5) * (1.0/4503599627370496.0);
// }


// int main(void)
// {
//     int i;
//     unsigned long long init[4]={0x12345ULL, 0x23456ULL, 0x34567ULL, 0x45678ULL}, length=4;
//     init_by_array64(init, length);
//     printf("1000 outputs of genrand64_int64()\n");
//     for (i=0; i<1000; i++) {
//       printf("%20llu ", genrand64_int64());
//       if (i%5==4) printf("\n");
//     }
//     printf("\n1000 outputs of genrand64_real2()\n");
//     for (i=0; i<1000; i++) {
//       printf("%10.8f ", genrand64_real2());
//       if (i%5==4) printf("\n");
//     }
//     return 0;
// }




#endif