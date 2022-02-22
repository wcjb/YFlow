/**
 * @file parser_mnist.h
 * @brief 
 * @author 殉道者 (wcjbyjx@gmail.com)
 * @version 1.0
 * @date 2021-01-25
 * @copyright Copyright © 2014-2021 weichijunbo.
 * 
 * *********************************************************************************
 * @par ChangeLog:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2021-01-25 <td>1.0     <td>殉道者     <td>内容
 * </table>
 * *********************************************************************************
 */
#pragma once
/*
 *读取mnist数据集的图像和标签
 *
*/

#include <iostream>
#include <fstream>
#include <string>
#include "Tensor.h"
using namespace std;

/*
 *常用的CPU中的Intel处理器用小端格式，因此需要把大端数据
 *转换为小端是数据进行处理。转换操作为将低8位（0~8位）左移24位，
 *变成了高8位（24~32位），8~16位左移8位变成了（16~24位）。
 *将原高8位和高16位右移，变成了新的低8位和低16位。
*/
inline uint32_t Bige2Little (uint32_t value)
{
    return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 | 
        (value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24; 
}
/**
 * @brief 
 * 
 * @param img_path 
 * @param label_path 
 */
void read(const string &img_path,const string &label_path )
{
    ifstream fimage,flabel;
    fimage.open(img_path,ios::in | ios::binary);
    flabel.open(label_path,ios::in | ios::binary);
    if (!fimage.is_open())
    {
        cerr << "open mnist image file error!" << endl;
        return;
    }
    if (!flabel.is_open())
    {
        cerr << "open mnist label file error!" << endl;
        return;
    }

    uint32_t magic; // 文件中的魔术数
    uint32_t num_img; // mnist图片数据集中的图像数目；
    uint32_t num_lab; // mnist标签数据集中的标签数目
    uint32_t rows; // 图像的行数
    uint32_t cols; // 图像的列数

    //从文件流中读取4个字节（魔术数）并将其从大端转为小端
    fimage.read(reinterpret_cast<char*>(&magic),4); //reinterpret_cast 强制类型转换
    magic = Bige2Little(magic);

    if (magic != 2051)
    {
        cerr << "this is not the mnist image file!" << endl;
        return;
    }

    flabel.read(reinterpret_cast<char*>(&magic),4);
    magic = Bige2Little(magic);
    if (magic != 2049)
    {
        cerr << "this is not the mnist label file!" << endl;
    }
    // 读取数据集中图象数和标签数，判断是否相等
    fimage.read(reinterpret_cast<char*>(&num_img),4);
    num_img = Bige2Little(num_img);
    flabel.read(reinterpret_cast<char*>(&num_lab),4);
    num_lab = Bige2Little(num_lab);

    if (!(num_img == num_lab))
    {
        cerr << "the image file and label file are not a pair！" << endl;
    }
     
    // 读取图像的分辨率
    fimage.read(reinterpret_cast<char*>(&rows),4);
    rows = Bige2Little(rows);
    fimage.read(reinterpret_cast<char*>(&cols),4);
    cols = Bige2Little(cols);

    cout << "magic number = " << magic << endl;
	cout << "number of images = " << num_img << endl;
	cout << "rows = " << rows << endl;
	cout << "cols = " << cols << endl;

    int size = rows * cols;

    unsigned char **image  = new unsigned char* [num_img];
    for (int i = 0;i < num_img;i++)
    {
        image[i] = new unsigned char[size];
    }
    unsigned char *label = new unsigned char[num_lab];
    // 读取图片数据
    unsigned char *pixels = new unsigned char[size];
    cout << "number of label = " << num_lab << endl;
    for (int item = 0;item < num_img;item++)
    {
        fimage.read((char*)pixels,rows * cols);
        for (int i = 0;i < size;i++ )
        {
            image[item][i] = pixels[i];
        }
        unsigned char lab = 0;
        flabel.read(reinterpret_cast<char*>(&lab),sizeof(unsigned char));
        cout << lab << endl;
    }
    
    // for (int i = 0;i < num_lab;i++)
    // {
    //     cout << label;
    // }

    delete[] label;
    delete[] pixels;
    for (int j = 0;j < num_img;j++)
    {
        delete[] image[j];
    }
    delete[] image;

}