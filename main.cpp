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
 * *********************************************************************************
 * @par ChangeLog:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2021-01-25 <td>1.0     <td>殉道者     <td>内容
 * </table>
 * *********************************************************************************
 * */
#include <iostream>
#include "source/tools.h"
#include "source/ytime.h"
#include "source/ylog.h"
#include <cmath>
#include<iomanip>
#include "source/Tensor.h" 
#include "source/multiplication.h"
int main()
{
	// {
	// 	Timer t;
	// 	std::string str = "C:/Users/wcjb/Documents/CodeSpace/YFlow/data/1.jpg";
	// 	std::string out_str  =  "C:/Users/wcjb/Desktop/002.jpg";
	// 	Tensor<unsigned char> tensor = imread<unsigned char>(str.c_str(),LINE_CONTINUOUS);
	// 	auto tt = convert(tensor,LINE_RGB);
	// 	imwrite(tt,out_str.c_str(),0);
	// }
	
	Timer time;
	auto a = matrix(240,240);
	auto b = matrix(240,240);
	for (int i = 0;i < 100;i++)
	{
		auto c = a * b;
	}
	auto c = a * b;
	std::cout << c << std::endl;
	return 0;
}
