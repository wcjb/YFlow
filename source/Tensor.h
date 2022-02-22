/**
 * @file Tensor.h
 * @author 殉道者  wcjbyjx@gmail.com
 * @version 0.0.1
 * @date 2022-02-22 11:50
 * @copyright Copyright © 2014-2022 weichijunbo.
 * @brief 实现张量数据结构
 * @par ChangeLog:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2022-02-22 <td>0.0.1     <td>殉道者     <td>内容
 * </table>
 * */
#pragma once
#include <iostream>
#include <random>
#include "Matrix.h"
using namespace std;

/* *
 * @class  <Tensor> [Tensor.h] [<Tensor>] 
 * &emsp;&emsp;实现张量数据结构，用于搭建神经网络，本质是多维矩阵。需要注意的是，当进行2维以上张量的运算时，两个张量之间的
 * 加减和二维矩阵类似。而两个高维张量的乘法可参考Tensorflow中matmul的源码说明来看，如果张量的dimention大于2，
 * 实际上进行的会是batch_mat_mul,此时进行叉乘的是batch中的每一个切片（slice），即高维张量的运算实质还是2维张
 * 量的运算，只不过是在更高维度的切片下，如下所示：
 *                  output[..., :, :] = matrix(x[..., :, :]) * matrix(y[..., :, :])
 * 使用tensorflow验证如下:
 * @code
 *
 *  In [2]: a
 *  Out[2]: 
 *   <tf.Tensor: shape=(2, 3, 2), dtype=int32, numpy=
 *  array([[[1, 1],
 *           [2, 2],
 *           [3, 3]],
 *
 *       [[4, 4],
 *           [5, 5],
 *           [6, 6]]])>
 *
 *  In [3]: c = tf.reshape(a,(2,2,3))
 *
 *  In [4]: c
 *  Out[4]: 
 *   <tf.Tensor: shape=(2, 2, 3), dtype=int32, numpy=
 *   array([[[1, 1, 2],
 *           [2, 3, 3]],
 *
 *       [[4, 4, 5],
 *           [5, 6, 6]]])>
 *
 *   In [5]: tf.matmul(a,c)
 *   Out[5]: 
 *   <tf.Tensor: shape=(2, 3, 3), dtype=int32, numpy=
 *   array([[[ 3,  4,  5],
 *           [ 6,  8, 10],
 *           [ 9, 12, 15]],
 *
 *       [[36, 40, 44],
 *         [45, 50, 55],
 *            [54, 60, 66]]])> 
 * @endcode
 * 则要求高维张量相乘符合如下规则：
 * -# 两个张量能相乘，则二者除了最后两个维度可以不相等外，其它维度必须相等；
 * -# 两个张量能相乘，则二者的最后两个维度必须满足二维矩阵相乘的条件；
 * 
 * @attention
 *  另外，因为张量乘法本质上为矩阵乘法，由于在神经网络中设计到大量的巨型矩阵的乘法，所以很有必要
 *优化矩阵乘法的计算逻辑，本实现使用通用矩阵乘（GEMM，General Matrix Multiplication）优化矩阵
 *乘法。可参考下文:
 [通用矩阵乘（GEMM）优化算法](https://jackwish.net/2019/gemm-optimization.html)
 [通用矩阵乘（GEMM）优化与卷积计算](https://zhuanlan.zhihu.com/p/66958390)
 *常规的矩阵乘法计算算法的时间复杂度为O(n^3) = O(n^(log2(8)))
 * -# 分块矩阵计算
 * -# Strassen 算法，时间复杂度是O(n^2.807) = O(n^(log2(7)))
 * -# Coppersmith-Winograd算法，时间复杂度是O（n^2.38) 
 * */
template<typename T> 
class Tensor
{
private:
    /**
     * @brief 构建2维及以上维度张量的最小组成部分-矩阵,
     */
    struct matrix
    {
        int rows;   /*!<张量最低层矩阵的的行*/
        int cols;   /*!<张量最底层矩阵的列*/
        T **numpy;    /*!<二级指针存储矩阵元素*/
    };
    /**
     * @brief 一维张量即矢量
     */
    struct vector
    {
        int number;  /*!<矢量的大小*/
        T *numpy;    /*!<指针，存储矢量元素*/
    };
    /*!<零阶张量即常数*/
    T constant;

public:
    int Dimension;       /*!<Tensor的维数*/
    int *Shape;          /*!<Tensor的维度*/
    string dtype;        /*!<Tensor的类型*/
    
    /**
     * @brief Construct a new Tensor object
     * @param  demension        张量的维数0，1，2，3,最高支持三维张量
     * @param  shape            张量的维度，传入参数必须为数组或指针对象
     */
    Tensor (const int demension,const int *shape) : Dimension(demension),Shape(shape)
    {

        matrix mat;
        mat.rows = *(shape+demension-1);
        mat.cols = *(shape+demension-2);

        switch (demension)
        {
        ///三维张量
        case 3 :
            //三维张量第一维的值
            int size = shape[0];   
            //初始化matrix结构体的二级指针成员numpy，即分配内存空间
            mat.numpy = new T* [size];
            for (int j = 0;j < size;j++)
            {
                mat.numpy[j] = new T* [mat.rows * mat.cols];
            }

            if (size!=0 && mat.rows!=0 && mat.cols!=0)
            {
                #pragma omp parallel for
                for (register int s = 0;s < size;s++)
                {
                    for (register int r = 0;r < mat.rows;r++)
                    {
                        for (register int c = 0;c < mat.cols;c++)
                        {
                            mat.numpy[s][c+r*c] = 1;
                        }
                    }
                }
            }
            break;
        ///二维张量即矩阵   
        case 2 :
            break;
        
        //一维张量即矢量
        case 1 :
            break;

        //零维张量即标量
        case 0 :
            break;
        }
    }
};