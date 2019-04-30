#include <iostream>
#include <fstream> 

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
	Node *left;
	Node *right;

	Node() : Node(0, nullptr, nullptr) {}
	Node(long key, Node *left, Node *right) : key(key), left(left), right(right) {}
};

class BinarySearchTree
{
	Node *root;

	void addNode(long key, Node *current)
	{
		if (key != current->key)
		{
			if (key > current->key)
			{
				if (current->right == nullptr)
				{
					current->right = new Node(key, nullptr, nullptr);
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
					current->left = new Node(key, nullptr, nullptr);
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

	void backwardRightTraversal(Node *current)
	{
		if (current->left != nullptr)
		{
			backwardRightTraversal(current->left);
		}
		if (current->right != nullptr)
		{
			backwardRightTraversal(current->right);
		}
		delete current;
	}

public:

	BinarySearchTree() : root(new Node()) {}
	BinarySearchTree(const BinarySearchTree &tree) : root(tree.root) {}
	~BinarySearchTree()
	{
		backwardRightTraversal(root);
	}

	friend std::ifstream & operator>>(std::ifstream &in, BinarySearchTree &tree)
	{
		long key;

		if (in >> key)
		{
			delete tree.root;
			tree.root = new Node(key, nullptr, nullptr);
		}
		else
			throw "Can't be readed!";

		while (in >> key)
		{
			tree.addNode(key, tree.root);
		}
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
	std::ifstream in("input.txt");
	std::ofstream out("output.txt");

	try
	{
		checkFile(in);
		Node* root;
		long key;

		BinarySearchTree tree;
		in >> tree;
		out << tree;

		out.close();
		in.close();
		system("pause");
		return 0;
	}
	catch (char *message)
	{
		std::cout << message << std::endl;
		system("pause");
	}
}