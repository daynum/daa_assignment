#include <line.cpp>
class Node
{
public:
	int height;
	vector<double> point;
	Node *left;
	Node *right;
	vector<Line> lines;

	Node()
	{
		this->lines = {};
		this->height = 0;
		this->left = nullptr;
		this->right = nullptr;
		this->point = vector<double>{};
	}

	Node(vector<double> point, Line l)
	{
		this->point = point;
		this->height = 1;
		this->left = nullptr;
		this->right = nullptr;
		if (l.isNull() == true)
		{
			this->lines = {};
		}
		else
		{
			this->lines = {l};
		}
	}

	bool is_null()
	{
		return ((this->lines == vector<Line>{}) && (this->height == 0));
	}
};

int compare(vector<double> p1, vector<double> p2)
{
	if (p1[1] != p2[1])
		return p1[1] - p2[1];

	return p2[0] - p1[0];
}

class EventQueue
{
public:
	Node root;
	EventQueue()
	{
		Node root = Node();
	}
};