/**
 * @file tools.h
 * @brief 
 * @author 殉道者 (wcjbyjx@gmail.com)
 * @version 0.0.1
 * @date 2021-01-25
 * @copyright Copyright © 2014-2021 weichijunbo.
 * 
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
long long binpow(long long a, long long b);
/**
 * @brief 参考1993年由Sun Microsystems写入C标准库的方案的log函数算法，函数为 double ieee754log(double x)（IEEE754: IEEE二进制浮点数算术标准）
 * @param  x  要取自然对数的值
 * @return float @c
 * @see 参考知乎回答的第一条答案：https://www.zhihu.com/question/333371020/answer/1686069171
 */
// float __ieee754_log(float x)
// {   
//     static const double
//     ln2_hi  =  6.93147180369123816490e-01,	/* 3fe62e42 fee00000 */
//     ln2_lo  =  1.90821492927058770002e-10,	/* 3dea39ef 35793c76 */
//     two54   =  1.80143985094819840000e+16,  /* 43500000 00000000 */
//     Lg1 = 6.666666666666735130e-01,  /* 3FE55555 55555593 */
//     Lg2 = 3.999999999940941908e-01,  /* 3FD99999 9997FA04 */
//     Lg3 = 2.857142874366239149e-01,  /* 3FD24924 94229359 */
//     Lg4 = 2.222219843214978396e-01,  /* 3FCC71C5 1D8E78AF */
//     Lg5 = 1.818357216161805012e-01,  /* 3FC74664 96CB03DE */
//     Lg6 = 1.531383769920937332e-01,  /* 3FC39A09 D078C69F */
//     Lg7 = 1.479819860511658591e-01;  /* 3FC2F112 DF3E5244 */

//     static double zero   =  0.0;
    
    
//     double hfsq,f,s,z,R,w,t1,t2,dk;
//     int k,hx,i,j;
//     unsigned lx;
//     hx = x & 0xffff0000) >> 16;
//     lx = x& 0x0000ffff;
//     hx = __HI(x);		/* high word of x */
//     lx = __LO(x);		/* low  word of x */

//     k=0;
//     if (hx < 0x00100000) {			/* x < 2**-1022  */
//         if (((hx&0x7fffffff)|lx)==0) 
//         return -two54/zero;		/* log(+-0)=-inf */
//         if (hx<0) return (x-x)/zero;	/* log(-#) = NaN */
//         k -= 54; x *= two54; /* subnormal number, scale up x */
//         hx = __HI(x);		/* high word of x */
//     } 
//     if (hx >= 0x7ff00000) return x+x;
//     k += (hx>>20)-1023;
//     hx &= 0x000fffff;
//     i = (hx+0x95f64)&0x100000;
//     __HI(x) = hx|(i^0x3ff00000);	/* normalize x or x/2 */
//     k += (i>>20);
//     f = x-1.0;
//         //  到这，第一步标记，得出 k, f, s 的值了
//     if((0x000fffff&(2+hx))<3) {	/* |f| < 2**-20 */
//         if(f==zero) if(k==0) return zero;  else {dk=(double)k;
//                 return dk*ln2_hi+dk*ln2_lo;}
//         R = f*f*(0.5-0.33333333333333333*f);
//         if(k==0) return f-R; else {dk=(double)k;
//                 return dk*ln2_hi-((R-dk*ln2_lo)-f);}
//     }
//     s = f/(2.0+f); 
//     dk = (double)k;
//     z = s*s;
//     i = hx-0x6147a;
//     w = z*z;
//     j = 0x6b851-hx;
//     t1= w*(Lg2+w*(Lg4+w*Lg6)); 
//     t2= z*(Lg1+w*(Lg3+w*(Lg5+w*Lg7))); 
//     i |= j;
//     R = t2+t1;
//     if(i>0) {
//         hfsq=0.5*f*f;
//         if(k==0) return f-(hfsq-s*(hfsq+R)); else
//             return dk*ln2_hi-((hfsq-(s*(hfsq+R)+dk*ln2_lo))-f);
//     } else {
//         if(k==0) return f-s*(f-R); else
//             return dk*ln2_hi-((s*(f-R)-dk*ln2_lo)-f);
//     }
// }


#endif