#include<iostream>
#include<locale.h>
#include<stdlib.h>
#include<time.h>
#define N 8
#pragma warning(disable : 4996)
void Elapsed_Time(int copyMatrix[N][N], int time);
int main()
{

	setlocale(LC_ALL, "Russian");
	int beg, end;
	long  times;
	char ch;
	do
	{
		printf("Введите количество повторений: ");
		scanf("%ld", &times);
		int mainMatrix[N][N], copyMatrixC[N][N], copyMatrixASM[N][N], copyMatrixMMX[N][N];

		/*Array initialization*/
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				mainMatrix[i][j] = i * N + j;

		/*Calculation_with_C*/
		beg = clock();
		for (int k = 0; k < times; k++)
			for (int i = 0; i < N; i++)
				for (int j = 0; j < N; j++)
					copyMatrixC[i][j] = mainMatrix[i][j];
		end = clock();
		Elapsed_Time(copyMatrixC, end - beg);

		/*Calculation_with_ASM*/
		int sizeOfInt = sizeof(int);
		int arrayInBytes = N*N * sizeof(int);
		beg = clock();
		for (long k = 0; k < times; k++)
			_asm
		{
			pusha;
			xor esi, esi
				xor ecx, ecx
				loop1 :
			mov ecx, N
				loop2 :
			mov eax, mainMatrix[esi]
				mov copyMatrixASM[esi], eax
				add esi, sizeOfInt
				loop loop2
				cmp arrayInBytes, esi
				jg loop1
				popa
		}
		end = clock();

		Elapsed_Time(copyMatrixASM, end - beg);

		/*Calculation_with_MMX*/
		int twoInts = 2 * sizeof(int);
		beg = clock();
		for (long k = 0; k < times; k++)
			_asm
		{
			pusha;
			xor esi, esi
				xor ecx, ecx
				loopMMX1 :
			mov ecx, N
				loopMMX2 :
			movq MM0, mainMatrix[esi]
				movq copyMatrixMMX[esi], MM0
				add esi, twoInts
				loop loopMMX2
				cmp arrayInBytes, esi
				jg loopMMX1
				EMMS
				popa

		}
		end = clock();
		Elapsed_Time(copyMatrixMMX, end - beg);
		printf("Желаете повторить? y/n:  ");
		rewind(stdin);
		scanf("%c", &ch);
	}while(ch=='y');
	return 0;
}

void Elapsed_Time(int copyMatrix[N][N], int time)
{
	printf("\nЗатраченное время : %d\n ", time);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			printf("%d ", copyMatrix[i][j]);
		printf("\n");
	}
}

