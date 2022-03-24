#pragma once

#include <iostream>
#include <random>
#include "yflow_random.h"
#define JPI 1
class matrix
{
private:
    int height;   //张量最低层矩阵的的行
    int width;   //张量最底层矩阵的列
    double *data;

public:
    
    matrix(const int _height,const int _width );
    matrix (double* array,const int _height,const int _width);
    
    matrix (const matrix &_temp);

   
    ~matrix()
    {
        if (data != nullptr) delete data;
    }
   
    matrix operator + (const matrix &mat) const;
    matrix operator - (const matrix &mat) const;
    matrix operator * (const matrix &mat) const;
    
    /*
     重载=运算符,实现矩阵赋值运算如果operator=是void类型，那么这个运算符是不能完成这个语句的:a=b=c;
     所以使用Matrix &，因为使用引用一方面可以使得上面这个语句行得通，而且效率也比较高。
     */
    matrix &operator = (const matrix &mat);

    int size() const
    {
          return width * height;
    }
   
    friend std::ostream &operator<<(std::ostream &strm,matrix& mat);
    
    friend std::istream &operator>>(std::istream &strm,matrix& mat);
    
    friend matrix Hadamard(const matrix& mat_A,const matrix& mat_B);
    
    friend matrix Transpose(const matrix& mat);
    
    friend void write(const char* m_file,const matrix& mat);
    
};


matrix::matrix(const int _height,const int _width) : height(_height), width(_width)
{
    const int size = height * width;
    data = new double[size];
    for (int l = 0;l < size;l++)
    {
        *(data+l) = double_rand();
    }
    //memset(data,0,sizeof(double) * size);
        
}

matrix::matrix(double* array,const int _height,const int _width) : height(_height), width(_width)
{
    const int size = height * width;
    data = new double[size];

    for (int i = 0; i < size; ++i)
    {
        *(data+i) = *(array+i);
    }
}


matrix::matrix (const matrix &_temp)
{
    height = _temp.height;
    width = _temp.width;
    const int size = height * width;
    data = new double[size];
    memcpy(data, _temp.data, sizeof(double) * size);
}   

matrix matrix::operator + (const matrix &mat) const
{
    if (this->width != mat.width && this->height != mat.height) throw "矩阵维度不匹配！";
    const int size = width * height;
    for (int i = 0;i < size;i++)
    {
        this->data[i] += mat.data[i];
    }
    return *this;
}

matrix matrix::operator - (const matrix &mat) const
{
    if (this->width != mat.width && this->height != mat.height) throw "矩阵维度不匹配！";
    const int size = width * height;
    for (int i = 0;i < size;i++)
    {
        this->data[i] -= mat.data[i];
    }
    return *this;
}

matrix matrix::operator * (const matrix &mat) const
{
    if (this->width != mat.height) throw "矩阵维度不匹配！";
    
    matrix _mat(this->height,mat.width);
   
#ifdef IJP   
    for (int row = 0;row < _mat.height; row++)
    {   
        for (int column = 0;column < _mat.width;column++)
        {
            for (int k = 0;k < width;k++)
            {   
                // (row,column) = (row,k)*(k,column)
                _mat.data[row * _mat.width + column] +=  this->data[row * width + k] * mat.data[k * mat.width + column];
            }
        }
    }
#endif

#ifdef JPI
        for (int column = 0;column < _mat.width;column++)
        {   
            for (int k = 0;k < width;k++)
            {
                for (int row = 0;row < _mat.height; row++)
                {   // (row,column) = (row,k)*(k,column)
                    _mat.data[row * _mat.width + column] +=  this->data[row * width + k] * mat.data[k * mat.width + column];
                }
            }

        }
#endif
    return _mat;
}

/*
重载=运算符,实现矩阵赋值运算如果operator=是void类型，那么这个运算符是不能完成这个语句的:a=b=c;
所以使用Matrix &，因为使用引用一方面可以使得上面这个语句行得通，而且效率也比较高。
*/
matrix& matrix::operator = (const matrix &mat)
{
    if (this->data != nullptr) delete this->data;

    this->width = mat.width;
    this->height = mat.height;

    const int size = mat.width * mat.height;
    data = new double[size];
    memcpy(data, this->data, size * sizeof(double));
    return *this;
}
std::ostream &operator<<(std::ostream &strm,matrix &mat)
{
    for(int i=0;i<mat.height;++i)
    {
        strm<<"[";
        for(int j=0;j<mat.width;++j)
        {
            if(j==mat.width-1)
                strm<<mat.data[i * mat.width + j];
            else
                strm<<mat.data[i * mat.width + j]<<" ";
        }
        strm<<"]"<<std::endl;
    }
    return strm;
}

std::istream &operator>>(std::istream &strm,matrix& mat)
{
    const int size = mat.width * mat.height;
    for (int i = 0;i < size;i++)
    {
        strm >> mat.data[i];
    }
    return strm;
}    

matrix Hadamard(const matrix& mat_A,const matrix& mat_B);
    
matrix Transpose(const matrix& mat);


void write(const char* m_file,const matrix& mat)
{
    FILE* fp = fopen(m_file, "w");
    if (fp == nullptr) printf("open error!");
    fwrite(mat.data , sizeof(double), mat.size(), fp);
    fclose(fp);
}