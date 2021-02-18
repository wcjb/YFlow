/**
 * @file tools.h
 * @brief 
 * @author 殉道者 (wcjbyjx@gmail.com)
 * @version 0.0.1
 * @date 2021-01-25
 * @copyright Copyright © 2014-2021 weichijunbo.
 * @see [math.h实现](https://zhuanlan.zhihu.com/p/80458536)
 * *********************************************************************************
 * @par ChangeLog:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2021-01-25 <td>1.0     <td>殉道者     <td>内容
 * </table>
 * *********************************************************************************
 */
#ifndef __TOOLS__
#define __TOOLS__

#pragma once

/**
 * @brief 判断当前系统类型
 * @return int @c 0为windows,1为macos
 */
int sys(void);

/**
 * @brief 基于牛顿迭代的快速平方根倒数算法(x(n+1) = x(n)-f(x(n))/f'(x(n)))
 * @param x 需要进行平方倒数运算的数
 * @details x = 0.5*x(3-2*alpha*x*)
 * @return 计算结果
*/
float InvSqrt (float x);

/**
 * @brief 快速幂实现
 * @details  其本质思想是将取幂的任务按照指数的二进制表示 来分割成更小的任务，即将指数不停的二分。
 * @section 算法描述
 * -# 若指数b为偶数，则f = a^b = a^(b/2)*a^(b/2)
 * -# 若指数b为奇数，则f = a^b = a^[(b-1)/2]*a^[(b-1)/2]*b
 * -# 经过上述转换将a^b的计算问题转为a^(b/2)或a^[(b-1)/2]的计算
 * 问题，继续上述迭代过程，直到指数为1为止；如此将计算的时间复杂度
 * 从O(n)降为log(n);
 * @param  a                幂运算的底数
 * @param  b                幂运算的指数
 * @see 快速幂 https://oi-wiki.org/math/quick-pow/
 * @return long long @c 
 */

/**
 * @brief 判断当前计算设备是大端存储还是小端存储
 * @return int @c  若为大端存储返回1，小端存储返回0
 */ 
int IsBigEndian();

long long binpow(long long a, long long b);

/**
 * @brief 基于泰勒级数和Remez算法的ln函数计算实现
 * @param  x  要取自然对数的值
 * @note                  
 *   1. 参数约化（Argument Reduction）: 找到k和f使得
 *			x = 2^k * (1+f), 
 *	   其中  sqrt(2)/2 < 1+f < sqrt(2) .
 *	   则有 log(x) = k*log2+log(1+f) 
 *   2.  计算log(1+f)的近似值.
 *	  作变换s = f/(2+f) ; 则有log(1+f) = log(1+s) - log(1-s)
 *		 = 2s + 2/3 s**3 + 2/5 s**5 + ...
 *	     = 2s + s*R
 *      接下来在区间[0,0.1716]使用Remez算法来计算多项式R的最佳一致逼近
 * 多项式，理论上，度数为14的最佳一致逼近多项式的精度为2**-58.45，完全能
 * 满足64位浮点数的运算要求。
 *		得到最佳一致多项式如下：
 *	    R(z) ~ Lg1*s +Lg2*s +Lg3*s +Lg4*s +Lg5*s  +Lg6*s  +Lg7*s
 *  	( Lg1 到 Lg7 的值已在程序种给出)
 *	此时，
 *	   
 *	    | Lg1*s +...+Lg7*s    -  R(z) | <= 2**-58.45
 *
 *	然后根据2s = f - s*f = f - hfsq + s*hfsq, 其中 hfsq = f*f/2.
 *  得到
 *		log(1+f) = f - s*(f - R)	
 * 如果对精度有更高要求，则继续迭代2s=f-sf,得到：
 *		log(1+f) = f - (hfsq - s*(hfsq+R)).	
 *	
 *	3. 最终,  log(x) = k*ln2 + log(1+f).  
 *			    = k*ln2_hi+(f-(hfsq-(s*(hfsq+R)+k*ln2_lo)))
 *     需要注意的是，此处将ln2分成了两部分进行计算：
 *                   ln2 = ln2_hi + ln2_lo
 *	   当|n| < 2000时n*ln2_hi是精确的。
 *
 * @return float @c
 * @see 参考知乎回答的第一条答案：https://www.zhihu.com/question/333371020/answer/1686069171
 * 关于Remez算法可参看项目路径/docs下的Function approximation and the remez algorithm和数值计
 * 算方法等文档。 算法实现可参考C语言math库源码http://www.netlib.org/fdlibm/。
 */

double __ieee754_log(double x);  //ln的remes

/**
 * @brief 基于自适应辛普森的ln函数实现
 * @param  x                desc
 * @note ln(x)可表示为函数1/t在[1,x]上的积分变上限函数，再使用自适应辛普森公式计算该积分
 * 即可得到函数值
 * @return double @c 
 */
double __simpson_log(double x);

/**
 * @brief 实现任意底数n的对数函数
 * 由于计算机无法存储浮点数的精确值（浮点数的存储方法可以参考 IEEE 754，这里不再赘述），
 * 而指数函数和对数函数的参数和返回值均为浮点数，因此运算过程中会存在误差。
 * @note log_{n}(x) = ln(x)/ln(n)
 * @param  x                要求对数的对象
 * @param  n                对数的底数
 * @return double @c 
 */
double log(double x,int n);

/**
 * @brief 平方根函数实现
 * @param  x                desc
 * @return float @c 
 */
float sqrt(float x);

/**
 * @brief 变量值交换
 * @param  x                desc
 * @param  y                desc
 */
void swap(int *x, int *y);

/**
 * @brief 选择排序的C++实现,其原理为首先找出序列中的最小（最大）元素，
 * 并置于序列的起始位置，再从剩下的元素中找到最小（最大元素）置于已排
 * 序序列的末尾，以此类推，直到确定最后一个元素的位置，则此时所有元素
 * 排序完成。算法的时间复杂度为O(n^2)
 * @param  list             desc
 * @param  n                desc
 */
void selection_sort(int list [], int n);

/**
 * @brief 冒泡排序算法的C++实现，其原理为重复访问序列中的元素，比较
 * 两个相邻元素的大小，如果顺序错误（此处的顺序错误指不符合理想顺序），
 * 则将它们的位置进调换，一直重复，直到没有元素需要进行位置交换为止。
 * 冒泡排序名称的由来是因为较小的元素会慢慢‘浮’到序列的顶端。算法的
 * 时间复杂度为O(n^2)
 * @param  list             desc
 * @param  n                desc
 */
void bubble_sort(int list[], int n);

#endif 