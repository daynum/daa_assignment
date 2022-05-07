/// \headerfile line.cpp
/// \brief including line cpp file.
#include "line.cpp"

/// \namespace std
/// \brief using standard namespace.
using namespace std;

/// \class Status_Node status_structure.cpp
/// \brief Status_Node class to create and manage nodes for status queue.
/// This class defines a node, with a line, left and right pointers to status nodes, and height of node in b-tree.
class Status_Node
{
public:
	/// \brief Line line stores a line in the node.
    Line line;
	/// \brief Status_Node *right stores the pointer to left status node.
    Status_Node *left;
	/// \brief Status_Node *right stores the pointer to right status node.
    Status_Node *right;
	/// \brief int height stores the height of node in binary tree.
    int height;
	
	/// \brief Empty Constructor to create a null status node.
    Status_Node()
    {
        Line *temp = new Line();
        this->line = *temp;
        this->height = 0;
        this->left = nullptr;
        this->right = nullptr;
    }
	/// \brief Constructor to create a status node with line l.
    Status_Node(Line l)
    {
        this->line = l;
        this->height = 1;
        this->left = new Status_Node();
        this->right = new Status_Node();
    }
	/// \brief member function to copy a status node into another status node.
	/// \param [in] Status_Node x node to be copied from.
    void copy(Status_Node x)
    {
        this->line = x.line;
        this->height = x.height;
        this->left = x.left;
        this->right = x.right;
    }
    /// \brief member function to check whether the status node is null.
    /// \returns boolean value null or not
    /// \retval true status node is null.
    /// \retval false status node is not null.
    bool is_null()
    {
        return (this->height == 0);
    }
};
/// \class StatusStructure status_structure.cpp
/// \brief StatusStructure class to maintain a self balancing binary search tree of status nodes in a self balanced binary search tree.
class StatusStructure
{
public:
	/// \brief Status_Node *root root of the b-tree initialized to null pointer.
    Status_Node *root = nullptr;
	
	/// \brief Empty status structure constructor creates a null status node, and assings it to the root.
    StatusStructure()
    {
        this->root = new Status_Node();
    }
	/// \brief member function to insert lines into the status structure.
	/// It takes the lines on eby one and inserts them.\n
    /// \param [in] vector<Line> &lines The lines to be inserted.
	/// \param [in] vector<double> point point at where to insert.
	/// \param [out] root is set to new root after insertion.
    void insert_lines(vector<double> point, vector<Line> &lines)
    {
        for (Line l : lines)
        {
            this->root = this->insert(this->root, point, l);
        }
    }
	/// \brief member function to insert a line into the status structure.
	/// It takes a line and point, compares them to select left or right insertion, at what node.
	/// Then it balances the tree, and finally returns the new node after insertion and balancing.
    /// \param [in] Status_Node *root root of the b-tree.
	/// \param [in] vector<double> point at where to insert.
	/// \param [in] Line l the line to be inserted.
	/// \returns Status_Node *root is set to new root after insertion and balancing.
    Status_Node *insert(Status_Node *root, vector<double> point, Line l)
    {
        if (!root || (*root).is_null())
        {
            Status_Node *x = new Status_Node(l);
            return x;
        }
        else if ((*root).line.compare_lower_angle(point[0], point[1], l) > 0)
        {
            Status_Node *x = this->insert((*root).left, point, l);
            (*root).left = x;
        }
        else
        {
            Status_Node *x = this->insert((*root).right, point, l);
            (*root).right = x;
        }

        (*root).height = 1 + max(this->fetch_height((*root).left), this->fetch_height((*root).right));
        root = balance_insert(point, root, l);
        return root;
    }
    /// \brief member function to balance the b-tree after an insertion.
	/// It takes the root, line and point.\n
	/// Balances the binary tree by comparing the balance of node, and relative location of point with line.\n
	/// Rotates rightward or leftward as needed.
	/// Finally returns the root pointer to the new balanced binary search tree.
    /// \param [in] Status_Node *root initial root of the balanced binary tree.
	/// \param [in] vector<double> point point to be inserted at.
	/// \param [in] Line l line to be inserted.
	/// \returns Status_Node *root the root of the balanced binary tree after balancing.
    Status_Node *balance_insert(vector<double> point, Status_Node *root, Line l)
    {
        int bal = fetch_balance(root);
        if ((bal > 1) && ((*root).line.compare_lower_angle(point[0], point[1], l) > 0))
            return this->rotateRight(root);
        if ((bal < -1) && ((*root).line.compare_lower_angle(point[0], point[1], l) < 0))
            return this->rotateLeft(root);
        if ((bal > 1) && ((*root).line.compare_lower_angle(point[0], point[1], l) < 0))
        {
            (*root).left = this->rotateLeft((*root).left);
            return this->rotateRight(root);
        }
        if ((bal < -1) && ((*root).line.compare_lower_angle(point[0], point[1], l) > 0))
        {
            (*root).right = this->rotateRight((*root).right);
            return this->rotateLeft(root);
        }
        return root;
    }
	/// \brief member function to rotate a node leftwards.
	/// \param [in] Status_Node *node node to rotate.
	/// \returns Status_Node *pointer to the node after rotation.
    Status_Node *rotateLeft(Status_Node *node)
    {
        Status_Node *x = (*node).right;
        Status_Node *temp = (*x).left;
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
	/// \param [in] Status_Node *node node to rotate.
	/// \returns Status_Node *pointer to the node after rotation.
    Status_Node *rotateRight(Status_Node *node)
    {

        Status_Node *x = (*node).left;
        Status_Node *temp = (*x).right;
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
	/// \param [in] Status_Node *root root of the binary tree/subtree.
	/// \param [out] vector<Status_Node> &result address of the result vector, where the result would be stored.
    void inorder(Status_Node *root, vector<Status_Node> &result)
    {
        if (!root || (*root).is_null())
        {
            return;
        }
        else
        {
            this->inorder((*root).left, result);
            result.push_back(*root);
            if ((*root).line.name != "empty_null_line")
                cout << "[line: " << (*root).line.name << "], ";
            this->inorder((*root).right, result);
        }
    }
	/// \brief member function for inorder traversal of the whole binary tree.
	/// not used in the actual code, just a tool to see the tree structure and debug.
	/// \returns vector<Status_Node> result the result vector, where the result would be stored.
    vector<Status_Node> self_inorder()
    {
        cout << "\n";
        vector<Status_Node> result{};
        this->inorder(this->root, result);
        cout << "\n";
        return result;
    }
	/// \brief member function to delete a line from the tree.
	/// \param [in] vector<double> point point.
	/// \param [in] Line l the line to be deleted.
	/// \param [out] Status_Node *root sets the root of b-tree to new root after deletion.
    void delete_line(vector<double> point, Line l)
    {
        this->root = this->delete_node(this->root, point, l);
    }
	/// \brief member function to delete multiple lines from the tree.
	/// \param [in] vector<double> point point.
	/// \param [in] vector<Line> lines the lines to be deleted.
    void delete_lines(vector<double> point, vector<Line> lines)
    {
        for (Line l : lines)
        {
            this->delete_line(point, l);
        }
    }
	
	/// \brief member function to delete a node from the b-tree.
	/// \param [in] Status_Node *root root of b-tree.
	/// \param [in] vector<double> point point.
	/// \param [in] Line l line to be deleted.
	/// \returns Status_Node *root pointer to the new b-tree root after node deletion and balancing.
    Status_Node *delete_node(Status_Node *root, vector<double> point, Line l)
    {
        if (!root || (*root).is_null())
        {
            return root;
        }
        else if ((*root).line.compare_upper_angle(point[0], point[1], l) > 0)
        {
            (*root).left = this->delete_node((*root).left, point, l);
        }
        else if ((*root).line.compare_upper_angle(point[0], point[1], l) < 0)
        {
            (*root).right = this->delete_node((*root).right, point, l);
        }
        else
        {
            if ((!(*root).left) || ((*root).left)->is_null())
            {
                Status_Node *temp = (*root).right;
                delete root;
                return temp;
            }
            else if ((!(*root).right) || (*root).right->is_null())
            {
                Status_Node *temp = (*root).left;
                delete root;
                return temp;
            }
            Status_Node *temp2 = this->fetch_min_val_node((*root).right);
            (*root).line = (*temp2).line;
            (*root).right = this->delete_node((*root).right, point, (*temp2).line);
        }
        if (!root || (*root).is_null())
            return root;

        (*root).height = 1 + max(this->fetch_height((*root).left), this->fetch_height((*root).right));
        root = balance_deletion(root);
        return root;
    }
	/// \brief member function to balance the b-tree after node deletion.
	/// \param [in] Status_Node *root root of b-tree.
	/// \returns Status_Node *root pointer to the new b-tree root after balancing.
    Status_Node *balance_deletion(Status_Node *root)
    {
        int bal = fetch_balance(root);
        if ((bal > 1) && ((fetch_balance((*root).left)) >= 0))
            return this->rotateRight(root);
        if ((bal < -1) && ((fetch_balance((*root).right)) <= 0))
            return this->rotateLeft(root);
        if ((bal > 1) && ((fetch_balance((*root).left)) < 0))
        {
            (*root).left = this->rotateLeft((*root).left);
            return this->rotateRight(root);
        }
        if ((bal < -1) && ((fetch_balance((*root).right)) > 0))
        {
            (*root).right = this->rotateRight((*root).right);
            return this->rotateLeft(root);
        }
        return root;
    }
	/// \brief member function to get the minimum node to another node (minimum in subtree).
	/// \param [in] Status_Node *root root of the tree.
	/// \returns Status_Node *node pointer to event node which is minimum.
    Status_Node *fetch_min_val_node(Status_Node *root)
    {
        if (!root || (*root).is_null() || !((*root).left) || ((*root).left)->is_null())
            return root;

        return this->fetch_min_val_node((*root).left);
    }
	/// \brief member function to get the height of a node in tree.
	/// \param [in] Status_Node *root node for which we need height.
	/// \returns int height height of the node.
    int fetch_height(Status_Node *root)
    {
        if (!root || (*root).is_null())
            return 0;
        else
            return (*root).height;
    }
	
	/// \brief member function to get the balance of a node in tree.
	/// \param [in] Status_Node *root node for which we need baance value.
	/// \returns int balance balance of the node.
    int fetch_balance(Status_Node *root)
    {
        if (!root || (*root).is_null())
        {
            return 0;
        }
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
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    /// \brief member function to get the lines containing a point.
	/// \param [in] vector<double> point point for which we need lines.
	/// \returns vector<vector<Line>> &result vector of lines passing through point.
	vector<vector<Line>> get_segments_containing_point(vector<double> point)
    {
        vector<vector<Line>> result(3, vector<Line>{});
        this->inner_get_segments(this->root, point, result);
        return result;
    }
	
	/// \brief member function to get the lines containing a point under a root.
	/// \param [in] Status_Node *root node for which we need lines.
	/// \param [in] vector<double> point point for which we need lines.
	/// \param [out] vector<vector<Line>> &result vector containing passing lines.
    void inner_get_segments(Status_Node *root, vector<double> point, vector<vector<Line>> &result)
    {

        if (!root || (*root).is_null())
        {
            return;
        }
        else if ((*root).line.pointWRTLine(point[0], point[1]) > 0)
        {
            this->inner_get_segments((*root).right, point, result);
        }
        else if ((*root).line.pointWRTLine(point[0], point[1]) < 0)
        {
            this->inner_get_segments((*root).left, point, result);
        }
        else
        {
            if ((!(*root).left->is_null()) || (!(*root).left))
            {
                this->inner_get_segments((*root).left, point, result);
            }
            if ((*root).line.lower_end == point)
            {
                result[0].push_back((*root).line);
            }
            else
            {
                result[1].push_back((*root).line);
            }
            result[2].push_back((*root).line);

            if ((!(*root).right->is_null()) || (!(*root).right))
            {
                this->inner_get_segments((*root).right, point, result);
            }
        }
    }
	/// \brief member function to get the left neighbour line of a point.
	/// \param [in] vector<double> point point around which to search.
	/// \returns Line l the adjacent left line to the point.
    Line get_adjacent_left_line(vector<double> point)
    {
        Status_Node *left = new Status_Node();
        this->inner_find_left(this->root, point, left);
        return (*left).line;
    }
	/// \brief member function to get the left neighbour line of a point under a node.
	/// \param [in] vector<double> point point around which to search.
	/// \param [in] Status_Node *root root under which to search.
	/// \param [out] Status_Node *left pointer to store the left neighbour line.
    void inner_find_left(Status_Node *root, vector< double > point, Status_Node *left)
    {
        if (!root || (*root).is_null() /* || (*root).line.isNull()*/)
            return;
        else if ((*root).line.pointWRTLine(point[0], point[1]) > 0)
        {
            (*left).copy((*root));
            this->inner_find_left((*root).right, point, left);
        }
        else if ((*root).line.pointWRTLine(point[0], point[1]) <= 0)
        {
            this->inner_find_left((*root).left, point, left);
        }
    }
	/// \brief member function to get the right neighbour line of a point.
	/// \param [in] vector<double> point point around which to search.
	/// \returns Line l the adjacent right line to the point.
    Line get_adjacent_right_line(vector< double > point)
    {
        Status_Node *right = new Status_Node();
        this->inner_find_right(this->root, point, right);
        return (*right).line;
    }
	/// \brief member function to get the right neighbour line of a point under a node.
	/// \param [in] vector<double> point point around which to search.
	/// \param [in] Status_Node *root root under which to search.
	/// \param [out] Status_Node *right pointer to store the right neighbour line.
    void inner_find_right(Status_Node *root, vector<double> point, Status_Node *right)
    {
        if (!root || (*root).is_null() /* || (*root).line.isNull()*/)
            return;
        else if ((*root).line.pointWRTLine(point[0], point[1]) >= 0)
        {
            this->inner_find_right((*root).right, point, right);
        }
        else if ((*root).line.pointWRTLine(point[0], point[1]) < 0)
        {
            (*right).copy((*root));
            this->inner_find_right((*root).left, point, right);
        }
    }
	/// \brief member function to get the leftmost line of a point.
	/// \param [in] vector<double> point point around which to search.
	/// \returns Line l the leftmost line to the point.
    Line get_leftmost(vector<double> point)
    {
        Status_Node *leftmost = new Status_Node();
        this->inner_leftmost(this->root, point, leftmost);
        return (*leftmost).line;
    }
	/// \brief member function to get the leftmost line of a point under a node.
	/// \param [in] vector<double> point point around which to search.
	/// \param [in] Status_Node *root root under which to search.
	/// \param [out] Status_Node *leftmost pointer to store the leftmost line.
    void inner_leftmost(Status_Node *root, vector<double> point, Status_Node *leftmost)
    {
        if (!root || (*root).is_null())
            return;
        else if ((*root).line.pointWRTLine(point[0], point[1]) > 0)
        {
            this->inner_leftmost((*root).right, point, leftmost);
        }
        else if ((*root).line.pointWRTLine(point[0], point[1]) < 0)
        {
            this->inner_leftmost((*root).left, point, leftmost);
        }
        else
        {
            this->inner_leftmost((*root).right, point, leftmost);
            (*leftmost).copy((*root));
            this->inner_leftmost((*root).left, point, leftmost);
        }
        return;
    }
	/// \brief member function to get the rightmost line of a point.
	/// \param [in] vector<double> point point around which to search.
	/// \returns Line l the rightmost line to the point.
    Line get_rightmost(vector<double> point)
    {
        Status_Node *rightmost = new Status_Node();
        this->inner_rightmost(this->root, point, rightmost);
        return (*rightmost).line;
    }
	/// \brief member function to get the rightmost line of a point under a node.
	/// \param [in] vector<double> point point around which to search.
	/// \param [in] Status_Node *root root under which to search.
	/// \param [out] Status_Node *rightmost pointer to store the rightmost line.
    void inner_rightmost(Status_Node *root, vector<double> point, Status_Node *rightmost)
    {
        if (!root || (*root).is_null())
            return;
        else if ((*root).line.pointWRTLine(point[0], point[1]) > 0)
        {
            this->inner_rightmost((*root).right, point, rightmost);
        }
        else if ((*root).line.pointWRTLine(point[0], point[1]) < 0)
        {
            this->inner_rightmost((*root).left, point, rightmost);
        }
        else
        {
            this->inner_rightmost((*root).left, point, rightmost);
            (*rightmost).copy((*root));
            this->inner_rightmost((*root).right, point, rightmost);
        }
    }
};