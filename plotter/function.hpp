#ifndef FUNCTION
#define FUNCTION
#include "muParser.h"
using namespace std;
class Function {
	private: 
		string _name;
		mu::Parser _p;
	
	public:
		Function(const string &name);
		Function(const Function &f);
		double evalAt(double T);
};

#endif
