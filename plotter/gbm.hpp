// header for GBM_Function class
#ifndef STOCHASTIC_FUNC
#define STOCHASTIC_FUNC
#include "function.hpp"

class GBM_Function {
	private: 
		double _dt;
		Function _u;
		Function _o;
	
		double dB() const;
			
	public:
		GBM_Function(const Function &u, const Function &o, double dt);
		double dy(double t);
};

#endif 
