/*a node will have lines passing through it, left node, right node, and it's height*/
#include "line.cpp"
#include <iostream>
using namespace std;
class Status_Node
{
public:
    Line line;
    Status_Node *left;
    Status_Node *right;
    int height;
    Status_Node()
    {

        this->line = Line();
        this->height = 0;
        this->left = nullptr;
        this->right = nullptr;
    }
    Status_Node(Line l)
    {
        this->line = l;
        this->height = 1;
        this->left = nullptr;
        this->right = nullptr;
    }

    void copy(Status_Node x)
    {
        this->line = x.line;
        this->height = x.height;
        this->left = x.left;
        this->right = x.right;
    }

    bool is_null()
    {
        return (this->height == 0);
    }
};

class StatusStructure
{
public:
    Status_Node *root = nullptr;
    StatusStructure()
    {
        this->root = new Status_Node();
    }

    Status_Node *insert(Status_Node *root, vector<double> point, Line l)
    {
        cout << "line: " << l.upper_end[0] << " " << l.upper_end[1];
        cout << ", " << l.lower_end[0] << " " << l.lower_end[1] << endl;
        if (!root || (*root).is_null())
        {
            Status_Node *x = new Status_Node(l);
            return x;
        }
        else if ((*root).line.compare_lower_angle(point[0], point[1], l) > 0)
        {
            if (!(*root).left || (*root).left->is_null())
            {
                Status_Node x = Status_Node(l);
                (*root).left = &x;
            }
            else
            {
                Status_Node *x = new Status_Node();
                x = this->insert((*root).left, point, l);
                (*root).left = x;
            }
        }
        else
        {
            if ((!(*root).right) || (*root).right->is_null())
            {
                Status_Node *x = new Status_Node();
                (*root).right = x;
            }
            else
            {
                Status_Node *x = new Status_Node();
                x = this->insert((*root).right, point, l);
                (*root).right = x;
            }
        }

        if ((!(*root).left) || (*root).left->is_null())
        {
            if ((!(*root).right) || (*root).right->is_null())
            {
                (*root).height = 1;
            }
            else
            {
                (*root).height = 1 + this->fetch_height((*root).right);
            }
        }
        else if ((!(*root).right) || (*root).right->is_null())
        {
            (*root).height = 1 + this->fetch_height((*root).left);
        }
        else
        {
            (*root).height = 1 + max(this->fetch_height((*root).right), this->fetch_height((*root).right));
        }
        int balance = fetch_balance(root);
        if ((balance > 1) && ((*root).line.compare_lower_angle(point[0], point[1], l) > 0))
            return this->rotateRight(root);
        if ((balance < -1) && ((*root).line.compare_lower_angle(point[0], point[1], l) < 0))
            return this->rotateLeft(root);
        if ((balance > 1) && ((*root).line.compare_lower_angle(point[0], point[1], l) < 0))
        {
            (*root).left = this->rotateLeft((*root).left);
            return this->rotateRight(root);
        }
        if ((balance < -1) && ((*root).line.compare_lower_angle(point[0], point[1], l) > 0))
        {
            (*root).right = this->rotateRight((*root).right);
            return this->rotateLeft(root);
        }
        return root;
    }
    void insert_lines(vector<double> point, vector<Line> &lines)
    {
        for (Line l : lines)
        {
            Status_Node *x = this->insert(this->root, point, l);
            this->root = x;
        }
    }
    // implement print_name
    Status_Node *rotateLeft(Status_Node *node)
    {
        Status_Node *x = new Status_Node();
        x = (*node).right;
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
    Status_Node *rotateRight(Status_Node *node)
    {

        Status_Node *x = new Status_Node();
        x = (*node).left;
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

    void inorder(Status_Node *root, vector<Status_Node> &result)
    {
        if (root == nullptr || (*root).is_null())
        {
            cout << "nullptr ";
            return;
        }
        else
        {

            this->inorder((*root).left, result);
            result.push_back(*root);
            this->inorder((*root).right, result);
        }
    }

    vector<Status_Node> self_inorder()
    {
        cout << "Inorder:\n";
        vector<Status_Node> result{};
        this->inorder(this->root, result);
        return result;
    }

    void delete_line(vector<double> point, Line l)
    {
        this->root = this->delete_node(this->root, point, l);
    }
    void delete_lines(vector<double> point, vector<Line> lines)
    {
        for (Line l : lines)
        {
            this->delete_line(point, l);
        }
    }
    Status_Node *delete_node(Status_Node *root, vector<double> point, Line l)
    {
        if (!root || (*root).is_null())
        {
            return root;
        }
        else if ((*root).line.compare_upper_angle(point[0], point[1], l) > 0)
        {
            Status_Node *x = this->delete_node((*root).left, point, l);
            (*root).left = x;
        }
        else if ((*root).line.compare_upper_angle(point[0], point[1], l) < 0)
        {
            Status_Node *x = this->delete_node((*root).right, point, l);
            (*root).right = x;
        }
        else
        {
            if ((!(*root).left) || ((*root).left)->is_null())
            {
                Status_Node *temp = new Status_Node();
                temp = (*root).right;
                *root = Status_Node();
                return temp;
            }
            else if ((!(*root).right) || (*root).right->is_null())
            {
                Status_Node *temp = new Status_Node();
                temp = (*root).left;
                *root = Status_Node();
                return temp;
            }
            Status_Node temp = *this->fetch_min_val_node((*root).right);
            (*root).line = temp.line;
            Status_Node *x = this->delete_node((*root).right, point, temp.line);
            (*root).right = x;
        }
        if (!root || (*root).is_null())
            return root;

        (*root).height = 1 + max(this->fetch_height((*root).left), this->fetch_height((*root).right));
        int balance = this->fetch_balance(root);
        if ((balance > 1) && (this->fetch_balance((*root).left) >= 0))
            return this->rotateRight(root);
        if ((balance < -1) && (this->fetch_balance((*root).right) <= 0))
            return this->rotateLeft(root);
        if ((balance > 1) && (this->fetch_balance((*root).left) < 0))
        {
            Status_Node *x = this->rotateLeft((*root).left);
            (*root).left = x;
            return this->rotateRight(root);
        }
        if ((balance < -1) && (this->fetch_balance((*root).right) > 0))
        {
            Status_Node *x = this->rotateRight((*root).right);
            (*root).right = x;
            return this->rotateLeft(root);
        }

        return root;
    }

    Status_Node *fetch_min_val_node(Status_Node *root)
    {
        if (!root || (*root).is_null() || !((*root).left) || ((*root).left)->is_null())
            return root;

        return this->fetch_min_val_node((*root).left);
    }

    int fetch_height(Status_Node *root)
    {
        if (!root || (*root).is_null())
            return 0;
        else
            return (*root).height;
    }
    int fetch_balance(Status_Node *root)
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

    vector<vector<Line>> get_segments_containing_point(vector<double> point)
    {
        vector<vector<Line>> result(3, vector<Line>{});
        this->inner_get_segments(this->root, point, result);
        return result;
    }
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
    void inner_find_left(Status_Node *root, vector<double> point, Status_Node left)
    {
        if (!root || (*root).is_null())
            return;
        else if ((*root).line.pointWRTLine(point[0], point[1]) > 0)
        {
            left.copy((*root));
            this->inner_find_left((*root).right, point, left);
        }
        else if ((*root).line.pointWRTLine(point[0], point[1]) <= 0)
        {
            this->inner_find_left((*root).left, point, left);
        }
    }

    Line get_adjacent_left_line(vector<double> point)
    {
        Status_Node left = Status_Node();
        this->inner_find_left(this->root, point, left);
        return left.line;
    }

    Line get_adjacent_right_line(vector<double> point)
    {
        Status_Node right = Status_Node();
        this->inner_find_right(this->root, point, right);
        return right.line;
    }

    void inner_find_right(Status_Node *root, vector<double> point, Status_Node right)
    {
        if (!root || (*root).is_null())
            return;
        else if ((*root).line.pointWRTLine(point[0], point[1]) >= 0)
        {
            this->inner_find_right((*root).right, point, right);
        }
        else if ((*root).line.pointWRTLine(point[0], point[1]) < 0)
        {
            right.copy((*root));
            this->inner_find_right((*root).left, point, right);
        }
    }
    void inner_leftmost(Status_Node *root, vector<double> point, Status_Node leftmost)
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
            leftmost.copy((*root));
            this->inner_leftmost((*root).left, point, leftmost);
        }
    }
    void inner_rightmost(Status_Node *root, vector<double> point, Status_Node rightmost)
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
            rightmost.copy((*root));
            this->inner_rightmost((*root).right, point, rightmost);
        }
    }
    Line get_leftmost(vector<double> point)
    {
        Status_Node leftmost = Status_Node();
        this->inner_leftmost(this->root, point, leftmost);
        return leftmost.line;
    }

    Line get_rightmost(vector<double> point)
    {
        Status_Node rightmost = Status_Node();
        this->inner_rightmost(this->root, point, rightmost);
        return rightmost.line;
    }
};

// int main()
// {

//     vector<Line> lines = {};
//     vector<pair<double, double>> x = {
//         {0, 0},
//         {1, 2},
//         {2, 3},
//         {3, 2},
//         {2, 1},
//         {2, 1},
//         {1, 2},
//         {1, 1},
//         {1.5, 1},
//         {0, 0}};
//     vector<pair<double, double>> y = {
//         {1, 2},
//         {2, 3},
//         {3, 2},
//         {2, 1},
//         {0, 0},
//         {2, 3},
//         {4, 2},
//         {1, 1.5},
//         {1, 1},
//         {3, 3}};
//     double xc[2] = {};
//     double yc[2] = {};

//     for (int i = 0; i < (int)x.size(); i++)
//     {
//         xc[0] = x[i].first;
//         xc[1] = x[i].second;
//         yc[0] = y[i].first;
//         yc[1] = y[i].second;

//         lines.push_back(Line(xc, yc));
//     }

//     StatusStructure T = StatusStructure();

//     vector<double> point = {2, 3};
//     T.insert_lines(point, lines);
//     cout << "All set";
//     return 0;
// }
