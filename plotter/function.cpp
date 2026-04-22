#include <iostream>
#include <string>
#include "muParser.h"
#include <regex>
#include <format>
using namespace std;
class Function {
	private: 
		string _name;
		mu::Parser p;
	public:
		Function(const string& name) : _name{ name }, p{} {}

		double evalAt(double T) {
			string t = format("({})", T);
			string exp = regex_replace(_name, regex("x"), t);
			
			try {
				p.SetExpr(exp);	
				double val = p.Eval();
				return val;
			}
			catch( mu::Parser::exception_type &e) {
				cout << "error: " << e.GetMsg() << endl;
			}
			cout << "BUG!" << endl;
			// BUG
			return 0;
		}
};
