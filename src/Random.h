/**
 * @file Random.h
 * @brief 随机数生成器实现
 * @author 殉道者 (wcjbyjx@gmail.com)
 * @version 0.0.1
 * @date 2021-01-26
 * @copyright Copyright © 2014-2021 weichijunbo.
 * @details     随机数生成器的实现，实现了绝大部分的随机数生成算法包括冯·诺伊曼平方取中法、
 * 线性同余发生器、FSR发生器、组合发生器、梅森旋转演算法。由于需要生成符合常见分布的随机数，
 * 故采用随机数生成算法生成符合均匀分布(即一个周期内所有数据出现是等概率)的随机数，再利用均匀分布的随机数生成符合其它分布的随
 * 机数。
 * 
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
/**
 * @brief 根据系统时间获取随机数种子，作为随机数生成算法中的初值
 * @param  _seed            初始化随机数种子，0表示使用时间种子，其余整数则直接使用314159+_seed作为随机数种子
 */
unsigned long seed_time(int _seed);

/**
 * @brief 计算整数的位数
 * @details     要计算整数的位数，直观来看，可以不断除10来累加计数，
 * 弊端在于需要不断进行除法，因此采用临界值比较来累加计数，提升效率
 * @param  number           desc
 * @return int @c 
 */
int len_number(unsigned long number);

int split_number(unsigned long number,int m,int n);

/**
 * @brief 冯·诺依曼平方取中法用于生成符合均匀分布的随机序列
 * @details     基本思想为：将数列中的第a(i)项（假设其有 m 位）平方，
 * 取得到的2m位数（若不足 2m 位，在最高位前补 0）中间部分的m位数字
 * 10^{[m/2]+1})至10^{[m/2]+m}的数作为a(i)的下一项 a(i+1)，由此产
 * 生一个伪随机数数列。
 * @section 算法描述
 * -# 选择一个m位数N(i)作为种子；
 * -# 计算N(i)的平方
 * @section C++数据类型
 * 
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
float middle_square(void);

/**
 * @brief 
 * @tparam T 
 * @param  data             desc
 * @param  count            desc
 * @return T @c 
 */
// template <class T>
// void boxmuller(T* data,size_t count)
// {
//     assert(count % 2 == 0);
//     static const T twopi = T(2.0*3.14159265358979323846);
//     /// 均匀分布生成器
//     LCG<T> r;
//     for (size_t i = 0; i < count;i+=2)
//     {   ///把 [0, 1) 的随机数转换成 (0, 1]，避免了 log(0) 的出现，不需要做rejection(拒绝采样)
//         T u1 = 1.0f-r();
//         T u2 = r();
//         T radius = std::sqrt(-2* std::log(u1));
//         T theta = twopi *u2;
//         data[i] = radius * std::cos(theta);
//         data[i+1] = radius * std::sin(theta);
//     }
// }
#endif