#pragma warning(disable : 4996)
#include <iostream>
#include <fstream>
#include <algorithm>

int main()
{
	FILE *fin, *fout;
	int number, first, second, position = 0;
	int *lilies;
	long *result;

	fin = fopen("input.txt", "r");
	fout = fopen("output.txt", "w");

	fscanf(fin, "%d", &number);
	lilies = new int[number];
	result = new long[number + 1];

	for (int i = 0; i < number; ++i)
	{
		fscanf(fin, "%d", &lilies[i]);
	}

	if (number == 1)
	{
		fprintf(fout, "%d", lilies[0]);
		fclose(fin);
		fclose(fout);
		return 0;
	}
	else if (number == 2)
	{
		fprintf(fout, "%d", -1);
		fclose(fin);
		fclose(fout);
		return 0;
	}

	result[0] = 0;
	result[1] = lilies[0];
	result[2] = 0;
	for (int i = 3; i < number + 1; ++i)
	{
		result[i] = std::max(result[i - 2], result[i - 3]) + lilies[i - 1];
	}

	fprintf(fout, "%ld", result[number]);

	fclose(fin);
	fclose(fout);
	return 0;
}