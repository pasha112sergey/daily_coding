#include "gbm.hpp"
#include <random>
#include <cmath>
#include <iostream>

using namespace std;


double GBM_Function::dB() const {
	random_device rd;
	mt19937 gen(rd());
	
	normal_distribution<double> d(0, 1);
	double epsilon = d(gen);
	double dB = sqrt(_dt) * epsilon;
	return dB;
}

double GBM_Function::dy(double t) {
	double dy = _u.evalDerivativeAt(t, _dt) * _dt + _o.evalAt(t) * dB();
	return dy;
}

GBM_Function::GBM_Function(const Function &f, const Function &g, double dt) :  _u{f}, _o{g}, _dt{dt} {}
