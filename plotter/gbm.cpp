#include "gbm.hpp"
#include <random>
#include <cmath>
#include <iostream>

using namespace std;

double GBM_Function::dt() const { return _dt; }
double GBM_Function::u() const { return _u; }
double GBM_Function::o() const { return _o; }
Function GBM_Function::f() const {return _f;};

void GBM_Function::f(const Function &f) { _f = Function(f); }
void GBM_Function::u(double u) { _u = u; }
void GBM_Function::o(double o) { _o = o; }
void GBM_Function::dt(double dt) { _dt = dt; }

double GBM_Function::dB() const {
	random_device rd;
	mt19937 gen(rd());
	
	normal_distribution<double> d(0, 1);
	double epsilon = d(gen);
	double dB = sqrt(_dt) * epsilon;
	return dB;
}

double GBM_Function::dy(double t) {
	double dy = _f.evalDerivativeAt(t, _dt) * _dt + _o * dB();
	return dy;
}

GBM_Function::GBM_Function(const Function &f, double o, double dt) :  _f{f}, _o{o}, _dt{dt} {}
