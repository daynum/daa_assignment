#include <line.cpp>
class Node
{
public:
	int height;
	vector<double> point;
	Node *left;
	Node *right;
	vector<Line> lines;

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
};
struct Event
{
	point p;
	int segment;
	int position;
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
	EventQueue()
	{
		//		Node head=nullptr;
	}
	EventQueue(Event e)
	{
		Event m = e;
		Node head;
		head.left = nullptr;
		head.right = nullptr;
		head.height = 1;
	}

	Node insert(Node node, point p, Line line)
	{
		if (node.head != nullptr)
			return node.head;

		else if (compare(p, node.q) < 0)
		{
			node.left = insert(node.left, p, line);
		}
		else if (compare(p, node.q) > 0)

		{
			node.right = insert(node.right, p, line);
		}

		else if (l != nullptr)
		{
			node.line.lower_end = line.lower_end;
			node.line.upper_end = line.upper_end;
		}
	}
};