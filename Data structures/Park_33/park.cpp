#pragma warning(disable: 4996)
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
using namespace std;

struct Dot {

	long long x;
	long long y;

	Dot(long long x = 0, long long y = 0) : x(x), y(y) {}
};

void mergeY(long long left, long long mid, long long right, vector<Dot>& dots)
{
	vector<Dot> mass(dots);

	long long index_1 = left, index_2 = mid + 1, curr = left;
	while (index_1 < mid + 1 && index_2 <= right)
	{
		if (mass[index_1].y < mass[index_2].y)
		{
			dots[curr] = mass[index_1];
			index_1++;
		}
		else if (mass[index_2].y <= mass[index_1].y)
		{
			dots[curr] = mass[index_2];
			index_2++;
		}
		curr++;
	}
	for (long long i = index_1; i < mid + 1; ++i)
	{
		dots[curr] = mass[i];
		curr++;
	}
	for (long long i = index_2; i <= right; ++i)
	{
		dots[curr] = mass[i];
		curr++;
	}
}

vector<Dot> mergeSortY(long long left, long long right, vector<Dot> dots)
{
	long long mid = 0;

	if (right != left)
	{
		mid = (left + right) / 2;

		dots = mergeSortY(left, mid, dots);
		dots = mergeSortY(mid + 1, right, dots);
		mergeY(left, mid, right, dots);
	}
	return dots;
}

vector<long long> getOriginalY(vector<Dot>& dots, long long &numOfDots, long long& sizeY)
{
	vector<long long> ordinates(numOfDots + 2);
	long long numOfY = numOfDots + 2, indexY = 0;
	ordinates[0] = 0;

	for (int i = 0; i < numOfDots; ++i)
	{
		if (dots[i].y == ordinates[indexY])
		{
			numOfY--;
			continue;
		}
		ordinates[++indexY] = dots[i].y;
	}
	ordinates[numOfY - 1] = sizeY;
	ordinates.resize(numOfY);
	return ordinates;
}

vector<long long> getOriginalX(vector<Dot>& dots, long long& numOfDots, long long& sizeX)
{
	vector<long long> abscissa(numOfDots);
	long long numOfX = numOfDots, indexX = 0;

	for (int i = 0; i < numOfDots; ++i)
	{
		if (dots[i].x == abscissa[indexX])
		{
			numOfX--;
			continue;
		}
		abscissa[++indexX] = dots[i].x;
	}
	abscissa.resize(numOfX);
	return abscissa;
}

void fillAdjacencyList(vector<vector<long long>>& adjacencyList, long long &numOfDots, vector<Dot> &dots, long long &firstY)
{
	int k = 0;
	long long currY = firstY;

	for (int i = 0; i < numOfDots; ++i)
	{
		if (dots[i].y != currY)
		{
			currY = dots[i].y;
			k++;
		}
		adjacencyList[k].push_back(dots[i].x);
	}
}

int main()
{
	FILE* fin = fopen("input.txt", "r");
	FILE* fout = fopen("output.txt", "w");

	long long num, numOfDots, sizeX, sizeY;
	long long maxArea = 0;

	fscanf(fin, "%lld%lld%lld", &num, &sizeX, &sizeY);
	vector<Dot> dots(num);
	numOfDots = num;

	long long x, y, k = 0;
	for (long long i = 0; i < num && numOfDots != 0; ++i)			//Выкидываем граничные точки, т.к. они не на что не влияют
	{
		fscanf(fin, "%lld%lld", &x, &y);
		if (!x || !y || x == sizeX || y == sizeY)
		{
			numOfDots--;
			continue;
		}
		dots[k] = Dot(x, y);
		k++;
	}

	if (numOfDots == 0)
	{
		maxArea = sizeX * sizeY;
		fprintf(fout, "%lld", maxArea);
		fclose(fin);
		fclose(fout);
		return 0;
	}

	dots.resize(numOfDots);
	dots = mergeSortY(0, numOfDots - 1, dots);

	vector<long long> ordinates(getOriginalY(dots, numOfDots, sizeY));
	vector<long long> abscissa(getOriginalX(dots, numOfDots, sizeX));
	long long numOfX = abscissa.size(), numOfY = ordinates.size();

	vector<vector<long long>> adjacencyList(numOfY - 2);
	fillAdjacencyList(adjacencyList, numOfDots, dots, ordinates[1]);

	vector<Dot> zeroYDots(numOfX);
	for (int i = 0; i < numOfX; ++i)
	{
		zeroYDots[i] = Dot(abscissa[i], 0);
	}

	long long floor = 0, ceiling = 0;
	long long tmpArea = 0, currIndex = 0, tmpSize = 0, numOfZeroDots = numOfX;
	long long leftX, rightX;
	bool status = false;
	Dot currDot, left, right;
	ceiling = ordinates[0];
	k = 0;

	for (int i = 0; i < numOfY - 1; ++i)
	{
		floor = ordinates[i];
		while (true)
		{
			leftX = 0;
			rightX = sizeX;
			status = false;

			if (floor == 0)
			{
				if (k == numOfZeroDots) { break; }
				currDot = zeroYDots[k];
				k++;
			}
			else if (floor == ordinates[numOfY - 2])
			{
				tmpArea = (sizeY - floor) * sizeX;
				maxArea = (tmpArea > maxArea) ? tmpArea : maxArea;
				break;
			}
			else
			{
				currDot = dots[currIndex];
				if (currDot.y == floor) { currIndex++; }
				else { break; }
			}

			for (int j = i + 1; j < numOfY; ++j)
			{
				ceiling = ordinates[j];

				if (i == 0 && j == 1) {
					tmpArea = sizeX * ceiling;
					maxArea = (tmpArea > maxArea) ? tmpArea : maxArea;
					continue;
				}

				if (j != i + 1) {
					tmpSize = adjacencyList[j - 2].size();
					for (int q = 0; q < tmpSize; ++q)
					{
						if (adjacencyList[j - 2][q] < currDot.x)
						{
							leftX = (adjacencyList[j - 2][q] > leftX) ? adjacencyList[j - 2][q] : leftX;
						}
						else if (adjacencyList[j - 2][q] > currDot.x)
						{
							rightX = (adjacencyList[j - 2][q] < rightX) ? adjacencyList[j - 2][q] : rightX;
						}
						else { status = true; }
					}
				}

				if (status)
				{
					tmpArea = (ceiling - floor) * (currDot.x - leftX);
					maxArea = (tmpArea > maxArea) ? tmpArea : maxArea;
					tmpArea = (ceiling - floor) * (rightX - currDot.x);
					maxArea = (tmpArea > maxArea) ? tmpArea : maxArea;
				}
				else
				{
					tmpArea = (ceiling - floor) * (rightX - leftX);
					maxArea = (tmpArea > maxArea) ? tmpArea : maxArea;
				}
			}
		}
	}
	fprintf(fout, "%lld", maxArea);
	fclose(fin);
	fclose(fout);
	return 0;
}