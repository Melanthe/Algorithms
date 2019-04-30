#pragma warning(disable : 4996)
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int ribs[500][500];

int main()
{
	FILE *fin, *fout;
	int numOfGrots, numOfTop, minWeight;
	unsigned long long int numOfMasks;

	fin = fopen("input.in", "r");
	fout = fopen("output.out", "w");

	fscanf(fin, "%d%d", &numOfGrots, &numOfTop);
	numOfMasks = 1 << numOfGrots;
	std::vector<std::vector<int>> weight(numOfMasks, std::vector<int>(numOfGrots, 1000));
	std::vector<int> resultWay;
	weight[0][0] = 0;

	int size = 3 * numOfGrots / 2;
	int first, second, level;
	for (int i = 0; i < size; ++i)
	{
		fscanf(fin, "%d%d%d", &first, &second, &level);
		ribs[first - 1][second - 1] = level + 1;
		ribs[second - 1][first - 1] = level + 1;		
	}

	long long int tmp, last, current;
	for (int mask = 0; mask < numOfMasks; ++mask)
	{
		for (int top = 0; top < numOfGrots; ++top)
		{
			if (weight[mask][top] == 1000)
			{
				continue;
			}
			for (int way = 0; way < numOfGrots; ++way)
			{
				
				tmp = 1 << way;
				if (!(mask & tmp) && ribs[top][way])
				{
					last = weight[mask ^ tmp][way];
					current = weight[mask][top] + ribs[top][way];
					weight[mask ^ tmp][way] = (last < current) ? last : current;
				}
			}
		}
	}


	int currentTop = 0;
	long long mask = numOfMasks - 1;
	resultWay.push_back(1);

	while (mask)
	{
		for (int i = 0; i < numOfGrots; ++i)
		{
			tmp = 1 << currentTop;
			if (ribs[currentTop][i] && (mask & tmp) && (weight[mask][currentTop] == (weight[mask ^ tmp][i] + ribs[currentTop][i])))
			{
				resultWay.push_back(i + 1);
				currentTop = i;
				mask = mask - tmp;
			}
		}
	}

	for (int i = 0; i < numOfGrots; ++i)
	{
		if (i == numOfGrots - 1)
		{
			fprintf(fout, "%d", resultWay[i]);
		}
		else
		{
			fprintf(fout, "%d ", resultWay[i]);
		}
	}

	fclose(fin);
	fclose(fout);
	return 0;
}