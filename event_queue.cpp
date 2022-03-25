#include <line.cpp>
class Node
{
public:
	int height;
	vector<double> point;
	Node *left;
	Node *right;
	vector<Line> lines;

	Node(int a)
	{
		this->lines = {};
		this->height = 0;
		this->left = nullptr;
		this->right = nullptr;
		this->point = vector<double>{};
	}
	Node()
	{
		this->lines = {};
		this->height = 0;
		*this->left = Node(0);
		*this->right = Node(0);
		this->point = vector<double>{};
	}

	Node(vector<double> point, Line l)
	{
		this->point = point;
		this->height = 1;
		*this->left = Node();
		*this->right = Node();
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

	Node inner_insert(Node root, vector<double> point, Line l = Line())
	{
		if (root.is_null())
		{
			return Node(point, l);
		}
		else if (compare(point, root.point) < 0)
		{

			*root.left = this->inner_insert(*root.left, point, l);
		}
		else if (compare(point, root.point) > 0)
		{

			*root.right = this->inner_insert(*root.right, point, l);
		}
		else
		{
			if (!l.isNull())
			{
				root.lines.push_back(l);
			}
			return root;
		}

		root.height = 1 + max(this->fetch_height(*root.left), this->fetch_height(*root.right));
		int balance = fetch_balance(root);

		if ((balance > 1) && (compare(point, root.point) < 0))
			return this->rotateRight(root);
		if ((balance < -1) && (compare(point, root.point) > 0))
			return this->rotateLeft(root);
		if ((balance > 1) && (compare(point, root.point) > 0))
		{
			*root.left = this->rotateLeft(*root.left);
			return this->rotateRight(root);
		}
		if ((balance < -1) && (compare(point, root.point) < 0))
		{
			*root.right = this->rotateRight(*root.right);
			return this->rotateLeft(root);
		}
		return root;
	}
	Node insert_line(Line l)
	{
		vector<double> upper = l.upper_end;
		vector<double> lower = l.lower_end;

		this->root = this->inner_insert(this->root, upper, l);
		this->root = this->inner_insert(this->root, lower, Line());
	}

	Node insert_point(vector<double> point)
	{
		this->root = this->inner_insert(this->root, point, Line());
	}

	Node rotateLeft(Node node)
	{
		Node x = *node.right;
		Node temp = *x.left;
		*x.left = node;
		*node.right = temp;

		if (this->fetch_height(*node.left) > this->fetch_height(*node.right))
		{
			node.height = this->fetch_height(*node.left) + 1;
		}
		else
		{
			node.height = this->fetch_height(*node.right) + 1;
		}

		if (this->fetch_height(*x.left) > this->fetch_height(*x.right))
		{
			x.height = this->fetch_height(*x.left) + 1;
		}
		else
		{
			x.height = this->fetch_height(*x.right) + 1;
		}

		return x;
	}
	Node rotateRight(Node node)
	{
		Node x = *node.left;
		Node temp = *x.right;
		*x.right = node;
		*node.left = temp;
		if (this->fetch_height(*node.left) > this->fetch_height(*node.right))
		{
			node.height = this->fetch_height(*node.left) + 1;
		}
		else
		{
			node.height = this->fetch_height(*node.right) + 1;
		}

		if (this->fetch_height(*x.left) > this->fetch_height(*x.right))
		{
			x.height = this->fetch_height(*x.left) + 1;
		}
		else
		{
			x.height = this->fetch_height(*x.right) + 1;
		}

		return x;
	}

	void inorder(Node root, vector<Node> &result)
	{
		if (root.is_null())
		{
			return;
		}
		else
		{
			this->inorder(*root.left, result);
			result.push_back(root);
			this->inorder(*root.right, result);
		}
	}

	vector<Node> self_inorder()
	{
		vector<Node> result{};
		this->inorder(this->root, result);
		return result;
	}

	void delete_point(vector<double> point)
	{
		this->root = this->delete_node(this->root, point);
	}

	Node delete_node(Node root, vector<double> point)
	{
		if (root.is_null())
		{
			return root;
		}
		else if (compare(point, root.point) < 0)
		{
			*root.left = this->delete_node(*root.left, point);
		}
		else if (compare(point, root.point) > 0)
		{
			*root.right = this->delete_node(*root.right, point);
		}
		else
		{
			if ((*root.left).is_null())
			{
				Node temp = *root.right;
				root = Node();
				return temp;
			}
			else if ((*root.right).is_null())
			{
				Node temp = *root.left;
				root = Node();
				return temp;
			}

			Node temp = this->fetch_min_val_node(*root.right);
			root.point = temp.point;
			root.lines = temp.lines;
			*root.right = this->delete_node(*root.right, temp.point);
		}

		if (root.is_null())
			return root;

		root.height = 1 + max(this->fetch_height(*root.left), this->fetch_height(*root.right));
		int balance = this->fetch_balance(root);

		if ((balance > 1) && (this->fetch_balance(*root.left) >= 0))
			return this->rotateRight(root);
		if ((balance < -1) && (this->fetch_balance(*root.right) <= 0))
			return this->rotateLeft(root);
		if ((balance > 1) && (this->fetch_balance(*root.left) < 0))
		{
			*root.left = this->rotateLeft(*root.left);
			return this->rotateRight(root);
		}
		if ((balance < -1) && (this->fetch_balance(*root.right) > 0))
		{
			*root.right = this->rotateRight(*root.right);
			return this->rotateLeft(root);
		}

		return root;
	}
	Node fetch_max()
	{
		Node curr = this->root;
		while (!(*curr.right).is_null())
		{
			curr = *curr.right;
		}
		return curr;
	}

	Node pop_next_event()
	{
		Node curr = this->fetch_max();
		this->delete_point(curr.point);
		return curr;
	}

	Node fetch_min_val_node(Node root)
	{
		if (root.is_null() || (*root.left).is_null())
			return root;

		return this->fetch_min_val_node(*root.left);
	}

	int fetch_height(Node root)
	{
		return root.height;
	}
	int fetch_balance(Node root)
	{
		return (*root.left).height - (*root.right).height;
	}

	bool is_null()
	{
		return this->root.is_null();
	}
};