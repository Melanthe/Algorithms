#pragma warning (disable: 4996)
#include <iostream>
#include <fstream>
#include <vector>
using namespace std; 

long n, m, k, t;
long numOfBlocks;
vector<long> setsSystem;

long getSetName(long boxNumber)
{
	long parent = setsSystem[boxNumber];
	if (parent >= 0)
	{
		boxNumber = getSetName(parent);
	}
	return boxNumber;
}

void mergeSets(long root1, long root2)
{
	if (setsSystem[root1] < setsSystem[root2])
	{
		setsSystem[root1] += setsSystem[root2];
		setsSystem[root2] = root1;
	}
	else
	{
		setsSystem[root2] += setsSystem[root1];
		setsSystem[root1] = root2;
	}
}

void copy(long* from, long* to)
{
	for (long i = 0; i < 3; ++i)
	{
		to[i] = from[i];
	}
}

long coordinatesToNumber(long* coordinates)
{
	return (coordinates[0] * m * k + coordinates[1] * k + coordinates[2]);
}

void step(long* from, long* to, long &mainIndex, FILE* fout)
{
	long currFrom[3], currTo[3];
	long root1, root2;

	copy(from, currFrom);
	copy(from, currTo);

	if (from[mainIndex] < to[mainIndex]) 
	{
		for (long i = from[mainIndex]; i < to[mainIndex]; ++i)
		{
			currFrom[mainIndex] = i;
			currTo[mainIndex] = i + 1;
			root1 = getSetName(coordinatesToNumber(currFrom));
			root2 = getSetName(coordinatesToNumber(currTo));
			if (root1 != root2)
			{
				mergeSets(root1, root2);
			}
			else
			{
				fprintf(fout, "%d %d %d ", currFrom[0] + 1, currFrom[1] + 1, currFrom[2] + 1);
				fprintf(fout, "%d %d %d\n", currTo[0] + 1, currTo[1] + 1, currTo[2] + 1);
			}
		}
	}
	else
	{
		for (long i = from[mainIndex]; i > to[mainIndex]; --i)
		{
			currFrom[mainIndex] = i;
			currTo[mainIndex] = i - 1;
			root1 = getSetName(coordinatesToNumber(currFrom));
			root2 = getSetName(coordinatesToNumber(currTo));
			if (root1 != root2)
			{
				mergeSets(root1, root2);
			}
			else
			{
				fprintf(fout, "%d %d %d ", currFrom[0] + 1, currFrom[1] + 1, currFrom[2] + 1);
				fprintf(fout, "%d %d %d\n", currTo[0] + 1, currTo[1] + 1, currTo[2] + 1);
			}
		}
	}
}

long main()
{
	FILE *fin = fopen("input.txt", "r");
	FILE *fout = fopen("output.txt", "w");

	fscanf(fin, "%d%d%d%d", &n, &m, &k, &t);
	numOfBlocks = n * m * k;

	setsSystem.reserve(numOfBlocks);
	setsSystem.assign(numOfBlocks, -1);
	
	long from[3], to[3];
	long mainIndex = 0;

	for (long i = 0; i < t; ++i)
	{
		fscanf(fin, "%d%d%d", &from[0], &from[1], &from[2]);
		fscanf(fin, "%d%d%d", &to[0], &to[1], &to[2]);

		if (from[0] != to[0]) { mainIndex = 0; }
		else if (from[1] != to[1]) { mainIndex = 1; }
		else { mainIndex = 2; }
		for (long j = 0; j < 3; ++j)
		{
			from[j]--;
			to[j]--;
		}
		step(from, to, mainIndex, fout);
	}

	fclose(fin);
	fclose(fout);
	return 0;
}