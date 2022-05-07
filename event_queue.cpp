/// \headerfile status_structure.cpp
/// \brief including status structure cpp file.
#include "status_structure.cpp"

/// \namespace std
/// \brief using standard namespace.
using namespace std;

/// \class Event_Node event_queue.cpp
/// \brief Event_Node class to create and manage nodes for event queue.
/// This class defines a node with a point, left and right pointers to event nodes, height of the node in b-tree, and a vector of lines.
class Event_Node
{
public:
	/// \brief int height stores the height of node in binary tree.
	int height;
	/// \brief vector<double> point stores the point of node.
	vector<double> point;
	/// \brief Event_Node *left stores the pointer to left event node.
	Event_Node *left;
	/// \brief Event_Node *right stores the pointer to right event node.
	Event_Node *right;
	/// \brief vector<Line> lines stores the vector of lines in the node.
	vector<Line> lines;
	/// \brief Empty Constructor to create a null event node.
	Event_Node()
	{
		this->lines = vector<Line>{};
		this->height = 0;
		this->left = nullptr;
		this->right = nullptr;
		this->point = vector<double>(2);
	}
	/// \brief Constructor to create a new event node. With a point and line given.
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
	/// \brief member function to check whether the event node is null.
	/// \returns boolean value null or not
	/// \retval true event node is null.
	/// \retval false event node is not null.
	bool is_null()
	{
		return (this->height == 0);
	}
};
/// \class EventQueue event_queue.cpp
/// \brief EventQueue class to maintain a self balancing binary search tree of event nodes in a self balanced binary search tree.
class EventQueue
{
public:
	/// \brief Event_Node *root root of the b-tree initialized to null pointer.
	Event_Node *root = nullptr;
	/// \brief Empty event queue constructor creates a null event node, and assings it to the root.
	EventQueue()
	{
		this->root = new Event_Node();
	}
	/// \brief member function to insert a line 'l' into the event queue.
	/// It takes the upper and lower endpoints of the line.\nInserts an event node with line l with upper endpoint, and inserts a n event node with null line at lower endpoint.\nFinally returns a pointer to the root of binary tree.
	/// \param [in] Line l The line to insert.
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
	/// \brief member function to insert a lin 'l' into the event queue.
	/// It takes the root, point, and line to insert\n
	/// Inserts an event node with line l at point given.\n
	/// Then it balances the binary tree.
	/// Finally returns a pointer to the root of new balanced binary tree.
	/// \param [in] Event_Node *root initial root of the balanced binary tree.
	/// \param [in] vector<double> point the point at which to insert the line.
	/// \param [in] Line l The line to insert.
	/// \returns Event_Node *root the root of the balanced binary tree after insertion.
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
	/// \brief member function to balance the binary search tree after an insertion.
	/// It takes the root and point.\n
	/// Balances the binary tree by comparing the balance of node, and relative points location.\n
	/// Rotates rightward or leftward as needed.
	/// Finally returns the root pointer to the new balanced binary search tree.
	/// \param [in] Event_Node *root initial root of the balanced binary tree.
	/// \param [in] vector<double> point.
	/// \returns Event_Node *root the root of the balanced binary tree after balancing.
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
	/// \brief member function to compare two points and return relative location.
	/// \param [in] vector<double> p1 point 1.
	/// \param [in] vector<double> p2 point 2.
	/// \returns difference between the y-coordinates ( or else x-coordinates) of the two points.
	static double compare_points(vector<double> p1, vector<double> p2)
	{
		if (p1[1] != p2[1])
			return p1[1] - p2[1];

		return p2[0] - p1[0];
	}
	/// \brief member function to insert a point in the event queue.
	/// Inserts a point with null line into the event queue.
	/// \param [in] vector<double> point.
	/// \returns pointer to the root of new binary tree after insertion.
	void insert_point(vector<double> point)
	{
		Line *l = new Line();
		this->root = this->inner_insert(this->root, point, *l);
	}
	/// \brief member function to rotate a node leftwards.
	/// \param [in] Event_Node *node node to rotate.
	/// \returns Event_Node *pointer to the node after rotation.
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
	/// \brief member function to rotate a node rightwards.
	/// \param [in] Event_Node *node node to rotate.
	/// \returns Event_Node *pointer to the node after rotation.
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
	/// \brief member function for inorder traversal of a node.
	/// not used in the actual code, just a tool to see the tree structure and debug.
	/// \param [in] Event_Node *root root of the binary tree/subtree.
	/// \param [out] vector<Event_Node> &result address of the result vector, where the result would be stored.
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
	/// \brief member function for inorder traversal of the whole binary tree.
	/// not used in the actual code, just a tool to see the tree structure and debug.
	/// \returns vector<Event_Node> result the result vector, where the result would be stored.
	vector<Event_Node> self_inorder()
	{
		vector<Event_Node> result{};
		cout << "Inorder Start:\n";
		this->inorder(this->root, result);
		cout << "\nInorder end:" << endl;
		return result;
	}
	/// \brief member function to delete a point from the tree.
	/// \param [in] vector<double> point the point to be deleted.
	/// \param [out] Event_Node *root sets the root of b-tree to new root after deletion.
	void delete_point(vector<double> point)
	{
		this->root = this->delete_node(this->root, point);
	}
	/// \brief member function to delete a node from the b-tree.
	/// \param [in] Event_Node *root root of b-tree.
	/// \param [in] vector<double> point of the node to be deleted.
	/// \returns Event_Node *root pointer to the new b-tree root after node deletion.
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

	/// \brief member function to balance the b-tree after node deletion.
	/// \param [in] Event_Node *root root of b-tree.
	/// \returns Event_Node *root pointer to the new b-tree root after balancing.
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
	/// \brief member function to get the maximum node in the queue.
	/// \returns Event_Node curr event node which is maximum.
	Event_Node fetch_max()
	{
		Event_Node *curr = this->root;
		while ((!(*curr).right->is_null()))
		{
			curr = (*curr).right;
		}
		return *curr;
	}
	/// \brief member function to get the next event in the queue, and pop it from the queue.
	/// \returns Event_Node curr next event node.
	Event_Node pop_next_event()
	{
		Event_Node curr = this->fetch_max();
		this->delete_point(curr.point);
		return curr;
	}
	/// \brief member function to get the minimum node to another node (minimum in subtree).
	/// \param [in] Event_Node *root root of the tree.
	/// \returns Event_Node *node pointer to event node which is minimum.
	Event_Node *fetch_min_val_node(Event_Node *root)
	{
		if (!root || (*root).is_null() || !((*root).left) || ((*root).left)->is_null())
			return root;

		return this->fetch_min_val_node((*root).left);
	}
	/// \brief member function to get the height of a node in tree.
	/// \param [in] Event_Node *root node for which we need height.
	/// \returns int height height of the node.
	int fetch_height(Event_Node *root)
	{
		if (!root || (*root).is_null())
			return 0;
		else
			return (*root).height;
	}
	/// \brief member function to get the balance of a node in tree.
	/// \param [in] Event_Node *root node for which we need baance value.
	/// \returns int balance balance of the node.
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
	/// \brief member function to check whether the eent queue is null (empty) or not.
	/// \returns bool empty or not
	/// \retval true the event queue is empty.
	/// \retval false the event queue is not empty.
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