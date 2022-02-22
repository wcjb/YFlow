#pragma once
#include "Tensor.h"
#include "activations.h"

class Layer
{
private:
	int input_size;
	int output_size;
	Tensor<int> weights;
	Tensor bias;
	string ftype;
public:
	Layer(const int _input_size, const int _output_size, const string _ftype) : input_size(_input_size), output_size(_output_size), ftype(_ftype) 
	{
		weights = Tensor(input_size, output_size);
		bias = Tensor(output_size,1);
	}
	Tensor GetWeight(void) const
	{
		return weights;
	}
	Tensor GetBias(void) const
	{
		return bias;
	}
	Tensor Forward(const Tensor mat);
	Tensor BackPropagation(const Tensor mat);
};
Tensor Layer::Forward(const Tensor mat)
{
	
	if (mat.col != input_size)
	{
		throw "输入矩阵维度和权重矩阵维度不匹配！请检查";
	}
	if (this->ftype == "relu")
	{
		activations activation("relu");
		return activation.Relu(mat * weights + bias);
	}
	else if (this->ftype == "logistic")
	{
		activations activation("logistic");
		return activation.Logistic(mat * weights + bias);
	}
	else if (this->ftype == "leakyrelu")
	{
		activations activation("leakyrelu");
		return activation.LeakyRelu(mat * weights + bias);
	}
	else
	{
		throw "未定义激活函数";
	}
}

Tensor Layer::BackPropagation(const Tensor mat)
{
	return mat;
}
#endif