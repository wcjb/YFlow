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
#include <string.h>
#include "Matrix.h"
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
    int channel;
    int width;
    int height;
    T* data;     // 由于涉及较多的看拷贝操作，使用共享指针的引用计数技术
public:
    Tensor(std::initializer_list<int> params);
    Tensor (const int& _width,const int& _height,const int& _channel = 3);
    Tensor(const unsigned char* const image_ptr,const int& _width,const int& _height);
    

   
    T at(const int& cols,const int& rows,const int& _channel = 3) const;
    
    
    T max() const;
    T min() const;
    
    int argmax() const;
    int argmin() const;
    
    template<typename _T>
    friend std::ostream &operator<<(std::ostream &strm,Tensor<_T> &tensor);
    template<typename _T>
    friend std::istream &operator>>(std::istream &strm,Tensor<_T> &tensor);

    ~Tensor()
    {
        delete[] data;
    }
};

template<typename T>
Tensor<T>::Tensor (const int& _width,const int& _height,const int& _channel) : width(_width),height(_height),channel(_channel)
{
    int length = channel * width *  height;
    data = new T[length];
    // 按字节填充字符0，用于初始化tensor的元素
    memset(data, 0,length * sizeof(T));
}

template <typename T>
Tensor<T>::Tensor(std::initializer_list<int> params)
{
    std::initializer_list<int>::iterator index = params.begin();

    width = *index;
    height = *(index+1);

    if ((index+2) == params.end())
    {
        channel = 3;
    }
    else
    {
         channel = *(index+2);
    }
    const int length = channel * width * height;
    data = new T [length];
    memset(data, 0, length * sizeof(T));
}
/**
 * @brief 基于Opencv的Mat类型创建Tensor,需要注意的是，虽然二者底层都是线性存储，但是具体存储的方式
 * 不同。
 * Mat是按照如下方式存储：
 * ==========================================================================================
 * |                 Column 0         Column 1         Column ...           Column m         |
 * | Row 0      b_00,g_00,r_00     b_01,g_01,r_01    ... , ... , ...      b_0m,g_0m,r_0m     |
 * | Row 1      b_10,g_10,r_10     b_11,g_11,r_11    ... , ... , ...      b_1m,g_1m,r_1m     |
 * | Row ...    ... , ... , ...    ... , ... , ...   ... , ... , ...      ... , ... , ...    |
 * | Row n      b_n0,g_n0,r_n0     b_n1,g_n1,r_n1    ... , ... , ...      b_nm,g_nm,r_nm     |
 * ===========================================================================================
 * 然后依次将每一行首尾连接，即为Mat的线性存储方式。
 *
 * Tensor按照如下方式存储：
 * ==========================================================================================
 * |                 0       1       2      ...      column       ...       column * rows   | 
 * | channel 0 (R)   r_0,   r_1,    r_2,    ...,    r_column,     ...,      r_column*rows   |
 * | channel 1 (G)   g_0,   g_1,    g_2,    ...,    g_column,     ...,      g_column*rows   |
 * | channel 2 (B)   b_0,   b_1,    b_2,    ...,    b_column,     ...,      b_column*rows   |
 * ==========================================================================================
 * 同上，依次将每一行首尾连接，即为Tensor的线性存储方式。
 * @param image_ptr    Mat的data指针
 * @param _width       Mat的column,即图像的宽
 * @param _height      Mat的rows，即图像的高
 * @return * template<typename T>  返回张量
 */
template<typename T>
Tensor<T>::Tensor(const unsigned char* const image_ptr,const int& _width,const int& _height)
{

}


/**
 * @brief 根据给定的坐标访问图像中某个位置的元素值，坐标系等同于OPencv的坐标系,Tensor底层
 * 为线性存储，从 0 到 width * height 个数是第 0 个通道的特征图内容，从width * height到
 * 2 * width * height是第 1 个通道的特征图内容，以此类推。
 * 
 * @param cols 列，即图像的宽，x
 * @param rows 行，即图像的高,y
 * @param _channel 图像的通道数
 * @return * T  （cols,rows）处的元素值
 */
template<typename T>
T Tensor<T>::at(const int& cols,const int& rows,const int& _channel = 3) const
{
    int index = _channel * width * height + cols * width + rows;
    return *(data+index);
}  

/**
 * @brief 获取Tensor中最大的元素所在线性位置
 * 
 * @return * int 返回最大值所在线性位置
 */
template<typename T>
int Tensor<T>::argmax() const
{
    if (data == nullptr) return 0;

    const int lenght = channel * width * height;
    T max_value = *data;
    int max_index = 0;

    for (int i = 1; i < lenght; i++)
    {
        if (max_value < *(data+i))
        {
            max_value = *(data+i);
            max_index = i;
        }
    }
    return max_index;
}

/**
 * @brief 获取Tensor中最小的元素所在线性位置
 * 
 * @return * int 返回最大值所在线性位置
 */
template<typename T>
int Tensor<T>::argmin() const
{
    if (data == nullptr) return 0;
    const int lenght = channel * width * height;
    T min_value = *data;
    int min_index = 0;

    for (int i = 1; i < lenght; i++)
    {
        if (min_value > *(data+i))
        {
            min_value = *(data+i);
            min_index = i;
        }
    }
    return min_index;
}

/**
 * @brief 获取该Tensor中的最大值
 * 
 * @return * T 返回最大值
 */
template<typename T>
T Tensor<T>::max() const
{
    if (data == nullptr) return 0;

    const int lenght = channel * width * height;
    T max_value = *data;

    for (int i = 1; i < lenght; i++)
    {
        if (max_value < *(data+i))
        {
            max_value = *(data+i);
        }
    }
    return max_value;
}


/**
 * @brief 获取该Tensor的最小值
 * 
 * @return * int 返回最小值
 */
template<typename T>
T Tensor<T>::min() const
{
    if (data == nullptr) return 0;

    const int lenght = channel * width * height;
    T min_value = *data;
        
    for (int i = 1; i < lenght; i++)
    {
        if (min_value > *(data+i))
        {
            min_value = *(data+i);
        }
    }
    return min_value;
} 


template<typename _T>
std::ostream &operator << (std::ostream &strm,Tensor<_T> &tensor)
{
    int len;
    strm << "shape:<" << tensor.width << "," << tensor.height << "," << tensor.channel << ">" << std::endl;
    strm<<"[";
    for (int c = 0; c < tensor.channel;c++) 
    {
        strm<<"[";
        // 高
        for(int row = 0;row < tensor.height;++row)
        {
            strm<<"[";
            // 宽
            for(int col = 0;col < tensor.width;++col)
            {
                len  = c * tensor.width * tensor.height + tensor.width * row + col;
                if(col == tensor.width - 1)
                    strm << *(tensor.data + len );
                else
                    strm << *(tensor.data + len) <<" ";
            }
            if (len ==  tensor.channel * tensor.width * tensor.height-1)
            {
                strm<<"]";
            }
            else
            {
                if (row == tensor.height -1) 
                {
                    strm<<"]";
                }
                else
                {
                    strm<<"]," << std::endl;
                }
            }
        }
        if (c == tensor.channel-1)
        {
             strm<<"]";
        }
        else
        {
             strm<<"]"<<std::endl;
        }
    }
    strm<<"]";
    return strm;
}

template<typename _T>
std::istream &operator >> (std::istream &strm,Tensor<_T>& tensor)
{
   const int lenght = tensor.channel * tensor.width * tensor.height;
   for (int l = 0; l < lenght;l++)  strm >> *(tensor.data+l);
   return strm;
}
