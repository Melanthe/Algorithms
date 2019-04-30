#pragma warning (disable: 4996)
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main()
{
	FILE *fin = fopen("input.txt", "r");
	FILE *fout = fopen("output.txt", "w");

	long number;
	fscanf(fin, "%ld", &number);

	vector<long> tree(number, 0);
	long v;
	for (int i = 0; i < number; ++i)
	{
		for (int j = 0; j < number; ++j)
		{
			fscanf(fin, "%ld", &v);
			if (v)
			{
				tree[j] = i + 1;
			}
		}
	}

	fclose(fin);

	for (int i = 0; i < number; ++i)
	{
		if (i == number - 1)
		{
			fprintf(fout, "%ld", tree[i]);
		}
		else
		{
			fprintf(fout, "%ld ", tree[i]);
		}
	}
	return 0;
}