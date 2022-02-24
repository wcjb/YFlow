#pragma once
#include "tools.h"

/**
 * @brief 多项式求值，逐项求和算法，即按多项式的次数由低到高进行求值。
 * @param x 值
 * @param coefficient double类型指针，存储多项式的系数，按次数由低到高进行存储。
 * @param max_exponential 多项式的次数，即多项式的最高次数
 * @note float精度是2^23，能保证6位。double精度是2^52，能保证15位。但是默认float和
 * double都只能显示6位，再多需要#include <iomanip>，然后在输出语句之前插入cout << 
 * setprecision(20);强制输出小数位。
 * @return 返回多项式在x处的值
 */
double Polynomial(double x,double* coefficient,int max_exponential)
{
    double power = 1.0;
    double poly = coefficient[0];
    if (x == 0 || max_exponential==0) return coefficient[0];
    for(int i = 1;i < max_exponential;i++)
    {
        power *= x;
        poly += coefficient[i] * power;

    }
    return poly;
}
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