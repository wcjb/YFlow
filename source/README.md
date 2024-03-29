+ Wely等分布定理

   &emsp;&emsp;当$x$为无理数时，序列$f_a(n)=\{a,2a,3a,...\}$ Mod 1后在$[0,1]$上均匀分布,可以证明序列${a,2^2a,3^2a,...}$与序列${p(n)a}$($p(n)$表示第$n$个素数)也具有这个性质。利用这个性质改良平方取中法来生成符合均匀分布的随机数详情可参考wiki词条[Middle-square method](https://www.wikiwand.com/en/Middle-square_method)。
   > [Weyl 的等分布定理与数项级数的收敛性](https://tigertooth4.github.io/post/2019-06/abel-dirichlet-test-fail-reveals-weyl-theorem-on-equidistribution-theorem/)

   > [Weyl sequence](https://www.wikiwand.com/en/Weyl_sequence)


+ 魔数

    &emsp;&emsp;很多类型的文件，其起始的几个字节的内容是固定的（或是有意填充，或是本就如此）。根据这几个字节的内容就可以确定文件类型，因此这几个字节的内容被称为魔数 (magic number)。

+ 大端存储与小端存储
  
  &emsp;&emsp;大端存储类似人的正常思维，小端存储机器处理更方便。
  > 小端存储（Little-Endian）：数据的高字节存储在高地址中，数据的低字节存储在低地址中 。

  > 大端存储（Big-Endian）：数据的高字节存储在低地址中，数据的低字节存储在高地址中。socket编程中网络字节序一般是大端存储

  ```C++
  int i = 1;
  // int在内存中占 4Byte; 
  // a在内存中的分布为 0x1 0x0 0x0 0x0; 
  // 从左到右内存地址降低，也就是高字节地址存放的是a的低字节数据
  ```
  &emsp;&emsp;mnist原始数据文件中32位的整型值是大端存储，C/C++变量是小端存储，所以读取数据的时候，需要对其进行大小端转换。 (注意：只有文件头的个别数字需要大小端转换，其余的60000个有效数据则不需要)

+ 牛顿迭代算法

  &emsp;&emsp;将$f(x)$在$x_0$处进行泰勒展开有：
  $$
  f(x) = \sum_{n = 0}^{\infty}\frac{f^{n}(x_0)}{n!}(x-x_0)^n
  $$
  取泰勒展开的前两项作为$f(x)$的近似，则有：
  $$
  f(x) = f(x_0)+f{'}(x_0)(x-x_0)
  $$
  现在要求$f(x)$的根，则有：
  $$
  f(x) =0 \to x = x_0+\frac{f(x_0)}{f^{'}(x_0)} 
  $$
  于是得到了方程根的近似值，并且可以证明，随着不断按下式进行迭代后可以以任意精度逼近方程$f(x)=0$根：
  $$
  f(x_{n+1}) = f(x_n)-\frac{f(x_n)}{f^{'}(x_n)}
  $$
  我们称这个迭代算法为牛顿迭代算法，其本质思想`是用函数的切线来对函数进行任意点的逼近`。
  > 以求$a$的平方根倒数为例:

  &emsp;&emsp;求$a$的平方根倒数
  $$
  x = \frac{1}{\sqrt{a}}
  $$
  等价于求方程
  $$
  \frac{1}{x^2}-a=0
  $$
  的根,令:
  $$
  f(x) = \frac{1}{x^2} -a
  $$
  则有：
  $$
  f^{'}(x) = \frac{-2}{x^3}
  $$
  得到牛顿迭代式为:
  $$
  x_{n+1} = \frac{x_n}{2}(3-ax_n^2)
  $$
  再结合神奇的**魔术数字**即可得到大名鼎鼎的快速平方根倒数算法：
  ```C++
  float InvSqrt (float x)
  {
      float xhalf = 0.5f*x;
      int i = *(int*)&x;
      i = 0x5f3759df - (i>>1);
      x = *(float*)&i;
      x = x*(1.5f - xhalf*x*x);
      return x;
  }
  ```