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
        Line *temp = new Line();
        this->line = *temp;
        this->height = 0;
        this->left = nullptr;
        this->right = nullptr;
    }
    Status_Node(Line l)
    {
        this->line = l;
        this->height = 1;
        this->left = new Status_Node();
        this->right = new Status_Node();
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
    void insert_lines(vector<double> point, vector<Line> &lines)
    {
        for (Line l : lines)
        {
            this->root = this->insert(this->root, point, l);
        }
    }
    Status_Node *insert(Status_Node *root, vector<double> point, Line l)
    {
        if (!root || (*root).is_null())
        {
            Status_Node *x = new Status_Node(l);
            return x;
        }
        else if ((*root).line.compare_lower_angle(point[0], point[1], l) > 0)
        {
            if (!(*root).left || (*root).left->is_null())
            {
                // bas yahape new likhna tha bhenchod
                Status_Node *x = new Status_Node(l);
                (*root).left = x;
            }
            else
            {
                Status_Node *x = this->insert((*root).left, point, l);
                (*root).left = x;
            }
        }
        else
        {
            if ((!(*root).right) || (*root).right->is_null())
            {
                Status_Node *x = new Status_Node(l);
                (*root).right = x;
            }
            else
            {
                Status_Node *x = this->insert((*root).right, point, l);
                (*root).right = x;
            }
        }

        (*root).height = 1 + max(this->fetch_height((*root).left), this->fetch_height((*root).right));
        root = balance_insert(point, root, l);
        return root;
    }

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

    vector<Status_Node> self_inorder()
    {
        cout << "\n";
        vector<Status_Node> result{};
        this->inorder(this->root, result);
        cout << "\n";
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
            cout << " attempting deletion of line: " << l.name;
            this->delete_line(point, l);
            cout << " " << l.name << " deleted. ";
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
            if ((*root).line.lower_end == point && (*root).line.name != "empty_null_line")
            {
                result[0].push_back((*root).line);
            }
            else
            {
                if ((*root).line.name != "empty_null_line")
                    result[1].push_back((*root).line);
            }
            if ((*root).line.name != "empty_null_line")
                result[2].push_back((*root).line);

            if ((!(*root).right->is_null()) || (!(*root).right))
            {
                this->inner_get_segments((*root).right, point, result);
            }
        }
    }

    Line get_adjacent_left_line(vector<double> point)
    {
        Status_Node *left = new Status_Node();
        this->inner_find_left(this->root, point, left);
        return (*left).line;
    }
    void inner_find_left(Status_Node *root, vector<double> point, Status_Node *left)
    {
        if (!root || (*root).is_null() || (*root).line.isNull())
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

    Line get_adjacent_right_line(vector<double> point)
    {
        Status_Node *right = new Status_Node();
        this->inner_find_right(this->root, point, right);
        return (*right).line;
    }

    void inner_find_right(Status_Node *root, vector<double> point, Status_Node *right)
    {
        if (!root || (*root).is_null() || (*root).line.isNull())
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

    Line get_leftmost(vector<double> point)
    {
        Status_Node *leftmost = new Status_Node();
        this->inner_leftmost(this->root, point, leftmost);
        return (*leftmost).line;
    }

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
    }

    Line get_rightmost(vector<double> point)
    {
        Status_Node *rightmost = new Status_Node();
        this->inner_rightmost(this->root, point, rightmost);
        return (*rightmost).line;
    }

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

// int main()
// {
//     StatusStructure T = StatusStructure();
//     int k = 0;
//     ////////////////////////////////////////////////////////
//     vector<Line> lines{};
//     cout << "Phase 1 insertion";
//     vector<pair<double, double>> x = {
//         {1, 2},
//         {2, 3},
//         {2, 1}};
//     vector<pair<double, double>> y = {
//         {2, 3},
//         {3, 2},
//         {2, 3}};
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
//     vector<double> point = {2, 3};
//     T.insert_lines(point, lines);
//     /////////////////////////////////////////////////////////////
//     cout << "Phase 2 insertion";
//     lines = {};
//     x = {
//         {0, 0}};
//     y = {
//         {3, 3}};

//     for (int i = 0; i < (int)x.size(); i++)
//     {
//         xc[0] = x[i].first;
//         xc[1] = x[i].second;
//         yc[0] = y[i].first;
//         yc[1] = y[i].second;

//         lines.push_back(Line(xc, yc));
//     }
//     point = {3, 3};
//     T.insert_lines(point, lines);
//     /////////////////////////////////////////////////////////////
//     cout << "Phase 3 deletion";
//     lines = {};
//     x = {
//         {2, 3},
//         {0, 0}};
//     y = {
//         {3, 2},
//         {3, 3}};

//     for (int i = 0; i < (int)x.size(); i++)
//     {
//         xc[0] = x[i].first;
//         xc[1] = x[i].second;
//         yc[0] = y[i].first;
//         yc[1] = y[i].second;

//         lines.push_back(Line(xc, yc));
//     }
//     point = {2.5, 2.5};
//     T.delete_lines(point, lines);
//     ///////////////////////////////////////////////////////////////
//     cout << "Phase 4 insertion";
//     lines = {};
//     x = {
//         {2, 3},
//         {0, 0}};
//     y = {
//         {3, 2},
//         {3, 3}};

//     for (int i = 0; i < (int)x.size(); i++)
//     {
//         xc[0] = x[i].first;
//         xc[1] = x[i].second;
//         yc[0] = y[i].first;
//         yc[1] = y[i].second;

//         lines.push_back(Line(xc, yc));
//     }
//     point = {2.5, 2.5};
//     T.insert_lines(point, lines);
//     /////////////////////////////////////////////////////////////
//     cout << "Phase 5 deletion";
//     lines = {};
//     x = {
//         {1, 2}};
//     y = {
//         {2, 3}};

//     for (int i = 0; i < (int)x.size(); i++)
//     {
//         xc[0] = x[i].first;
//         xc[1] = x[i].second;
//         yc[0] = y[i].first;
//         yc[1] = y[i].second;

//         lines.push_back(Line(xc, yc));
//     }
//     point = {1, 2};
//     T.delete_lines(point, lines);
//     ///////////////////////////////////////////////////////////////
//     cout << "Phase 6 insertion";
//     lines = {};
//     x = {
//         {0, 0},
//         {1, 2}};
//     y = {
//         {1, 2},
//         {4, 2}};

//     for (int i = 0; i < (int)x.size(); i++)
//     {
//         xc[0] = x[i].first;
//         xc[1] = x[i].second;
//         yc[0] = y[i].first;
//         yc[1] = y[i].second;

//         lines.push_back(Line(xc, yc));
//     }
//     point = {1, 2};
//     T.insert_lines(point, lines);
//     /////////////////////////////////////////////////////////////
//     cout << "Phase 7 deletion";
//     lines = {};
//     x = {
//         {1, 2},
//         {2, 1},
//         {0, 0}};
//     y = {
//         {4, 2},
//         {2, 3},
//         {3, 3}};

//     for (int i = 0; i < (int)x.size(); i++)
//     {
//         xc[0] = x[i].first;
//         xc[1] = x[i].second;
//         yc[0] = y[i].first;
//         yc[1] = y[i].second;

//         lines.push_back(Line(xc, yc));
//     }

//     point = {2, 2};
//     T.delete_lines(point, lines);
//     cout << "here?";
//     /////////////////////////////////////////////////////////////
//     cout << "Phase 8 insertion";
//     lines = {};
//     x = {
//         {1, 2},
//         {2, 1},
//         {0, 0}};
//     y = {
//         {4, 2},
//         {2, 3},
//         {3, 3}};

//     for (int i = 0; i < (int)x.size(); i++)
//     {
//         xc[0] = x[i].first;
//         xc[1] = x[i].second;
//         yc[0] = y[i].first;
//         yc[1] = y[i].second;

//         lines.push_back(Line(xc, yc));
//     }
//     point = {2, 2};
//     T.insert_lines(point, lines);
//     T.self_inorder();
//     cout << "it's done";
//     /////////////////////////////////////////////////////////////

//     //     vector<Line> lines = {};
//     //     vector<pair<double, double>> x = {
//     //         {0, 0},
//     //         {1, 2},
//     //         {2, 3},
//     //         {3, 2},
//     //         {2, 1},
//     //         {2, 1},
//     //         {1, 2},
//     //         {1, 1},
//     //         {1, 1.5000},
//     //         {1.5000, 1},
//     //         {0, 0}};
//     //     vector<pair<double, double>> y = {
//     //         {1, 2},
//     //         {2, 3},
//     //         {3, 2},
//     //         {2, 1},
//     //         {0, 0},
//     //         {2, 3},
//     //         {4, 2},
//     //         {1, 1.5},
//     //         {1.5, 1},
//     //         {1, 1},
//     //         {3, 3}};
//     //     double xc[2] = {};
//     //     double yc[2] = {};

//     //     for (int i = 0; i < (int)x.size(); i++)
//     //     {
//     //         xc[0] = x[i].first;
//     //         xc[1] = x[i].second;
//     //         yc[0] = y[i].first;
//     //         yc[1] = y[i].second;

//     //         lines.push_back(Line(xc, yc));
//     //     }

//     //     StatusStructure T = StatusStructure();

//     //     vector<double> point = {2, 3};
//     //     T.insert_lines(point, lines);
//     //     cout << "All set";
//     //     return 0;
// }
