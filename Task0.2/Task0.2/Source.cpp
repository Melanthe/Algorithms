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
	Node *father;
	Node *left;
	Node *right;

	Node() : Node(0, nullptr, nullptr, nullptr) {}
	Node(long key, Node * father, Node *left, Node *right) 
		: key(key), father(father), left(left), right(right) {}
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
					current->right = new Node(key, current, nullptr, nullptr);
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
					current->left = new Node(key, current, nullptr, nullptr);
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

	Node * findSmallest(Node *current)
	{
		Node *smallestNode = current;
		while (smallestNode->left != nullptr)
		{
			smallestNode = smallestNode->left;
		}
		return smallestNode;
	}

	Node * findNode(Node *current, long key)
	{
		if (key > current->key)
		{
			if (current->right != nullptr)
			{
				findNode(current->right, key);
			}
			else
			{
				return nullptr;
			}
		}
		else if (key < current->key)
		{
			if (current->left != nullptr)
			{
				findNode(current->left, key);
			}
			else
			{
				return nullptr;
			}
		}
		else if (key == current->key)
		{
			return current;
		}
		else
			return nullptr;
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
			if (smallestNode->right != nullptr)
			{
				smallestNode->right->father = smallestNode->father;
			}
			smallestNode->father->left = smallestNode->right;
		}
		delete smallestNode;
	}

	void deleteRoot(Node *deleting)
	{
		if (deleting->right != nullptr)
		{
			root = deleting->right;
		}
		else if (deleting->left != nullptr)
		{
			root = deleting->left;
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
			}
			else
			{
				deleting->father->right = deleting->left;
			}
		}
		else
		{
			if (deleting->right != nullptr)
			{
				deleting->father->left = deleting->right;
			}
			else
			{
				deleting->father->left = deleting->left;
			}
		}
		delete deleting;
	}

public:

	BinarySearchTree() : root(new Node()) {}
	BinarySearchTree(const BinarySearchTree &tree) : root(tree.root) {}
	~BinarySearchTree()
	{
		deleteTree(root);
	}

	friend std::ifstream & operator>>(std::ifstream &in, BinarySearchTree &tree)
	{
		long key = 0;

		if (in >> key)
		{
			delete tree.root;
			tree.root = new Node(key, nullptr, nullptr, nullptr);
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

	void rightDelete(long key)
	{
		Node *foundNode = nullptr;
		Node *smallestNode = nullptr;

		foundNode = findNode(root, key);
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
};

int main()
{
	std::ifstream in("input.txt");
	std::ofstream out("output.txt");

	try
	{
		checkFile(in);
		long deletedKey = 0;

		BinarySearchTree *tree = new BinarySearchTree();
		in >> deletedKey;
		in >> *tree;
		tree->rightDelete(deletedKey);
		out << *tree;

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