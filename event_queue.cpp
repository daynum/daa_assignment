#include "status_structure.cpp"
// #include "line.cpp"
class Event_Node
{
public:
	int height;
	vector<double> point;
	Event_Node *left;
	Event_Node *right;
	vector<Line> lines;

	Event_Node()
	{
		this->lines = vector<Line>{};
		this->height = 0;
		this->left = nullptr;
		this->right = nullptr;
		this->point = vector<double>(2);
	}

	Event_Node(vector<double> point, Line l)
	{
		if (point.empty())
		{
			this->point = vector<double>(2);
		}
		else
		{
			this->point = vector<double>{point[0], point[1]};
		}

		this->height = 1;

		this->left = new Event_Node();
		this->right = new Event_Node();

		if (l.isNull())
		{
			this->lines = {};
		}
		else
		{
			this->lines = vector<Line>{l};
		}
	}
	bool is_null()
	{
		return (this->height == 0);
	}
};

class EventQueue
{
public:
	Event_Node *root = nullptr;
	EventQueue()
	{
		this->root = new Event_Node();
	}

	void insert_line(Line l)
	{
		vector<double> upper = {l.upper_end[0], l.upper_end[1]};
		vector<double> lower = {l.lower_end[0], l.lower_end[1]};
		Event_Node *t1 = this->inner_insert(this->root, upper, l);
		this->root = t1;
		Line *l2 = new Line();
		Event_Node *t2 = this->inner_insert(this->root, lower, *l2);
		delete l2;
		this->root = t2;
	}

	Event_Node *inner_insert(Event_Node *root, vector<double> point, Line l)
	{
		if ((*root).is_null())
		{
			Event_Node *x = new Event_Node(point, l);
			return x;
		}
		else if (compare_points(point, (*root).point) < 0)
		{
			Event_Node *x = this->inner_insert((*root).left, point, l);
			(*root).left = x;
		}
		else if (compare_points(point, (*root).point) > 0)
		{
			Event_Node *x = this->inner_insert((*root).right, point, l);
			(*root).right = x;
		}
		else
		{
			if (!l.isNull())
			{
				(*root).lines.push_back(l);
			}
			return root;
		}
		(*root).height = 1 + max(this->fetch_height((*root).left), this->fetch_height((*root).right));
		root = balance_insert(point, root);
		return root;
	}

	Event_Node *balance_insert(vector<double> point, Event_Node *root)
	{
		int bal = fetch_balance(root);

		if ((bal > 1) && (compare_points(point, (*root).point) < 0))
			return this->rotateRight(root);
		if ((bal < -1) && (compare_points(point, (*root).point) > 0))
			return this->rotateLeft(root);
		if ((bal > 1) && (compare_points(point, (*root).point) > 0))
		{
			(*root).left = this->rotateLeft((*root).left);
			return this->rotateRight(root);
		}
		if ((bal < -1) && (compare_points(point, (*root).point) < 0))
		{
			(*root).right = this->rotateRight((*root).right);
			return this->rotateLeft(root);
		}
		return root;
	}
	static double compare_points(vector<double> p1, vector<double> p2)
	{
		if (p1[1] != p2[1])
			return p1[1] - p2[1];

		return p2[0] - p1[0];
	}

	void insert_point(vector<double> point)
	{
		Line *l = new Line();
		this->root = this->inner_insert(this->root, point, *l);
	}

	Event_Node *rotateLeft(Event_Node *node)
	{
		Event_Node *x = (*node).right;
		Event_Node *temp = (*x).left;
		(*x).left = node;
		(*node).right = temp;
		if (this->fetch_height((*node).left) > this->fetch_height((*node).right))
		{
			(*node).height = this->fetch_height((*node).left) + 1;
		}
		else
		{
			(*node).height = this->fetch_height((*node).right) + 1;
		}

		if (this->fetch_height((*x).left) > this->fetch_height((*x).right))
		{
			(*x).height = this->fetch_height((*x).left) + 1;
		}
		else
		{
			(*x).height = this->fetch_height((*x).right) + 1;
		}

		return x;
	}
	Event_Node *rotateRight(Event_Node *node)
	{
		Event_Node *x = (*node).left;
		Event_Node *temp = (*x).right;
		(*x).right = node;
		(*node).left = temp;
		if (this->fetch_height((*node).left) > this->fetch_height((*node).right))
		{
			(*node).height = this->fetch_height((*node).left) + 1;
		}
		else
		{
			(*node).height = this->fetch_height((*node).right) + 1;
		}

		if (this->fetch_height((*x).left) > this->fetch_height((*x).right))
		{
			(*x).height = this->fetch_height((*x).left) + 1;
		}
		else
		{
			(*x).height = this->fetch_height((*x).right) + 1;
		}

		return x;
	}

	void inorder(Event_Node *root, vector<Event_Node> &result)
	{
		if (!root || (*root).is_null())
		{
			return;
		}
		else
		{
			this->inorder((*root).left, result);
			result.push_back(*root);
			cout << "\n[point: " << (*root).point[0] << " " << (*root).point[1] << "], \n";

			for (Line l : (*root).lines)
			{
				cout << "[" << l.name << "], ";
			}

			this->inorder((*root).right, result);
		}
	}

	vector<Event_Node> self_inorder()
	{
		vector<Event_Node> result{};
		cout << "Inorder Start:\n";
		this->inorder(this->root, result);
		cout << "\nInorder end:" << endl;
		return result;
	}

	void delete_point(vector<double> point)
	{
		this->root = this->delete_node(this->root, point);
	}

	Event_Node *delete_node(Event_Node *root, vector<double> point)
	{
		if (!root || (*root).is_null())
		{
			return root;
		}
		else if (compare_points(point, (*root).point) < 0)
		{
			(*root).left = this->delete_node((*root).left, point);
		}
		else if (compare_points(point, (*root).point) > 0)
		{
			(*root).right = this->delete_node((*root).right, point);
		}
		else
		{
			if ((!(*root).left) || ((*root).left)->is_null())
			{
				Event_Node *temp = new Event_Node();
				temp = (*root).right;
				root = new Event_Node();
				delete root;
				return temp;
			}
			else if ((!(*root).right) || (*root).right->is_null())
			{
				Event_Node *temp = new Event_Node();
				temp = (*root).left;
				root = new Event_Node();
				delete root;
				return temp;
			}
			Event_Node *temp2 = this->fetch_min_val_node((*root).right);
			(*root).point = (*temp2).point;
			(*root).lines = (*temp2).lines;
			(*root).right = this->delete_node((*root).right, (*temp2).point);
		}

		if (!root || (*root).is_null())
			return root;

		(*root).height = 1 + max(this->fetch_height((*root).left), this->fetch_height((*root).right));
		root = balance_delete(root);
		return root;
	}
	Event_Node *balance_delete(Event_Node *root)
	{
		int bal = fetch_balance(root);

		if ((bal > 1) && (fetch_balance((*root).left) >= 0))
			return this->rotateRight(root);
		if ((bal < -1) && (fetch_balance((*root).right) <= 0))
			return this->rotateLeft(root);
		if ((bal > 1) && (fetch_balance((*root).left) < 0))
		{
			(*root).left = this->rotateLeft((*root).left);
			return this->rotateRight(root);
		}
		if ((bal < -1) && (fetch_balance((*root).right) > 0))
		{
			(*root).right = this->rotateRight((*root).right);
			return this->rotateLeft(root);
		}
		return root;
	}
	Event_Node fetch_max()
	{
		Event_Node *curr = this->root;
		while ((!(*curr).right->is_null()))
		{
			curr = (*curr).right;
		}
		return *curr;
	}

	Event_Node pop_next_event()
	{
		Event_Node curr = this->fetch_max();
		this->delete_point(curr.point);
		return curr;
	}

	Event_Node *fetch_min_val_node(Event_Node *root)
	{
		if (!root || (*root).is_null() || !((*root).left) || ((*root).left)->is_null())
			return root;

		return this->fetch_min_val_node((*root).left);
	}

	int fetch_height(Event_Node *root)
	{
		if (!root || (*root).is_null())
			return 0;
		else
			return (*root).height;
	}
	int fetch_balance(Event_Node *root)
	{
		if ((!(*root).left) || (*root).left->is_null())
		{
			if ((!(*root).right) || (*root).right->is_null())
			{
				return 0;
			}
			else
			{
				return 0 - (*root).right->height;
			}
		}
		else if ((!(*root).right) || (*root).right->is_null())
		{
			return (*root).left->height;
		}
		else
		{
			return (*root).left->height - (*root).right->height;
		}
	}

	bool is_null()
	{
		return (this->root)->is_null();
	}
};

// int main()
// {
// 	vector<Line> lines = {};
// 	vector<pair<double, double>> x = {
// 		{0, 0},
// 		{1, 2},
// 		{2, 3},
// 		{3, 2},
// 		{2, 1},
// 		{2, 1},
// 		{1, 2},
// 		{1, 1},
// 		{1.5, 1},
// 		{0, 0}};
// 	vector<pair<double, double>> y = {
// 		{1, 2},
// 		{2, 3},
// 		{3, 2},
// 		{2, 1},
// 		{0, 0},
// 		{2, 3},
// 		{4, 2},
// 		{1, 1.5},
// 		{1, 1},
// 		{3, 3}};
// 	double xc[2] = {};
// 	double yc[2] = {};
// 	EventQueue Q = EventQueue();
// 	for (int i = 0; i < (int)x.size(); i++)
// 	{
// 		xc[0] = x[i].first;
// 		xc[1] = x[i].second;
// 		yc[0] = y[i].first;
// 		yc[1] = y[i].second;
// 		Q.insert_line(Line(xc, yc));
// 		// Q.root->lines[i].name = (char)(i + (int)'0');
// 	}
// 	Q.self_inorder();
// 	return 0;
// }