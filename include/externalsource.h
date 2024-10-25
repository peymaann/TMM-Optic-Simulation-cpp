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
		void solve() override;
	private:
};
#endif