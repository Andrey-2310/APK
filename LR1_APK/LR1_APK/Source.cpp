#include<iostream>
using namespace std;

int main()
{
	double a, b, x, res;
	int two;
	cin >> x;

	_asm {
		FLD x
			FLD1
			FADD
			FSIN
			FLD x
			FCOS
			FILD two
			FADD
			FDIV; FDIVR
			FSTP res
	}
	cout << res;
	return 0;
}