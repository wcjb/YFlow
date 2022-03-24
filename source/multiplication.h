#pragma once

// 创建宏，矩阵按列主顺序存储
#define A(i,j) a[ (j)*lda + (i) ]
#define B(i,j) b[ (j)*ldb + (i) ]
#define C(i,j) c[ (j)*ldc + (i) ]

/**
 * @brief 常规矩阵乘法实现
 * 
 * @param m 矩阵C的行
 * @param n 矩阵C的列
 * @param k 矩阵A的列、矩阵B的行
 * @param a 矩阵A的数据指针
 * @param lda 矩阵A的列数
 * @param b 矩阵B的数据指针
 * @param ldb 矩阵B的列数
 * @param c   矩阵C的数据指针
 * @param ldc 矩阵C的列数
 * @return ** void 
 */
void multiply(int m, int n, int k,  double *a, int lda, 
                                    double *b, int ldb,
                                    double *c, int ldc )
{
  int i, j, p;

  for ( i=0; i<m; i++ )
  {        
    for ( j=0; j<n; j++ )
    {       
        for ( p=0; p<k; p++ )
        {
	        C( i,j ) +=  A( i,p ) * B( p,j );
        }
    }
  }
}


