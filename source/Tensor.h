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
#include "matrix.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../img/stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../img/stb_image_resize.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../img/stb_image_write.h"

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


enum store
{
    LINE_RGB,              // 以RGBRGBRGB...方式存储，即各颜色通道交替存储，顺序为RGB
    LINE_BGR,              // 以BGRBGRBGR...方式存储，即各颜色通道交替存储，顺序为BGR
    LINE_CONTINUOUS        // 以RR...GG...BB方式存储，即按颜色通道存储，先存储所有R通道数值，在存储GB通道数值
};
template<typename T> 
class Tensor
{
private:
    T* data;     // 由于涉及较多的看拷贝操作，使用共享指针的引用计数技术
public:
    int channel;
    int width;
    int height;
    int store_type; 

    Tensor(std::initializer_list<int> params);
    Tensor (const int& _width,const int& _height,const int& _channel = 3);
    Tensor(const unsigned char* image_ptr,const int& _width,const int& _height,const int& _channel = 3,const int& _stype = LINE_CONTINUOUS);
    
    Tensor(const Tensor<T>& tensor);

    Tensor<T>& operator = (const Tensor<T>& tensor);
    Tensor<T> operator - (const Tensor<T>& tensor);
    Tensor<T> operator + (const Tensor<T>& tensor);
    Tensor<T> operator * (const Tensor<T>& tensor);

    T at(const int& cols,const int& rows,const int& _channel = 3) const;
    
    T max() const;
    T min() const;
    size_t size();
    int argmax() const;
    int argmin() const;
    
    
    template<typename _T>
    friend std::ostream &operator<<(std::ostream &strm,Tensor<_T> &tensor);
    template<typename _T>
    friend std::istream &operator>>(std::istream &strm,Tensor<_T> &tensor);

    template <typename _T>
    friend bool imwrite(const Tensor<_T>& tensor,const char* output_dir,const unsigned char flag);

    template<typename _T>
    friend Tensor<_T> convert(const Tensor<_T>& tensor,const int& target_type);


    ~Tensor()
    {
        delete[] data;
    }
};

template<typename T>
Tensor<T>::Tensor (const int& _width,const int& _height,const int& _channel) : width(_width),height(_height),channel(_channel)
{
    store_type = LINE_CONTINUOUS;
    int length = channel * width *  height;
    data = new T[length];
    // 按字节填充字符0，用于初始化tensor的元素
    memset(data, 0,length * sizeof(T));
}

template <typename T>
Tensor<T>::Tensor(std::initializer_list<int> params)
{
    store_type = LINE_CONTINUOUS;
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
 * @brief 重载=运算符,实现矩阵赋值运算如果operator=是void类型，那么这个运算符是不能完成这个语句的:a=b=c;
 * 所以使用Tensor<T>&，因为使用引用一方面可以使得上面这个语句行得通，而且效率也比较高。
 * 
 * @param tensor 
 * @return * template<typename T>& 
 */
template<typename T>
Tensor<T>& Tensor<T>::operator = (const Tensor<T>& tensor)
{
    if (data != nullptr)
    {
        delete [] data;
    }
    channel = tensor.channel;
    width = tensor.width;
    height = tensor.height;
    store_type = tensor.store_type;
    const int length = channel * width *  height;
    data = new T [length];
    memcpy(data, tensor.data, length * sizeof(T));
		
    return *this;
}
/**
 * @brief 重载运算符-实现张量间的减法运算
 * 
 * @param tensor 
 * @return ** template<typename T> 
 */
template<typename T>
Tensor<T> Tensor<T>::operator - (const Tensor<T>& tensor)
{
    if (this->width == tensor.width && this->height == tensor.height) throw "张量维度不匹配！";

    for (int index = 0;index < this->size;index++)
    {
        this->width -= tensor.width;
    }
}

/**
 * @brief 重载运算符+实现张量间的加法运算
 * 
 * @param tensor 
 * @return ** template<typename T> 
 */
template<typename T>
Tensor<T> Tensor<T>::operator + (const Tensor<T>& tensor)
{
    if (this->width == tensor.width && this->height == tensor.height) throw "张量维度不匹配！";

    for (int index = 0;index < this->size;index++)
    {
        this->width += tensor.width;
    }
}
/**
 * @brief 重载运算符*实现张量间的卷积运算
 * 
 * 
 * @param tensor 
 * @return ** template<typename T> 
 */
template<typename T>
Tensor<T> Tensor<T>::operator * (const Tensor<T>& tensor)
{
    ;
}
/**
 * @brief 获得张量的元素数量
 * 
 * @return * template<typename T> 
 */
template<typename T>
size_t Tensor<T>::size()
{
    return channel * width * height;
}
/**
 * @brief 基于Opencv的Mat类型创建Tensor,需要注意的是，虽然二者底层都是线性存储，但是具体存储的方式
 * 不同。
 * Mat（默认三通道的BGR图像）是按照如下方式存储：
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
 * 由于Opencv体量较大，采用stb_image来进行图片存取，stb_image底层仍为线性存储，只是颜色通道顺序为RGB。
 * @param image_ptr    Mat的data指针
 * @param _width       Mat的column,即图像的宽
 * @param _height      Mat的rows，即图像的高
 * @return * template<typename T>  返回张量
 */
template<typename T>
Tensor<T>::Tensor(const unsigned char*  image_ptr,const int& _width,const int& _height,const int& _channel,const int& _stype)
{
    // 默认输入的Mat通道数为3
    
    channel = _channel;
    width = _width;
    height = _height;
    int length = _width *  _height;
    data = new T [channel * length];
    
    switch(_stype)
    {
        case LINE_CONTINUOUS:
            for (int l = 0; l < length; l++) 
            {
                int index = 3 * l;

                // 测试后发现[]操作赋值比指针赋值慢
                // data[l] = image_ptr[index];
                // data[length + l] = image_ptr[index + 1];
                // data[2 * length + l] = image_ptr[index+2];
                *(data + l) = *(image_ptr + index);
                *(data + length + l) = *(image_ptr + index + 1);
                *(data + 2 * length + l) = *(image_ptr + index +2);
            }
            store_type = LINE_CONTINUOUS;
            break;
        case LINE_BGR:
            for (int l = 0; l < length; l++) 
            {
                int index = 3 * l;
                *(data + l) = *(image_ptr + index + 2);
                *(data + length + l) = *(image_ptr + index +  1);
                *(data + 2 * length + l) = *(image_ptr + index);
            }
            store_type = LINE_BGR;
            break;
        default:
            memcpy(data,image_ptr,channel * length * sizeof(unsigned char));   // 当输入存储为RGB交替存储时
            store_type = LINE_RGB;
            break;
    }
   
}

template <typename T>
Tensor<T>::Tensor(const Tensor<T>& tensor)
{	
    store_type = tensor.store_type;
    width = tensor.width;
    height = tensor.height;
    channel = tensor.channel;
    data = new T [width * height * channel];
    memcpy(data, tensor.data, width * height * channel * sizeof(T));
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
T Tensor<T>::at(const int& cols,const int& rows,const int& _channel) const
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

/**
 * @brief 重载 << 符号,用于标准化输出Tensor
 * 
 * @param strm 
 * @param tensor 
 * @return * template<typename _T>& 
 */
template<typename _T>
std::ostream &operator << (std::ostream &strm,Tensor<_T> &tensor)
{
    int len;
    //在使用cout将数据打印出来的时候，char和unsigned char数据打印出来的是其相应的ASCII码，不是相关的数值
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
                    printf("%4d",*(tensor.data + len));
                    //strm << (double)*(tensor.data + len);
                else
                    printf("%4d",*(tensor.data + len));
                    //strm << (double)*(tensor.data + len) <<" ";
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
/**
 * @brief 读取本地图片并存储为Tensor
 * 
 * @param source_dir  图片的完整路径
 * @param image_type  
 * @return * template<typename T> 
 */
template<typename T>
Tensor<T> imread(char const * source_dir,const int image_type)
{
    int im_width, im_height, im_channel;
    // 加载图片获取宽、高、颜色通道信息
    unsigned char *idata = stbi_load(source_dir, &im_width, &im_height, &im_channel, 0);

    Tensor<T> tensor(idata,im_width,im_height,im_channel,image_type);
    
    stbi_image_free(idata);
    return tensor;
}
/**
 * @brief 将Tensor保存为图片
 * 
 * @param tensor  张量数据
 * @param output_dir  输出的完整路径
 * @param flag     输出的图片类型0,jpg;1,png
 * @return * template <typename _T> 
 */
template <typename _T>
bool imwrite(const Tensor<_T>& tensor,const char* output_dir,const unsigned char flag)
{
    switch (flag)
    {
        case 0:
            stbi_write_jpg(output_dir, tensor.width, tensor.height, tensor.channel, tensor.data, 0);
            break;
        case 1:
            stbi_write_png(output_dir, tensor.width, tensor.height, tensor.channel, tensor.data, 0);
            break;
    }
   
    return true;
}
/**
 * @brief 转换Tensor底层的存储方式，在处理时为方便进行运算，采用分通道存储的方式进行存储，在写入本地文件时，采用
 * 各颜色通道交替存储的方式。主要用于将分通道存储的方式转换为交替存储的方式。
 * @param tensor  需要改变存储方式的张量
 * @param target_type  目标存储方式
 * @return * template<typename _T> 
 */
template<typename _T>
Tensor<_T> convert(const Tensor<_T>& tensor,const int& target_type )
{
    const int length = tensor.width * tensor.height;

    if (tensor.store_type == target_type) return tensor;
    Tensor<_T> _tensor = Tensor<_T>(tensor.width,tensor.height);
    switch(target_type)
    {
        case LINE_RGB:
            for (int l = 0; l < length; ++l)
            {
                int index = 3 * l;
                *(_tensor.data + index)     = *(tensor.data + l);
                *(_tensor.data + index + 1) = *(tensor.data + length + l);
                *(_tensor.data + index + 2) = *(tensor.data + 2 * length + l);
            }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
            _tensor.store_type = LINE_RGB;
            break;
        case LINE_BGR:
            for (int l = 0; l < length; ++l)
            {
                int index = 3 * l;
                *(_tensor.data + index)     = *(tensor.data + 2 * length + l);
                *(_tensor.data + index + 1) = *(tensor.data + length + l);
                *(_tensor.data + index + 2) = *(tensor.data + l);
            }
            _tensor.store_type = LINE_BGR;
            break;
    }
    return _tensor;
} 
