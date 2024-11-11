#ifndef EXTERNAL_SOURCE_H
#define EXTERNAL_SOURCE_H
#include"matrix2by2.h"
#include"tmm.h"
#include <utility>
#include<vector>

using namespace std;
class externalsource: public tmm
{
	public:
		externalsource();
		~externalsource();
		vector<double> get_tra() override;
		void solve() override;
		int check_highly_absobing();

	private:
		double _T;
		double _R;
		double _A;

};
#endif