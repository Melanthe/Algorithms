#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

void checkFile(std::ifstream &fin)
{
	if (!fin.is_open()) throw "File Input.txt can't be opened!";
	fin.seekg(0, std::ios::end);
	int pos = fin.tellg();
	if (pos == 0) throw "File is empty!";
	fin.seekg(0, std::ios::beg);
}

struct Node
{
	long key;
	int depth;
	int righHigh;
	int leftHigh;
	int numOfWays;
	Node *father;
	Node *left;
	Node *right;

	Node() : Node(0, 0, 0, 0, 0, nullptr, nullptr, nullptr) {}
	Node(long key, int depth, int righHigh, int leftHigh, int ways,
		Node * father, Node *left, Node *right)
			: key(key), depth(depth), righHigh(righHigh), leftHigh(leftHigh),
		numOfWays(ways), father(father), left(left), right(right) {}
};

class BinarySearchTree
{
	Node *root;
	int longestWay;
	int maxWay;
	std::map<int,std::vector<Node*>> levels;

	void addNode(long key, Node *current)
	{
		int depth = current->depth;
		depth++;

		if (key != current->key)
		{
			if (key > current->key)
			{
				if (current->right == nullptr)
				{
					current->right = new Node(key, depth, 0, 0, 0, current, nullptr, nullptr);
				}
				else
				{
					addNode(key, current->right);
				}
			}
			else
			{
				if (current->left == nullptr)
				{
					current->left = new Node(key, depth, 0, 0, 0, current, nullptr, nullptr);
				}
				else
				{
					addNode(key, current->left);
				}
			}
		}
	}

	void straightLeftTraversal(Node *current, std::ofstream &out)
	{
		out << current->key << '\n';
		if (current->left != nullptr)
		{
			straightLeftTraversal(current->left, out);
		}
		if (current->right != nullptr)
		{
			straightLeftTraversal(current->right, out);
		}
	}
	
	void deleteTree(Node *current)
	{
		if (current->left != nullptr)
		{
			deleteTree(current->left);
		}
		if (current->right != nullptr)
		{
			deleteTree(current->right);
		}
		delete current;
	}

	Node * findNode(Node *current, long key)
	{
		while (current != nullptr || current->key != key)
		{
			Node *result = nullptr;

			if (key > current->key)
			{
					current = current->right;
			}
			else if (key < current->key)
			{
					current = current->left;
			}
		}
		return current;
	}

	int findHigh(Node *root)
	{
		if (root == nullptr)
		{
			return 0;
		}
		return (1 + std::max(findHigh(root->left), findHigh(root->right)));
	}

	void fillLeavesList(Node *current)
	{
		if (current->left != nullptr)
		{
			fillLeavesList(current->left);
		}
		if (current->right != nullptr)
		{
			fillLeavesList(current->right);
		}
		if (current->right == nullptr && current->left == nullptr)
		{
			levels[current->depth].push_back(current);
		}
	}

	Node * findSmallest(Node *current)
	{
		Node *smallestNode = current;
		while (smallestNode->left != nullptr)
		{
			smallestNode = smallestNode->left;
		}
		return smallestNode;
	}

	void rightDeleting(Node *deleting)
	{
		Node *smallestNode = findSmallest(deleting->right);
		deleting->key = smallestNode->key;
		if (smallestNode->father == deleting)
		{
			smallestNode->father->right = smallestNode->right;
		}
		else
		{
			smallestNode->father->left = smallestNode->right;
		}
		if (smallestNode->right != nullptr)
		{
			smallestNode->right->father = smallestNode->father;
		}
		delete smallestNode;
	}

	void deleteRoot(Node *deleting)
	{
		if (deleting->right != nullptr)
		{
			root = deleting->right;
			deleting->right->father = nullptr;
		}
		else if (deleting->left != nullptr)
		{
			root = deleting->left;
			deleting->left->father = nullptr;
		}
		delete deleting;
	}

	void deleteWithNoTwoBranches(Node *deleting)
	{
		if (deleting->key > deleting->father->key)
		{
			if (deleting->right != nullptr)
			{
				deleting->father->right = deleting->right;
				deleting->right->father = deleting->father;
			}
			else if (deleting->left != nullptr)
			{
				deleting->father->right = deleting->left;
				deleting->left->father = deleting->father;
			}
			else
			{
				deleting->father->right = nullptr;
			}
		}
		else
		{
			if (deleting->right != nullptr)
			{
				deleting->father->left = deleting->right;
				deleting->right->father = deleting->father;
			}
			else if (deleting->left != nullptr)
			{
				deleting->father->left = deleting->left;
				deleting->left->father = deleting->father;
			}
			else
			{
				deleting->father->left = nullptr;
			}
		}
		delete deleting;
	}

	void makeWay(Node *leaf, Node *kink, std::vector<Node *> &way)
	{
		Node *current = leaf;

		while (current != kink)
		{
			way.push_back(current);
			current = current->father;
		}
	}

	void addWays(std::vector<Node *> &way, int num)
	{
		if (num == 0)
		{
			num = 1;
		}
		int size = way.size();
		for (int i = 0; i < size; ++i)
		{
			way[i]->numOfWays += num;
		}
	}

public:

	BinarySearchTree() : root(new Node()), longestWay(0) {}
	BinarySearchTree(const BinarySearchTree &tree) : root(tree.root), longestWay(tree.longestWay) {}
	~BinarySearchTree()
	{
		deleteTree(root);
	}

	void alghorithm()
	{
		maxWay = findMaxLength(root, 0);
		std::vector<Node *> kinks;
		findKinks(root, kinks);
		deleteTops(kinks);
	}

	int findMaxLength(Node *current, int max)
	{
		int right = findHigh(current->right);
		int left = findHigh(current->left);
		int tmp = right + left;
		current->righHigh = right;
		current->leftHigh = left;

		if (tmp > max)
		{
			max = tmp;
		}
		if (current->left != nullptr)
		{
			max = findMaxLength(current->left, max);
		}
		if (current->right != nullptr)
		{
			max = findMaxLength(current->right, max);
		}
		return max;
	}

	void findKinks(Node *current, std::vector<Node *> &kinks)
	{
		if ((current->righHigh + current->leftHigh) == maxWay)
		{
			handleKink(current);
			kinks.push_back(current);
		}
		if (current->left != nullptr)
		{
			findKinks(current->left, kinks);
		}
		if (current->right != nullptr)
		{
			findKinks(current->right, kinks);
		}
	}

	void handleKink(Node *kink)
	{
		int rightLow = kink->depth + kink->righHigh;
		int leftLow = kink->depth + kink->leftHigh;
		int size = 0;
		Node * tmpTop = nullptr;
		std::vector<Node *> tmpWay;
		std::vector<Node *> rightLeaves;
		std::vector<Node *> leftLeaves;

		size = levels[leftLow].size();
		for (int i = 0; i < size; ++i)
		{
			tmpTop = levels[leftLow][i];
			if ((kink->key <= root->key) && (tmpTop->key < root->key && tmpTop->key < kink->key))
			{
				leftLeaves.push_back(tmpTop);
			}
			else if (tmpTop->key >= root->key && tmpTop->key < kink->key)
			{
				leftLeaves.push_back(tmpTop);
			}
		}
		size = levels[rightLow].size();
		for (int i = 0; i < size; ++i)
		{
			tmpTop = levels[rightLow][i];
			if ((kink->key <= root->key) && (tmpTop->key < root->key && tmpTop->key > kink->key))
			{
				rightLeaves.push_back(tmpTop);
			}
			else if (tmpTop->key >= root->key && tmpTop->key > kink->key)
			{
				rightLeaves.push_back(tmpTop);
			}
		}

		int leftSize = leftLeaves.size();
		int rightSize = rightLeaves.size();
		int tmpCount = 0;
		for (int i = 0; i < leftSize; ++i)
		{
			makeWay(leftLeaves[i], kink, tmpWay);
			addWays(tmpWay, rightSize);
			if (rightSize == 0)
				tmpCount += 1;
			else
				tmpCount += rightSize;
			tmpWay.clear();
		}
		for (int i = 0; i < rightSize; ++i)
		{
			makeWay(rightLeaves[i], kink, tmpWay);
			addWays(tmpWay, leftSize);
			if (leftSize == 0)
				tmpCount += 1;
			else
				tmpCount += leftSize;
			tmpWay.clear();
		}
		if (leftSize && rightSize)
			kink->numOfWays += tmpCount / 2;
		else
			kink->numOfWays += tmpCount;
	}

	void rightDelete(Node *top)
	{
		Node *foundNode = top;

		if (foundNode != nullptr)
		{
			if (foundNode->right != nullptr && foundNode->left != nullptr)
			{
				rightDeleting(foundNode);
			}
			else if (foundNode == root)
			{
				deleteRoot(foundNode);
			}
			else
			{
				deleteWithNoTwoBranches(foundNode);
			}
		}
	}

	void deleteTops(std::vector<Node *> &kinks)
	{
		int maxNum = 0;
		int size = kinks.size();
		
		for (int i = 0; i < size; ++i)
		{
			if (kinks[i]->numOfWays > maxNum)
			{
				maxNum = kinks[i]->numOfWays;
			}
		}

		findDeletingTops(maxNum, root);
	}

	void findDeletingTops(int maxNum, Node *current)
	{
		if (current->left != nullptr)
		{
			findDeletingTops(maxNum, current->left);
		}
		if (current->right != nullptr)
		{
			findDeletingTops(maxNum, current->right);
		}
		if (current->numOfWays == maxNum)
		{
			rightDelete(current);
		}
	}

	friend std::ifstream & operator>>(std::ifstream &in, BinarySearchTree &tree)
	{
		long key = 0;

		if (in >> key)
		{
			delete tree.root;
			tree.root = new Node(key, 0, 0, 0, 0, nullptr, nullptr, nullptr);
		}
		else
			throw "Can't be readed!";

		while (in >> key)
		{
			tree.addNode(key, tree.root);
		}
		tree.fillLeavesList(tree.root);
		return in;
	}

	friend std::ofstream & operator<<(std::ofstream &out, BinarySearchTree &tree)
	{
		tree.straightLeftTraversal(tree.root, out);
		return out;
	}
};

int main()
{
	std::ifstream in("in.txt");
	std::ofstream out("out.txt");

	try
	{
		BinarySearchTree tree;
		in >> tree;
		tree.alghorithm();
		out << tree;

		in.close();
		out.close();
		system("pause");
		return 0;
	}
	catch (char *message)
	{
		std::cout << message << "\n";
		system("pause");
	}
}