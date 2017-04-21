#include<iostream>
#include<math.h>
#include<time.h>
using namespace std;
int Calculating_with_CoProcessor(double, double, double);
int Calculating_with_Math(double, double, double);
int main()
{
	setlocale(LC_ALL, "Russian");
	double a, b, d, res = 0;
	char ident;
	do
	{
		cout << "������� ��������� ��������, �������� ��������, ��� �� ������ ����" << endl;
		cin >> a >> b >> d;
		Calculating_with_CoProcessor(a, b, d);
		Calculating_with_Math(a, b, d);
		cout << "������� 1 ��� ����������, ����� ������ ��� ������: ";
		cin >> ident;
	} while (ident=='1');
	return 0;
}

int Calculating_with_CoProcessor(double a, double b, double d)
{
	double res = 0;
	unsigned int beg, end;
	double x;
	cout << endl << "���������� � ������� ������������" << endl;
	beg = clock();
	_asm FINIT
	for (int i = 0; a + i*d <= b; i++)
	{	x = a + i*d;
		_asm { 
				FLD x	//������ � ���� X
				FLD x	//������ � ���� X
				FMUL    // x*x
				FLD x   //������ � ���� X
				FSUB    //x*x-x
				FLD x   //������ � ���� X
				FABS    //abs(x)
				FSQRT   //sqrt(abs(x))
				FADD    //x*x-x+sqrt(abs(x))
				FSTP res//����� ���������� � ���������� res
		}
	}
	_asm FWAIT
	end = clock();
	cout << "����������� �����: " << end - beg << ", ���������� ��������: " << res << endl;
	return 0;
}

int Calculating_with_Math(double a, double b, double d)
{
	double res = 0;
	unsigned int beg, end;
	double x;
	cout << endl << "���������� � ������� ���������� Math.h" << endl;
	beg = clock();
	for (int i = 0; a + i*d <= b; i++)
	{
	    x = a + i*d;
		res = x*x - x + sqrt(abs(x));
	//	cout  << res << endl;
	}
	end = clock();
	cout << "����������� �����: " << end - beg << ", ���������� ��������: " << res << endl;
	return 0;
}