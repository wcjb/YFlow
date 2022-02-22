#include "tools.h"

int sys(void)
{
    #ifdef _WIN64
   //define something for Windows (64-bit)
        return 0;
    #elif _WIN32
    //define something for Windows (32-bit)
    #elif __APPLE__
        #include "TargetConditionals.h"
        #if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
            // define something for simulator   
        #elif TARGET_OS_IPHONE
            // define something for iphone  
        #else
            #define TARGET_OS_OSX 1
            // define something for OSX
            return 1;
        #endif
    #elif __linux
        // linux
    #elif __unix // all unices not caught above
        // Unix
    #elif __posix
        // POSIX
    #endif
}
float InvSqrt (float x)
{
    float xhalf = 0.5f*x;
    int i = *(int*)&x;
    i = 0x5f3759df - (i>>1);
    x = *(float*)&i;
    x = x*(1.5f - xhalf*x*x);
    return x; 
}

long long binpow(long long a, long long b) 
{   
    long long res = 1;
    while (b > 0) 
    {
        if (b & 1) res = res * a;
        a = a * a;
        b >>= 1;
    }
  return res;
}

int IsBigEndian()
{
	short int a = 0x1122;//十六进制，一个数值占4位
	char b =  *(char *)&a;  //通过将short(2字节)强制类型转换成char单字节，b指向a的起始字节（低字节）
	if( b == 0x11)//低字节存的是数据的高字节数据
	{
		return 1; //大端
	}
	else
	{
		return 0; //小端
	}
}

double __ieee754_log(double x)
{
	static const double	
	ln2_hi  =  6.93147180369123816490e-01,	/* 3fe62e42 fee00000 */
	ln2_lo  =  1.90821492927058770002e-10,	/* 3dea39ef 35793c76 */
	two54   =  1.80143985094819840000e+16,  /* 43500000 00000000 */
	Lg1 = 6.666666666666735130e-01,  /* 3FE55555 55555593 */
	Lg2 = 3.999999999940941908e-01,  /* 3FD99999 9997FA04 */
	Lg3 = 2.857142874366239149e-01,  /* 3FD24924 94229359 */
	Lg4 = 2.222219843214978396e-01,  /* 3FCC71C5 1D8E78AF */
	Lg5 = 1.818357216161805012e-01,  /* 3FC74664 96CB03DE */
	Lg6 = 1.531383769920937332e-01,  /* 3FC39A09 D078C69F */
	Lg7 = 1.479819860511658591e-01;  /* 3FC2F112 DF3E5244 */

	static double zero   =  0.0;

	if (IsBigEndian())
	{
		#define __BIG_ENDIAN__ 1;
	}
	else
	{
		#define __LITTLE_ENDIAN 0;	
	}

	#ifdef __BIGENDBIAN__
	#define __HI(x) *(int*)&x
	#define __LO(x) *(1+(int*)&x)
	#define __HIp(x) *(int*)x
	#define __LOp(x) *(1+(int*)x)
	#else
	#define __HI(x) *(1+(int*)&x)
	#define __LO(x) *(int*)&x
	#define __HIp(x) *(1+(int*)x)
	#define __LOp(x) *(int*)x
	#endif

	double hfsq,f,s,z,R,w,t1,t2,dk;
	int k,hx,i,j;
	unsigned lx;

	hx = __HI(x);		/* high word of x */
	lx = __LO(x);		/* low  word of x */

	k=0;
	if (hx < 0x00100000) 
	{			/* x < 2**-1022  */
	    if (((hx&0x7fffffff)|lx)==0) 
		return -two54/zero;		/* log(+-0)=-inf */
	    if (hx<0) return (x-x)/zero;	/* log(-#) = NaN */
	    k -= 54; x *= two54; /* subnormal number, scale up x */
	    hx = __HI(x);		/* high word of x */
	} 
	if (hx >= 0x7ff00000) return x+x;
	k += (hx>>20)-1023;
	hx &= 0x000fffff;
	i = (hx+0x95f64)&0x100000;
	__HI(x) = hx|(i^0x3ff00000);	/* normalize x or x/2 */
	k += (i>>20);
	f = x-1.0;
	if((0x000fffff&(2+hx))<3) {	/* |f| < 2**-20 */
	    if(f==zero) if(k==0) return zero;  else {dk=(double)k;
				 return dk*ln2_hi+dk*ln2_lo;}
	    R = f*f*(0.5-0.33333333333333333*f);
	    if(k==0) return f-R; else {dk=(double)k;
	    	     return dk*ln2_hi-((R-dk*ln2_lo)-f);}
	}
 	s = f/(2.0+f); 
	dk = (double)k;
	z = s*s;
	i = hx-0x6147a;
	w = z*z;
	j = 0x6b851-hx;
	t1= w*(Lg2+w*(Lg4+w*Lg6)); 
	t2= z*(Lg1+w*(Lg3+w*(Lg5+w*Lg7))); 
	i |= j;
	R = t2+t1;
	if(i>0) 
	{
	    hfsq=0.5*f*f;
	    if(k==0) return f-(hfsq-s*(hfsq+R)); else
		     return dk*ln2_hi-((hfsq-(s*(hfsq+R)+dk*ln2_lo))-f);
	} else 
	{
	    if(k==0) return f-s*(f-R); else
		     return dk*ln2_hi-((s*(f-R)-dk*ln2_lo)-f);
	}
}

double __simpson_log(double x)
{
	return 0.0;
}

void swap(int *x, int *y)
{   
    //声明变量temp为一个整型变量，并且它的内容是x所指向的地址的内容
    int temp = *x;
    //将y指向的地址中的内容存储到x所指向的地址
    *x = *y;
    //将temp中的内容存储到y所指向的地址
    *y = temp;
}

void selection_sort(int list [], int n)
{
    int min;
    for(int i = 0; i < n-1; i++)
    {
        min = i;
        for(int j = i+1; j < n; j++)
        {
            if(list[j] < list[min])
            {
                min = j;
            }
        }
        swap(&list[i],&list[min]);
    }
}

void bubble_sort(int list[], int n)
{
    for (int i = 0;i < n;i++)
    {
        for (int j = 0;j < n-(i+1);j++)
        {
            if (list[j] > list[j+1])
            {
                swap(&list[j],&list[j+1]);
            }
        }
    }
}
