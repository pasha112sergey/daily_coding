// header for GBM_Function class
#ifndef STOCHASTIC_FUNC
#define STOCHASTIC_FUNC
#include "function.hpp"

class GBM_Function {
	private: 
		double _u;
		double _o;
		double _dt;
		Function _f;
		
		double dB() const;
			
	public:
		GBM_Function(double u, double o, double dt) ;
		GBM_Function(const Function &f, double o, double dt);
		

		double u() const;
		void u(double u);
		double o() const ;
		void o(double o);
		Function f() const;
		void f(const Function &);
		double dt() const;
		void dt(double);		

		double dy(double t);
};

#endif 
