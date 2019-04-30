#pragma warning(disable : 4996)
#include <iostream>
#include <fstream>
#include <vector>

int main()
{
	FILE *fin, *fout;

	fin = fopen("input.txt", "r");
	fout = fopen("output.txt", "w");

	long long num;
	fscanf(fin, "%lld", &num);

	std::vector<int> result;
	while (num != 0)
	{
		result.push_back(num % 2);
		num /= 2;
	}

	int size = result.size();
	for (int i = 0; i < size; ++i)
	{
		if (result[i])
		{
			fprintf(fout, "%d\n", i);
		}
	}
	return 0;
}