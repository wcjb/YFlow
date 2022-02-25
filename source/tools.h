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
#pragma once

/**
 * @brief 判断当前系统类型
 * @return int @c 0为windows,1为macos
 */
int sys(void);

/**
 * @brief 绝对值函数实现
 * @tparam T 
 * @param  x                desc
 * @return T @c 
 */
template<class T> 
inline static T abs(T x)
{
    return x>0 ? x:(-x);
} 
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
double binpow(double a, long long b);


/**
 * @brief 判断当前计算设备是大端存储还是小端存储
 * @return int @c  若为大端存储返回1，小端存储返回0
 */ 
int IsBigEndian();


/**
 * @brief 平方根函数实现
 * @param  x                desc
 * @return float @c 
 */
float _sqrt(float x);

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
