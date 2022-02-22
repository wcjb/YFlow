#pragma once
#include <iostream>
using namespace std;

class complex;

complex& __doapl(complex& ths,const complex& r);
complex& __doami(complex& ths,const complex& r);
complex& __doaml(complex& ths,const complex& r);
// 模版类
//template<typename T>
/*
complex<double> c(2,1);
*/
class complex
{
public:
    // 初始列，效率较高
    complex (double r = 0, double i = 0) : re (r), im (i)  {}
    /* 函数可以定义在类内部也可以定义在类外部，定义在类内部的
    函数可能成为inline（内联)函数。和宏定义类似
    */
    complex& operator += (const complex&);
    complex& operator -= (const complex&);
    complex& operator *= (const complex&);
    complex& operator /= (const complex&);

    // 内联函数，但最后不一定成为内联函数，需编译器确定
    //类内部使用const关键词的函数是指不会改变数据内容的函数
    double real () const {return re;}
    double imag () const {return im;}
private:
    //定义复数的实部和虚部
    double re, im;
    friend complex& __doapl (complex *,const complex&);
    friend complex& __doami (complex *, const complex&);
    friend complex& __doaml (complex *, const complex&);
};
   
inline complex& __doapl (complex* ths, const complex& r)
{
  ths->re += r.re;
  ths->im += r.im;
  return *ths;
}

inline complex& complex::operator += (const complex& r)
{
    return __doapl (this,r);
}

inline complex&  __doami (complex* ths, const complex& r)
{
  ths->re -= r.re;
  ths->im -= r.im;
  return *ths;
}
 
inline complex& complex::operator -= (const complex& r)
{
  return __doami (this, r);
}

inline complex& __doaml (complex* ths, const complex& r)
{
  double f = ths->re * r.re - ths->im * r.im;
  ths->im = ths->re * r.im + ths->im * r.re;
  ths->re = f;
  return *ths;
}
inline complex& complex::operator *= (const complex& r)
{
  return __doaml (this, r);
}
// global 函数
inline double imag (const complex& x)
{
  return x.imag ();
}

inline double real (const complex& x)
{
  return x.real ();
}

inline complex operator + (const complex& x, const complex& y)
{
  return complex (real (x) + real (y), imag (x) + imag (y));
}

inline complex operator + (const complex& x, double y)
{
  return complex (real (x) + y, imag (x));
}

inline complex operator + (double x, const complex& y)
{
  return complex (x + real (y), imag (y));
}

inline complex operator - (const complex& x, const complex& y)
{
  return complex (real (x) - real (y), imag (x) - imag (y));
}

inline complex operator - (const complex& x, double y)
{
  return complex (real (x) - y, imag (x));
}

inline complex operator - (double x, const complex& y)
{
  return complex (x - real (y), - imag (y));
}

inline complex operator * (const complex& x, const complex& y)
{
  return complex (real (x) * real (y) - imag (x) * imag (y),
			   real (x) * imag (y) + imag (x) * real (y));
}

inline complex operator * (const complex& x, double y)
{
  return complex (real (x) * y, imag (x) * y);
}

inline complex operator * (double x, const complex& y)
{
  return complex (x * real (y), x * imag (y));
}
// 复数与实数的除法
complex operator / (const complex& x, double y)
{
  return complex (real (x) / y, imag (x) / y);
}
// 复数与复数之前的除法
complex operator / (const complex& x,complex& y)
{
  return x*y/(real (y) * real (y)+imag (y) * imag (y));
}
inline complex operator + (const complex& x)
{
  return x;
}

inline complex operator - (const complex& x)
{
  return complex (-real (x), -imag (x));
}

inline bool operator == (const complex& x, const complex& y)
{
  return real (x) == real (y) && imag (x) == imag (y);
}

inline bool operator == (const complex& x, double y)
{
  return real (x) == y && imag (x) == 0;
}

inline bool operator == (double x, const complex& y)
{
  return x == real (y) && imag (y) == 0;
}

inline bool operator != (const complex& x, const complex& y)
{
  return real (x) != real (y) || imag (x) != imag (y);
}

inline bool operator != (const complex& x, double y)
{
  return real (x) != y || imag (x) != 0;
}

inline bool operator != (double x, const complex& y)
{
  return x != real (y) || imag (y) != 0;
}

// 将复数转为极坐标模式
// inline complex polar (double r, double t)
// {
//   return complex (r * cos (t), r * sin (t));
// }
// 求共轭复数
inline complex conj (const complex& x) 
{
  return complex (real (x), -imag (x));
}
//  求复数的标准化形式
inline double norm (const complex& x)
{
  return real (x) * real (x) + imag (x) * imag (x);
}

ostream& operator << (ostream& os, const complex& x)
{
  if (imag (x) >= 0)
  {
    return os << real (x) << '+' << imag (x) << 'i';
  }
  else
  {
    return os << real (x) << '-' << -imag (x) << 'i';
  }
}
