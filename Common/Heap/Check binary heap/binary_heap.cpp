#pragma warning(disable : 4996)
#include <iostream>
#include <fstream>
#include <vector>

int main()
{
	FILE *fin, *fout;

	fin = fopen("input.txt", "r");
	fout = fopen("output.txt", "w");

	int num;
	fscanf(fin, "%d", &num);

	std::vector<int> mass(num);
	for (int i = 0; i < num; ++i)
	{
		fscanf(fin, "%d", &mass[i]);
	}

	bool flag = true;
	int start = num  / 2;
	int parent, child_1, child_2;

	for (int i = start; i > 0; --i)
	{
		parent = mass[i - 1];
		child_1 = mass[2 * i - 1];
		child_2 = (2 * i < num) ? mass[2 * i] : child_1;

		if (child_1 < parent || child_2 < parent)
		{
			flag = false;
			break;
		}
	}

	if (!flag)
	{
		fprintf(fout, "%s", "No");
	}
	else
	{
		fprintf(fout, "%s", "Yes");
	}

	return 0;
}