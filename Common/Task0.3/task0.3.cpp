#include <iostream>
#include <vector>
#pragma warning(disable : 4996)

struct Matrix
{
	int raws;
	int column;

	Matrix(int raws = 0, int column = 0) : raws(raws), column(column) {}
};

struct Item
{
	int raws;
	int column;
	long long operations;

	Item(int raws = 0, int column = 0, long long operations = 0) : raws(raws), column(column), operations(operations) {}
};

long long findMinNumOfOperations(Matrix *matrixs, int &number)
{
	long long tmpMin = LLONG_MAX;
	long long tmpResult = 0;
	int tmpRaws = 0;
	int tmpColumn = 0;
	Item **tmpMatrix = new Item*[number];
	Item first, second;
	for (int i = 0; i < number; ++i)
	{
		tmpMatrix[i] = new Item[number];
	}

	for (int i = 0; i < number; ++i)
	{
		tmpMatrix[i][i].raws = matrixs[i].raws;
		tmpMatrix[i][i].column = matrixs[i].column;
	}

	for (int amount = 1; amount < number; ++amount)
	{
		for (int cell = 0; cell < number - amount; ++cell)
		{
			tmpMin = LLONG_MAX;
			for (int var = 0 ; var < amount  ; ++var)
			{
				first = tmpMatrix[cell][cell + var];
				second = tmpMatrix[cell + var + 1][amount + cell];
				tmpResult = (first.raws *  first.column *  second.column) + first.operations + second.operations;

				if (tmpResult < tmpMin)
				{
					tmpMin = tmpResult;
					tmpRaws = first.raws;
					tmpColumn = second.column;
				}
			}
			tmpMatrix[cell][amount + cell].raws = tmpRaws;
			tmpMatrix[cell][amount + cell].column = tmpColumn;
			tmpMatrix[cell][amount + cell].operations = tmpMin;
		}
	}
	tmpResult = tmpMatrix[0][number - 1].operations;
	for (int i = 0; i < number; ++i)
	{
		delete[] tmpMatrix[i];
	}
	delete[] tmpMatrix;
	return tmpResult;
}

int main()
{
	FILE *fin, *fout;
	int number = 0;
	Matrix *matrixs;
	long long result;

	fin = fopen("input.txt", "r");
	fout = fopen("output.txt", "w");

	fscanf(fin, "%d", &number);

	matrixs = new Matrix[number];

	int i = 0;
	for (int i = 0; i < number; ++i)
	{
		fscanf(fin, "%d%d", &(matrixs[i].raws), &(matrixs[i].column));
	}

	result = findMinNumOfOperations(matrixs, number);

	fprintf(fout, "%lld", result);

	fclose(fin);
	fclose(fout);
	delete[] matrixs;
	return 0;
}