#pragma once

#include <iostream>
#include <random>
using namespace std;
template<typename T>
class Matrix
{
private:
    int row;   //张量最低层矩阵的的行
    int col;   //张量最底层矩阵的列
    T **num;

public:
    int dimension;  //张量的维数
    int *shape;     //张量的维度
    string dtype;   //张量的类型
    //std::type_info(int) 
    Matrix (const int _row ,const int _col)
    {
        row = _row;
        col = _col;
        // 随机数引擎，产生指定范围内的数字
        static std::default_random_engine _rand_engine((unsigned int) time(NULL));
        // 随机数分布，产生指定范围内符合指定分布的数
        static std::uniform_real_distribution<double> _rand_init(-1.0,1.0);
        if (row != 0 && col != 0)
        {
            num = new T *[row];
            for (int i = 0;i < row; ++i)
            {
                num[i] = new T[col];
                for ( int j = 0;j < col;++j)
                {
                    num[i][j] = _rand_init(_rand_engine);
                }
            }
        }
        else
        {
            num = NULL;
        }
    }
    Matrix (const int _row,const int _col,T *array)
    {
        row = _row;
        col = _col;
        if (row != 0 && col != 0)
        {
            num = new T *[row];
            for (int i = 0 ;i < row;++i)
            {
                num[i] = new T[col];
                for ( int j = 0;j < col;++j)
                {
                    num[i][j] = array[i*col+j];
                }
            }
        }
        
    }
    Matrix (const Matrix<T> &_temp)
    {
        row = _temp.row;
        col = _temp.col;
        if (row != 0 && col !=0 )
        {
            num  = new T* [row];  //首先指针指向一个一维数组（数组里面元素为指针）
            for (int i = 0;i < row;++i)
            {
                num[i] = new T[col]; ////然后一维数组中每一个元素（指针）再指向一个一维数组
                for(int j = 0;j < col;++j)
                {
                    num[i][j] = _temp.num[i][j];
                }
            }
        }
        else
        {
            num = NULL;
        }
    }

    /*
     析构函数，C++的特殊函数，在每次删除对象时执行，用于跳出程序后，释放所占资源
     */
    ~Matrix()
    {
        if(num!=NULL)
        {
            for(int i=0;i<row;++i)
            {
                delete[] num[i];
            }
            delete num;
        }
    }
    /*
     重载+运算符实现矩阵加法运算
     */
    Matrix operator + (const Matrix<T> &B) const
    {
        if (this->col == B.col && this->row == B.row)
        {
            for (int i = 0; i < this->row; ++i)
            {
                for (int j = 0; j < this->col; ++j)
                {
                    this->num[i][j] += B.num[i][j];
                }
            }
            return *this;
        }
        else if (this->col == B.row && B.col == 1)
        {
            for (int j = 0; j < this->col ;++j)
            {
                for (int i = 0; i < this->row ;++i)
                {
                    this->num[i][j] += B.num[j][1];
                }
            }
            return *this;
        }
        else
        {
            Matrix<T> NullMatrix(0,0);
            std::string WarningInformation="No matching matrix";
            throw WarningInformation;
            return NullMatrix;
        }
    }
   
    /*
     重载-运算符实现矩阵减法运算
     */
    Matrix operator - (const Matrix<T> &B) const
    {
        if (this->col == B.col && this->row == B.row)
        {
            for (int i = 0; i < row;++i)
            {
                for (int j = 0;j < col;++j)
                {
                    this->num[i][j] -= B.num[i][j];
                }
            }
            return *this;
        }
        else
        {
            Matrix<T> NullMatrix(0,0);
            std::string WarningInformation="No matching matrix";
            throw WarningInformation;
            return NullMatrix;
        }
    }
    
    /*
     重载*实现矩阵的乘法运算
     */
    Matrix operator * (const Matrix<T> &B) const
    {
        if (this->row == 0 || this->col == 0 || B.row == 0 ||B.col == 0)
        {
            Matrix<T> NullMatrix(0,0);
            std::string WarningInformation="No matching matrix";
            throw WarningInformation;
            return NullMatrix;
        }
        else if (this->col != B.row)
        {
            Matrix<T> NullMatrix(0,0);
            std::string WarningInformation="No matching matrix";
            throw WarningInformation;
            return NullMatrix;
        }
        else
        {
            Matrix<T> Temp(this->row,B.col);
            T thans;
            for (int i = 0;i < Temp.row;++i)
            {
                for (int j = 0;j < Temp.col;++j)
                {   thans = 0;
                    for (int k = 0;k < this->col;++k)
                    {
                        thans += this->num[i][k]*B.num[k][j];
                    }
                    Temp.num[i][j] = thans;
                }
            }
            return Temp;
        }
    }
    
    /*
     重载=运算符,实现矩阵赋值运算如果operator=是void类型，那么这个运算符是不能完成这个语句的:a=b=c;
     所以使用Matrix &，因为使用引用一方面可以使得上面这个语句行得通，而且效率也比较高。
     */
    Matrix &operator = (const Matrix<T> &B)
    {
        //在进行复制前，保证左边的矩阵是没有指向内存空间的
        if (num == NULL)
        {
            for (int i = 0;i < row;++i)
            {
                delete[] num[i];
            }
            delete num;
        }
        row = B.row;
        col = B.col;
        if (row != 0 && col != 0)
        {
            num = new T *[row];
            for(int i = 0;i < row;++i)
            {
                num[i] = new T[col];
            }
                for (int i = 0;i < row;++i)
                {
                    for (int j = 0;j < col;++j)
                    {
                        num[i][j] = B.num[i][j];
                    }
                }
        }
        else
        {
            row = 0;
            col = 0;
            num = NULL;
        }
        return *this;
    }
    template<typename _T>
    friend std::ostream &operator<<(std::ostream &strm,Matrix<_T> &B);
    template<typename _T>
    friend std::istream &operator>>(std::istream &strm,Matrix<_T> &B);
    template<typename _T>
    // 友元函数实现矩阵的Hadamard
    friend Matrix<_T> Hadamard(const Matrix<_T> &A,const Matrix<_T> &B);
    template<typename _T>
    // 使用友元函数实现矩阵转置
    friend Matrix<_T> Transpose(const Matrix<_T> &B);
    
    template<typename _T>
    Matrix<_T> Zeros(const int _row,const int _col)
    {
        Matrix<_T> temp(_row,_col);
        for(int i=0;i<_row;++i)
            for(int j=0;j<_col;++j)
                temp.num[j][i] = 0;
        return temp;
    }
};

template<typename _T>
std::ostream &operator<<(std::ostream &strm,Matrix<_T> &B)
{
    for(int i=0;i<B.row;++i)
    {
        strm<<"[";
        for(int j=0;j<B.col;++j)
        {
            if(j==B.col-1)
                strm<<B.num[i][j];
            else
                strm<<B.num[i][j]<<" ";
        }
        strm<<"]"<<std::endl;
    }
    return strm;
}

template<typename _T>
std::istream &operator>>(std::istream &strm,Matrix<_T> &B)
{
    for(int i=0;i<B.row;++i)
        for(int j=0;j<B.col;++j)
            strm>>B.num[i][j];
    return strm;
}

template<typename _T>
Matrix<_T> Hadamard(const Matrix<_T> &A,const Matrix<_T> &B)
{
    if(A.row==0||A.col==0||B.row==0||B.col==0)
    {
        Matrix<_T> NullMatrix(0,0);
        std::string WarningInformation="No matching matrix";
        throw WarningInformation;
        return NullMatrix;
    }
    else if(A.row!=B.row||A.col!=B.col)
    {
        Matrix<_T> NullMatrix(0,0);
        std::string WarningInformation="No matching matrix";
        throw WarningInformation;
        return NullMatrix;
    }
    else
    {
        Matrix<_T> Temp(A.row,A.col);
        for(int i=0;i<A.row;++i)
            for(int j=0;j<A.col;++j)
                Temp.num[i][j]=A.num[i][j]*B.num[i][j];
                
        return Temp;
    }
}

template<typename _T>
Matrix<_T> Transpose(const Matrix<_T> &B)
{
    Matrix<_T> temp(B.col,B.row);
    for(int i=0;i<B.row;++i)
        for(int j=0;j<B.col;++j)
            temp.num[j][i] = B.num[i][j];
    return temp;
}