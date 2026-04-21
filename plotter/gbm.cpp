#include "gbm.hpp"
#include <random>
#include <cmath>
#include <iostream>

using namespace std;

double GBM_Function::u() const { return _u; }
double GBM_Function::o() const { return _o; }

void GBM_Function::u(double u) { _u = u; }
void GBM_Function::o(double o) { _o = o; }

double GBM_Function::dB(double dt) const {
	random_device rd;
	mt19937 gen(rd());
	
	normal_distribution<double> d(0, 1);
	double epsilon = d(gen);
	double dB = sqrt(dt) * epsilon;
	cout << dB;
	return dB;
}

double GBM_Function::perform(double dt) const {
	double dS = _u*dt + _o * dB(dt);
	return dS;
}

GBM_Function::GBM_Function(double u, double o) : _u{u}, _o{o} {}
