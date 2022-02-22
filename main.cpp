/**
 * @file main.cpp
 * @brief 基于C++和CUDA的机器学习框架
 * @mainpage
 * @author 殉道者 (wcjbyjx@gmail.com)
 * @version 0.0.1
 * @date 2021-01-25
 * @copyright Copyright © 2014-2021 weichijunbo.
 * @section   项目详细描述
 * &emsp;&emsp;基于C++实现的深度学习框架YFlow，这是作者练手之作，用于总结回顾平时工作中
 * 各类算法，当然，由于局限于个人水平，目前仍有许多不足之处，目前计划是在一年内完成深度学
 * 习相关开发底层实用C++和CUDA混合开发。在实现过程中，我本人也接触学习了许多底层算法计算
 * 优化的技巧，本项目参考了Darknet框架的内容，并尝试进行突破。本项目的最终目标是成为一个
 * 基于C++和CUDA实现的高效率机器学习框架。
 * 
 * @section   功能描述 
 * -# Tensor.h 实现张量数据结构，将以此为基础一一构建庞大的机器学习算法体系，因此该数据结构的计算效率尤为重要,作者本人实现了通用矩阵乘法主流的几种实现方式，详情可参考代码。
 * -# tools.h 实现各类底层计算工具，如快速平方根倒数算法、对数函数的高效计算、高效的泰勒级数实现、各类随机数生成器的实现
 * -# layer.h 基于张量结构搭建单层神经网络，作为底层单元构建神经网络模型
 * 
 * @section   用法描述 
 * -# 待补充
 * -# 待补充
 * 
 * @section   
 * *********************************************************************************
 * @par ChangeLog:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2021-01-25 <td>1.0     <td>殉道者     <td>内容
 * </table>
 * *********************************************************************************
 * */
#include <iostream>
using namespace std;
#include "source/tools.h"
#include "source/ytime.h"
#include <cmath>

int main(int argc, char** argv)
{
	// vector<int> layer_szie = {2,2,8,2};
	// Matrix input(10,layer_szie[0]);
	// Layer layer_01(layer_szie[0], layer_szie[1], "logistic");
	// Layer layer_02(layer_szie[1], layer_szie[2], "relu");
	// Layer layer_03(layer_szie[2], layer_szie[3], "relu");
	// Matrix outputs_01 = layer_01.Forward(input);
	// Matrix outputs_02 = layer_02.Forward(outputs_01);
	// Matrix outputs_03 = layer_03.Forward(outputs_02);

	// cout << "outputs:" << endl << outputs_01 << endl;
	// cout << "outputs:" << endl << outputs_02 << endl;
	// cout << "outputs:" << endl << outputs_03 << endl;
	// string img_dir = "C:\\Users\\wcjb\\Documents\\CodeSpace\\YFlow\\data\\Mnist\\t10k-images-idx3-ubyte\\t10k-images.idx3-ubyte";
	// string lab_dir = "C:\\Users\\wcjb\\Documents\\CodeSpace\\YFlow\\data\\Mnist\\t10k-labels-idx1-ubyte\\t10k-labels.idx1-ubyte";
	// read(img_dir,lab_dir);
	// return 0;

	// int a[4] = {1,2,3,4};
	// cout << sum(a) << endl;

    ///C++中int类型和long类型范围相同-2147483648～2147483647(-2^31~2^31-1)
    // if (typeid(a)==typeid(int))
    // {
    //         cout << "int" << endl;
    // }
	for( int i = 100;i< 200;i+=5 ) 
	{
		float a = __ieee754_log(i);
		float b = log(i);
		cout << a << " " << b << endl;	
	}
	return 0;
}
