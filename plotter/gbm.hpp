// header for GBM_Function class
class GBM_Function {
	private: 
		double _u;
		double _o;
		double dB(double dt) const;
		
	public:
		GBM_Function(double u, double o) ;
		double u() const;
		void u(double u);
		double o() const ;
		void o(double o);
		
		double perform(double timestep) const;
};
