#include <iostream>
#include <vector>
#pragma warning(disable : 4996)

struct Step 
{
	char operation;
	long long max;
	long long min;

	Step(): max(LLONG_MIN), min(LLONG_MAX), operation(' ') {}
	
};

struct Element 
{
	char operation;
	long long top;

	Element(): operation(' '), top(0) {}
};

long long firstStep(Step** matrix, Element *elementList, int &number)
{
	Element first, second;
	long long tmpMax = LLONG_MIN;
	long long tmpMin = LLONG_MAX;
	long long tmpResult = 0;
	Step operand1, operand2;

	for (int i = 0; i < number; ++i)
	{
		matrix[i][i].max = elementList[i].top;
		matrix[i][i].min = elementList[i].top;
		matrix[i][i].operation = elementList[i].operation;
	}

	for (int i = 0; i < number - 1; ++i)
	{
		first = elementList[i];
		second = elementList[i + 1];

		if (second.operation == 't')
		{
			matrix[i][i + 1].max = first.top + second.top;
			matrix[i][i + 1].min = first.top + second.top;
			matrix[i][i + 1].operation = first.operation;
		}
		else
		{
			matrix[i][i + 1].max = first.top * second.top;
			matrix[i][i + 1].min = first.top * second.top;
			matrix[i][i + 1].operation = first.operation;
		}
	}

	for (int i = 2; i < number; ++i)
	{
		for (int cell = 0; cell < number - i; ++cell)
		{
			for (int var = 0; var < i; ++var)
			{
				operand1 = matrix[cell][cell + var];
				operand2 = matrix[cell + var + 1][i + cell];

				if (operand2.operation == 't')
				{
					tmpResult = operand1.min + operand2.min;
					tmpMax = (tmpResult > tmpMax) ? tmpResult : tmpMax;
					tmpMin = (tmpResult < tmpMin) ? tmpResult : tmpMin;

					tmpResult = operand1.max + operand2.max;
					tmpMax = (tmpResult > tmpMax) ? tmpResult : tmpMax;
					tmpMin = (tmpResult < tmpMin) ? tmpResult : tmpMin;

					tmpResult = operand1.max + operand2.min;
					tmpMax = (tmpResult > tmpMax) ? tmpResult : tmpMax;
					tmpMin = (tmpResult < tmpMin) ? tmpResult : tmpMin;

					tmpResult = operand1.min + operand2.max;
					tmpMax = (tmpResult > tmpMax) ? tmpResult : tmpMax;
					tmpMin = (tmpResult < tmpMin) ? tmpResult : tmpMin;
				}
				else
				{
					tmpResult = operand1.max * operand2.max;
					tmpMax = (tmpResult > tmpMax) ? tmpResult : tmpMax;
					tmpMin = (tmpResult < tmpMin) ? tmpResult : tmpMin;

					tmpResult = operand1.min * operand2.min;
					tmpMax = (tmpResult > tmpMax) ? tmpResult : tmpMax;
					tmpMin = (tmpResult < tmpMin) ? tmpResult : tmpMin;

					tmpResult = operand1.max * operand2.min;
					tmpMax = (tmpResult > tmpMax) ? tmpResult : tmpMax;
					tmpMin = (tmpResult < tmpMin) ? tmpResult : tmpMin;

					tmpResult = operand1.min * operand2.max;
					tmpMax = (tmpResult > tmpMax) ? tmpResult : tmpMax;
					tmpMin = (tmpResult < tmpMin) ? tmpResult : tmpMin;
				}
			}

			matrix[cell][i + cell].max = tmpMax;
			matrix[cell][i + cell].min = tmpMin;
			matrix[cell][i + cell].operation = elementList[cell].operation;

			tmpMax = LLONG_MIN;
			tmpMin = LLONG_MAX;
		}
	}
	return matrix[0][number - 1].max;
}

void shift(Step** matrix, int &number)
{
	Step tmp;
	tmp = matrix[0][0];
	for (int i = 1; i < number; ++i)
	{
		matrix[i - 1][i - 1] = matrix[i][i];
	}
	matrix[number - 1][number - 1] = tmp;
	for (int i = 1; i < number - 1; ++i)
	{
		for (int j = i + 1; j < number; ++j)
		{
			matrix[i - 1][j - 1] = matrix[i][j];
		}
	}
}

void findMaxBranches(Element *elementList, Step** matrix, int &number, long long &maximum, int &numOfBranches, int *result)
{
	Step operand1, operand2;
	long long tmpMax = LLONG_MIN;
	long long tmpMin = LLONG_MAX;
	long long tmpResult = 0;
	long long *maxFromBranch = new long long[number];
	maxFromBranch[0] = firstStep(matrix, elementList, number);
	maximum = maxFromBranch[0];

	for (int i = 1; i < number; ++i)
	{
		shift(matrix, number);

		for (int cell = number - 2; cell >= 0; --cell)
		{
			for (int var = 0; var < number - 1 - cell; ++var)
			{
				operand1 = matrix[cell][cell + var];
				operand2 = matrix[cell + var + 1][number - 1];

				if (operand2.operation == 't')
				{
					tmpResult = operand1.min + operand2.min;
					tmpMax = (tmpResult > tmpMax) ? tmpResult : tmpMax;
					tmpMin = (tmpResult < tmpMin) ? tmpResult : tmpMin;

					tmpResult = operand1.max + operand2.max;
					tmpMax = (tmpResult > tmpMax) ? tmpResult : tmpMax;
					tmpMin = (tmpResult < tmpMin) ? tmpResult : tmpMin;

					tmpResult = operand1.max + operand2.min;
					tmpMax = (tmpResult > tmpMax) ? tmpResult : tmpMax;
					tmpMin = (tmpResult < tmpMin) ? tmpResult : tmpMin;

					tmpResult = operand1.min + operand2.max;
					tmpMax = (tmpResult > tmpMax) ? tmpResult : tmpMax;
					tmpMin = (tmpResult < tmpMin) ? tmpResult : tmpMin;
				}
				else
				{
					tmpResult = operand1.max * operand2.max;
					tmpMax = (tmpResult > tmpMax) ? tmpResult : tmpMax;
					tmpMin = (tmpResult < tmpMin) ? tmpResult : tmpMin;

					tmpResult = operand1.min * operand2.min;
					tmpMax = (tmpResult > tmpMax) ? tmpResult : tmpMax;
					tmpMin = (tmpResult < tmpMin) ? tmpResult : tmpMin;

					tmpResult = operand1.max * operand2.min;
					tmpMax = (tmpResult > tmpMax) ? tmpResult : tmpMax;
					tmpMin = (tmpResult < tmpMin) ? tmpResult : tmpMin;

					tmpResult = operand1.min * operand2.max;
					tmpMax = (tmpResult > tmpMax) ? tmpResult : tmpMax;
					tmpMin = (tmpResult < tmpMin) ? tmpResult : tmpMin;
				}
			}

			matrix[cell][number - 1].max = tmpMax;
			matrix[cell][number - 1].min = tmpMin;
			matrix[cell][number -1].operation = operand1.operation;

			tmpMax = LLONG_MIN;
			tmpMin = LLONG_MAX;
		}
		tmpResult = matrix[0][number - 1].max;
		maxFromBranch[i] = tmpResult;
		if (tmpResult > maximum)
		{
			maximum = tmpResult;
		}
	}

	for (int i = 0; i < number; ++i)
	{
		if (maxFromBranch[i] == maximum)
		{
			result[numOfBranches] = i + 1;
			numOfBranches++;
		}
	}

	delete[] maxFromBranch;
}

int main() 
{
	FILE *fin, *fout;
	int number = 0, numOfBranches = 0;
	Element *elementList;
	Step **matrix;
	int *result;
	long long maximum = LLONG_MIN;

	fin = fopen("input.txt", "r");
	fout = fopen("output.txt", "w");

	fscanf(fin, "%d", &number);

	elementList = new Element[number];
	result = new int[number];
	matrix = new Step*[number];
	for (int i = 0; i < number; ++i)
	{
		matrix[i] = new Step[number];
	}

	int i = 0;
	while (fscanf(fin, "%s%lld", &(elementList[i].operation), &(elementList[i].top)) != EOF)
	{
		++i;
	}
	
	findMaxBranches(elementList, matrix, number, maximum, numOfBranches, result);

	fprintf(fout, "%lld\n", maximum);

	for (int i = 0; i < numOfBranches; ++i)
	{
		fprintf(fout, "%lld", result[i]);
		if (i < numOfBranches - 1)
		{
			fprintf(fout, " ");
		}
	}

	fclose(fin);
	fclose(fout);
	delete[] elementList;
	delete[] result;
	for (int i = 0; i < number; ++i)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
	return 0;
}