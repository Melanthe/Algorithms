#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

void checkFile(std::ifstream &fin)
{
	if (!fin.is_open()) throw "File Input.txt can't be opened!";
	fin.seekg(0, std::ios::end);
	long pos = fin.tellg();
	if (pos == 0) throw "File is empty!";
	fin.seekg(0, std::ios::beg);
}

struct Node
{
	long key;
	long depth;
	long high;
	long rootWays;
	long noRootWays;
	long leaves;
	Node *father;
	Node *left;
	Node *right;

	Node() : Node(0, 0, 0, 0, 0, 0, nullptr, nullptr, nullptr) {}
	Node(long key, long depth, long high, long rootWays, long noRootWays, long leaves,
		Node * father, Node *left, Node *right)
			: key(key), depth(depth), high(high), rootWays(rootWays), noRootWays(noRootWays),
		leaves(leaves), father(father), left(left), right(right) {}
};

class BinarySearchTree
{
	Node *root;

	void addNode(long key)
	{
		Node *parent = nullptr;
		Node *current = root;
		while (current)
		{
			parent = current;
			if (key < current->key)
			{
				current = current->left;
			}
			else if (key > current->key)
			{
				current = current->right;
			}
			else
			{
				return;
			}
		}
		if (key < parent->key)
		{
			parent->left = new Node(key, 0, 0, 0, 0, 0, parent, nullptr, nullptr);
		}
		else if (key > parent->key)
		{
			parent->right = new Node(key, 0, 0, 0, 0, 0, parent, nullptr, nullptr);
		}
	}

	void print(Node *current, std::ofstream &out)
	{
		out << current->key << '\n';
		if (current->left != nullptr)
		{
			print(current->left, out);
		}
		if (current->right != nullptr)
		{
			print(current->right, out);
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

public:

	BinarySearchTree() : root(new Node()){}
	BinarySearchTree(const BinarySearchTree &tree) : root(tree.root) {}
	~BinarySearchTree()
	{
		deleteTree(root);
	}

	void alghorithm()
	{
		long maxWay = 0;
		long maxNum = 0;
		long size = 0;
		std::vector<Node *> deleting;

		findMaxLength(root, maxWay);
		findNumOfWays(root, maxWay, maxNum, deleting);
		size = deleting.size();
		for (long i = 0; i < size; ++i)
		{
			if (deleting[i]->rootWays)
			{
				removeFound(deleting[i], maxNum);
			}
		}
	}

	void findMaxLength(Node *current, long &max)
	{
		if (current->left != nullptr)
		{
			findMaxLength(current->left, max);
		}
		if (current->right != nullptr)
		{
			findMaxLength(current->right, max);
		}
		setHighLeavesMax(current, max);
	}

	void setHighLeavesMax(Node *current, long &max)
	{
		long right = 0;
		long left = 0;

		if (!current->right && !current->left)
		{
			current->high = 0;
			current->leaves = 1;
		}
		else if (current->right && current->left)
		{
			right = current->right->high;
			left = current->left->high;
			current->high = std::max(right, left) + 1;

			if ((right + left + 2) > max)
			{
				max = (right + left + 2);
			}
			if (right == left)
			{
				current->leaves = current->right->leaves + current->left->leaves;
			}
			else
			{
				current->leaves = (right > left ? current->right->leaves : current->left->leaves);
			}
		}
		else
		{

			if (current->right)
			{
				current->high = current->right->high + 1;
				current->leaves = current->right->leaves;
			}
			else
			{
				current->high = current->left->high + 1;
				current->leaves = current->left->leaves;
			}
			if (current->high > max)
			{
				max = current->high;
			}
		}
	}

	void findNumOfWays(Node *current, long maxWay, long &maxNum, std::vector<Node *> &deleting)
	{
		long right = 0;
		long left = 0;

		if (current->right && current->left)
		{
			right = current->right->high;
			left =  current->left->high;
			if ((right + left + 2) == maxWay)
			{
				current->rootWays = current->right->leaves * current->left->leaves;
			}
			if (right == left)
			{
				current->right->noRootWays = current->rootWays + (current->right->leaves * (current->noRootWays / current->leaves));
				current->left->noRootWays = current->rootWays + (current->left->leaves * (current->noRootWays / current->leaves));
			}
			else if (right > left)
			{
				current->right->noRootWays = current->noRootWays + current->rootWays;
				current->left->noRootWays = current->rootWays;
			}
			else
			{
				current->left->noRootWays = current->noRootWays + current->rootWays;
				current->right->noRootWays = current->rootWays;
			}
		}
		else if (current->right)
		{
			right = current->right->high;
			if ((right + 1) == maxWay)
			{
				current->rootWays = current->right->leaves;
			}
			current->right->noRootWays = current->noRootWays + current->rootWays;
		}
		else if (current->left)
		{
			left = left = current->left->high;
			if ((left + 1) == maxWay)
			{
				current->rootWays = current->left->leaves;
			}
			current->left->noRootWays = current->noRootWays + current->rootWays;
		}
		long tmp = current->rootWays + current->noRootWays;
		if (tmp > maxNum)
		{
			maxNum = tmp;
			deleting.clear();
			deleting.push_back(current);
		}
		else if (tmp == maxNum)
		{
			deleting.push_back(current);
		}

		if (current->left != nullptr)
		{
			findNumOfWays(current->left, maxWay, maxNum, deleting);
		}
		if (current->right != nullptr)
		{
			findNumOfWays(current->right, maxWay, maxNum, deleting);
		}
	}

	void removeFound(Node *current, long &maxNum)
	{
		if (current->left != nullptr)
		{
			removeFound(current->left, maxNum);
		}
		if (current->right != nullptr)
		{
			removeFound(current->right, maxNum);
		}
		if (current->rootWays + current->noRootWays == maxNum)
		{
			rightDelete(current);
		}
	}

	void rightDelete(Node *top)
	{
		Node *foundNode = top;

		if (foundNode != nullptr)
		{
			if (foundNode->right && foundNode->left)
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

	friend std::ifstream & operator>>(std::ifstream &in, BinarySearchTree &tree)
	{
		long key = 0;

		if (in >> key)
		{
			delete tree.root;
			tree.root = new Node(key, 0, 0, 0, 0, 0, nullptr, nullptr, nullptr);
		}
		else
			throw "Can't be readed!";

		while (in >> key)
		{
			tree.addNode(key);
		}
		return in;
	}

	friend std::ofstream & operator<<(std::ofstream &out, BinarySearchTree &tree)
	{
		tree.print(tree.root, out);
		return out;
	}
};

long main()
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