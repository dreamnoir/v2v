#ifndef AUTOREGMODEL_H_
#define AUTOREGMODEL_H_

#include <omnetpp.h>

class AutoregModel
{
public:
	AutoregModel();
	AutoregModel(double alpha, double beta, double std);

	void set(double alpha, double beta, double std);
	double getValue();
	void nextValue();

private:
	double alpha;
	double beta;
	double std;
	double value;
};

#endif /* AUTOREGMODEL_H_ */
