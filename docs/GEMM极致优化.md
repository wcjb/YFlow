&emsp;&emsp;通用矩阵乘法（GEMM，General Matrix Multiplication）在高性能计算、深度学习等领域都有着重要应用，本文将从基于理论的数学计算方法，逐步优化得到高效简洁的高性能矩阵乘法优化算法。
> 已知矩阵$A_{(m,k)},B_{(k,n)}$,计算二者的乘积$C_{(m,n)} = A_{(m,k)}B_{(k,n)}$,则有：
$$
c_{(i,j)} = \sum_{i = 0}^{k}a_{(i,k)}b_{(k,j)}
$$
其中$c_{(i,j)}$表示矩阵$C$中第$i$行第$j$列的元素。

## 基本实现
&emsp;&emsp;为方便编程实现，约定矩阵元素为列序线性存储。
> 以列序为主（先列后行）：按照行号从小到大的顺序，依次存储每一列的元素
> 以行序为主（先行后序）：按照列号从小到大的顺序，依次存储每一行的元素。

```c
/* Create macros so that the matrices are stored in column-major order */
#define A(i,j) a[ (j)*lda + (i) ]
#define B(i,j) b[ (j)*ldb + (i) ]
#define C(i,j) c[ (j)*ldc + (i) ]

/* Routine for computing C = A * B + C */

void mult( int m, int n, int k, double *a, int lda, 
                                    double *b, int ldb,
                                    double *c, int ldc )
{
  int i, j, p;

  for ( i=0; i<m; i++ ){        /* Loop over the rows of C */
    for ( j=0; j<n; j++ ){        /* Loop over the columns of C */
      for ( p=0; p<k; p++ ){        /* Update C( i,j ) with the inner
				       product of the ith row of A and
				       the jth column of B */
	C( i,j ) = C( i,j ) +  A( i,p ) * B( p,j );
      }
    }
  }
}
```

## 参考文献

1. [How to optimize gemm](https://github.com/flame/how-to-optimize-gemm/wiki)
2. [高性能计算初步：矩阵乘法](https://www.twisted-meadows.com/high-performance-gemm/)
3. [多通道卷积后通道数计算理解](https://codeantenna.com/a/DMtoMdaZyd)
4. [CNN C++实现：im2col + gemm](https://zhuanlan.zhihu.com/p/90301095)
5. [通用矩阵乘（GEMM）优化与卷积计算](https://zhuanlan.zhihu.com/p/66958390)