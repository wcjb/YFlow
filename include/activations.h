#ifndef __ACTIVATIONS__
#define __ACTIVATIONS__

#pragma once

#include "Tensor.h"

class activations
{
private:
	string type;
public:
	activations(string functype) : type(functype) {};
	
	Tensor inline Logistic(const Tensor mat)
	{	
		Tensor logistic(mat);
		for (int i = 0; i < mat.row; i++)
		{
			for (int j = 0; j < mat.col; j++)
			{
				logistic.num[i][j] = 1/(1+exp(-logistic.num[i][j]));
			}
		}
		return logistic;
	}

	Tensor inline Relu(const Tensor mat)
	{
		Tensor relu(mat);
		for (int i = 0; i < mat.row; i++)
		{
			for (int j = 0; j < mat.col; j++)
			{
				if (relu.num[i][j] >= 0)
				{
					relu.num[i][j] = relu.num[i][j];
				}
				else
				{
					relu.num[i][j] = 0;
				}
			}
		}
		return relu;
	}
		
	Tensor inline LeakyRelu(const Tensor mat,double lambda = 0.01)
	{

		Tensor leakyrelu(mat);
		for (int i = 0; i < mat.row; i++)
		{
			for (int j = 0; j < mat.col; j++)
			{
				if (leakyrelu.num[i][j] >= 0)
				{
					leakyrelu.num[i][j] = leakyrelu.num[i][j];
				}
				else
				{
					leakyrelu.num[i][j] = lambda*leakyrelu.num[i][j];
				}
			}
		}
		return leakyrelu;

	}
	
};

#endif // !__ACTIVATIONS__
