#pragma warning(disable : 4996)
#include <iostream>
#include <fstream>
#include <vector>

struct InnerWay {

	int weight;
	int start;
	int finish;

	std::vector<int> innerTops;

	InnerWay(int size = 0) {
		this->innerTops.reserve(size);
	}
};

struct Way {

	std::vector<int> way;
	int weight;
};

int ribs[500][500];
int adjacency[500][3];
int mark[500];
std::vector<int> outerLoop;
int resultWay[500];
int resultWeight = 1001;
int numOfGrots, numOfTop, numOfInner;

void findInnerWay(const int &start, const int &finish, std::vector<int> &resultWay)
{
	int stack[500];
	int currentSize = 1;
	int tmpTop, currentTop;
	bool flag = false;

	stack[0] = start;
	mark[start] = 1;
	while (currentSize != 0)
	{
		flag = false;
		currentTop = stack[currentSize - 1];
		currentSize--;

		if (currentTop == finish)
		{
			break;
		}

		for (int i = 0; i < 3; ++i)
		{
			tmpTop = adjacency[currentTop][i];
			if (tmpTop >= numOfTop && !mark[tmpTop])
			{
				flag = true;
				stack[currentSize] = tmpTop;
				currentSize++;
				mark[tmpTop] = 1;
				if (tmpTop == finish) {
					break;
				}
			}
		}
		if (flag)
		{
			resultWay.push_back(currentTop);
		}
	}
	resultWay.push_back(finish);

	tmpTop = resultWay.size() - 1;
	for (int i = tmpTop; i > 0; --i)
	{
		if (!ribs[resultWay[i]][resultWay[i - 1]])
		{
			resultWay.erase(resultWay.begin() + i - 1);
		}
	}
	for (int i = numOfTop; i < numOfGrots; ++i)
	{
		mark[i] = 0;
	}
}

void findWeight(std::vector<int> &way, int &resultWeight, int &start, int &finish)
{
	int size = way.size() - 1;
	int weight = 0;

	for (int i = 0; i < size; ++i)
	{
		weight += ribs[way[i]][way[i + 1]];
	}
	weight += ribs[start][way[0]] + ribs[way[size]][finish];
	resultWeight = weight;
}

void fillOuterLoop(std::vector<int> &outerLoop)
{
	int tmpTop;
	outerLoop[0] = 0;
	outerLoop[1] = adjacency[0][0];
	for (int i = 2; i < numOfTop; ++i)
	{
		tmpTop = outerLoop[i - 1];
		for (int j = 0; j < 2; ++j)
		{
			if (adjacency[tmpTop][j] != outerLoop[i - 2])
			{
				outerLoop[i] = adjacency[tmpTop][j];
			}
		}
	}
}

bool isDifference(std::vector<int> &tops, std::vector<int> &status)
{
	bool flag = true;
	int tmpSize = tops.size();

	for (int i = 0; i < tmpSize; ++i)
	{
		if (status[tops[i]] == 1)
		{
			flag = false;
			break;
		}
	}

	return flag;
}

void buildWay(std::vector<int> &sequence, std::vector<InnerWay> &innerWays)
{
	int tmpSize = sequence.size();
	std::vector<int> currentWay;
	currentWay.reserve(numOfGrots);
	int currentWeight = 0;
	for (int i = 0; i < tmpSize; ++i)
	{
		mark[sequence[i]] = 1;
	}
	
	int currentIndex = 0;
	int start, finish, tmpTop;
	InnerWay tmpWay;
	for (int i = 0; i < numOfTop; ++i)
	{
		if (mark[i])
		{
			tmpWay = innerWays[i];
			start = tmpWay.start;
			finish = tmpWay.finish;
			while (outerLoop[currentIndex] != start)
			{
				tmpTop = outerLoop[currentIndex];
				currentWeight += ribs[tmpTop][outerLoop[currentIndex + 1]];
				currentWay.push_back(tmpTop);
				currentIndex++;
			}
			currentIndex++;

			currentWeight += tmpWay.weight;

			currentWay.push_back(start);
			currentWay.insert(currentWay.end(), tmpWay.innerTops.begin(), tmpWay.innerTops.end());
		}
	}
	while (outerLoop[currentIndex] != 0)
	{
		tmpTop = outerLoop[currentIndex];
		currentWay.push_back(tmpTop);
		currentWeight += ribs[tmpTop][outerLoop[currentIndex + 1]];
		currentIndex++;
	}
	
	if (currentWeight < resultWeight)
	{
		for (int i = 0; i < numOfGrots; ++i)
		{
			resultWay[i] = currentWay[i];
		}
		resultWeight = currentWeight;
	}

	for (int i = 0; i < numOfTop; ++i)
	{
		mark[i] = 0;
	}
}

void findSequence(int parent, std::vector<int> status, std::vector<int> sequenceOfWays, int line, std::vector<InnerWay> &innerWays, std::vector<std::vector<int>> &include)
{
	int tmpSize = 0;
	int tmpTop = 0;
	int flag = false;

	if (parent != -1)
	{
		tmpTop = parent;
		sequenceOfWays.push_back(tmpTop);
		tmpSize = innerWays[tmpTop].innerTops.size();
		for (int j = 0; j < tmpSize; ++j)
		{
			status[innerWays[tmpTop].innerTops[j]] = 1;
		}
		for (int j = 0; j < 3; ++j)
		{
			status[include[line - 1][j]] = 1;
		}
	}

	if (line == numOfInner)
	{
		buildWay(sequenceOfWays, innerWays);
		return;
	}
	if (status[line + numOfTop])
	{
		findSequence(-1, status, sequenceOfWays, line + 1, innerWays, include);
	}
	else
	{
		for (int i = 0; i < 3; ++i)
		{
			flag = false;
			tmpTop = include[line][i];

			if (status[tmpTop] == 1)
			{
				flag = true;
			}

			if ((!flag) && isDifference(innerWays[tmpTop].innerTops, status))
			{
				findSequence(tmpTop, status, sequenceOfWays, line + 1, innerWays, include);
			}
		}
	}
}

int main()
{
	FILE *fin, *fout;

	fin = fopen("input.in", "r");
	fout = fopen("output.out", "w");

	fscanf(fin, "%d%d", &numOfGrots, &numOfTop);
	numOfInner = numOfGrots - numOfTop;

	int size = 3 * numOfGrots / 2;
	int first, second, level;
	for (int i = 0; i < size; ++i)
	{
		fscanf(fin, "%d%d%d", &first, &second, &level);
		first--;
		second--;
		ribs[first][second] = level + 1;
		ribs[second][first] = level + 1;
	}

	int count = 0;
	for (int i = 0; i < numOfGrots; ++i)
	{
		count = 0;
		for (int j = 0; j < numOfGrots; ++j)
		{
			if (ribs[i][j])
			{
				adjacency[i][count++] = j;
			}
		}
	}

	outerLoop.reserve(numOfTop + 1);
	for (int i = 0; i <= numOfTop; ++i)
	{
		outerLoop.push_back(0);
	}
	fillOuterLoop(outerLoop);

	int start, finish;
	std::vector<InnerWay> innerWays(numOfTop);
	for (int i = 0; i < numOfTop; ++i)
	{
		start = outerLoop[i];
		finish = outerLoop[i + 1];
		innerWays[i].start = start;
		innerWays[i].finish = finish;
		findInnerWay(adjacency[start][2], adjacency[finish][2], innerWays[i].innerTops);
		findWeight(innerWays[i].innerTops, innerWays[i].weight, start, finish);
	}

	std::vector<std::vector<int>> include(numOfInner);
	
	int tmpSize;
	for (int i = 0; i < numOfTop; ++i)
	{
		tmpSize = innerWays[i].innerTops.size();
		for (int j = 0; j < tmpSize; ++j)
		{
			include[innerWays[i].innerTops[j] - numOfTop].push_back(i);
		}
	}

	std::vector<int> sequenceOfWays;
	sequenceOfWays.reserve(numOfTop);
	std::vector<int> status(numOfGrots, 0);
	int line = 0;

	findSequence(-1, status, sequenceOfWays, line, innerWays, include);

	for (int i = 0; i < numOfGrots; ++i)
	{
		if (i < numOfGrots - 1)
		{
			fprintf(fout, "%d ", resultWay[i] + 1);
		}
		else
		{
			fprintf(fout, "%d", resultWay[i] + 1);
		}
	}

	fclose(fin);
	fclose(fout);
	return 0;
}