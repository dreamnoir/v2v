#include "AutoregModel.h"

AutoregModel::AutoregModel()
{
	this->alpha = 0;
	this->beta = 0;
	this->std = 0;
	this->value = 0;
}

AutoregModel::AutoregModel(double alpha, double beta, double std)
{
	set(alpha, beta, std);
	this->value = 0;
}

void AutoregModel::set(double alpha, double beta, double std)
{
	this->alpha = alpha;
	this->beta = beta;
	this->std = std;
}

double AutoregModel::getValue()
{
	return value;
}

void AutoregModel::nextValue()
{
	value = alpha*value + beta*normal(0, std);
}
