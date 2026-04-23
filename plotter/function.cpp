#include <iostream>
#include <string>
#include "muParser.h"
#include <regex>
#include <format>
#include "function.hpp"

using namespace std;
Function::Function(const string& name) : _name{ name }, _p{} {}

double Function::evalAt(double T) {
	string t = format("({})", T);
	string exp = regex_replace(_name, regex("x"), t);
			
	try {
		_p.SetExpr(exp);	
		double val = _p.Eval();
		return val;
	}
	catch( mu::Parser::exception_type &e) {
		cout << "error: " << e.GetMsg() << endl;
	}
	cout << "BUG!" << endl;
	// BUG
	return 0;
}

double Function::evalDerivativeAt(double t, double dt) {
	double f_top = evalAt(t + dt);
	double f_bot = evalAt(t - dt);
	return (f_top - f_bot) / (2*dt);
}

Function::Function(const Function & f) { 
	_name = f._name;
}	
