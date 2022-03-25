/*a node will have lines passing through it, left node, right node, and it's height*/
#include <line.cpp>
class Node
{
public:
    Line line;
    Node *left;
    Node *right;
    int height;
    Node(int a)
    {

        this->line = Line();
        this->height = 0;
        this->left = nullptr;
        this->right = nullptr;
    }
    Node()
    {
        this->line = Line();
        this->height = 0;
        *this->left = Node(0);
        *this->right = Node(0);
    }
    Node(Line l)
    {
        this->line = l;
        this->height = 1;
        this->left = nullptr;
        this->right = nullptr;
    }

    void copy(Node x)
    {
        this->line = x.line;
        this->height = x.height;
        this->left = x.left;
        this->right = x.right;
    }

    bool is_null()
    {
        return (this->height == 0 && this->line.isNull());
    }
};

class StatusStructure
{
public:
    Node root;
    StatusStructure()
    {
        Node root = Node();
    }

    Node insert(Node root, vector<double> point, Line l)
    {
        if (root.is_null())
        {
            return Node(l);
        }
        else if (root.line.compare_lower_angle(point[0], point[1], l) > 0)
        {

            *root.left = this->insert(*root.left, point, l);
        }
        else
        {

            *root.right = this->insert(*root.right, point, l);
        }

        root.height = 1 + max(this->fetch_height(*root.left), this->fetch_height(*root.right));
        int balance = fetch_balance(root);
        if ((balance > 1) && (root.line.compare_lower_angle(point[0], point[1], l) > 0))
            return this->rotateRight(root);
        if ((balance < -1) && (root.line.compare_lower_angle(point[0], point[1], l) < 0))
            return this->rotateLeft(root);
        if ((balance > 1) && (root.line.compare_lower_angle(point[0], point[1], l) < 0))
        {
            *root.left = this->rotateLeft(*root.left);
            return this->rotateRight(root);
        }
        if ((balance < -1) && (root.line.compare_lower_angle(point[0], point[1], l) > 0))
        {
            *root.right = this->rotateRight(*root.right);
            return this->rotateLeft(root);
        }
        return root;
    }
    Node insert_lines(vector<double> point, vector<Line> &lines)
    {
        for (Line l : lines)
        {
            this->root = this->insert(this->root, point, l);
        }
    }
    // implement print_name
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
    Node delete_node(Node root, vector<double> point, Line l)
    {
        if (root.is_null())
        {
            return root;
        }
        else if (root.line.compare_upper_angle(point[0], point[1], l) > 0)
        {
            *root.left = this->delete_node(*root.left, point, l);
        }
        else if (root.line.compare_upper_angle(point[0], point[1], l) < 0)
        {
            *root.right = this->delete_node(*root.right, point, l);
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
            root.line = temp.line;
            *root.right = this->delete_node(*root.right, point, temp.line);
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

    Node fetch_min_val_node(Node root)
    {
        if (root.is_null() || (*root.left).is_null())
            return root;

        return this->fetch_min_val_node(*root.left);
    }

    int fetch_height(Node root)
    {
        if (root.is_null())
            return 0;
        else
            return root.height;
    }
    int fetch_balance(Node root)
    {
        return (*root.left).height - (*root.right).height;
    }

    vector<vector<Line>> get_segments_containing_point(vector<double> point)
    {
        vector<vector<Line>> result(3, vector<Line>{});
        this->inner_get_segments(this->root, point, result);
        return result;
    }
    void inner_get_segments(Node root, vector<double> point, vector<vector<Line>> &result)
    {
        if (root.is_null())
        {
            return;
        }
        else if (root.line.pointWRTLine(point[0], point[1]) > 0)
        {
            this->inner_get_segments(*root.right, point, result);
        }
        else if (root.line.pointWRTLine(point[0], point[1]) < 0)
        {
            this->inner_get_segments(*root.left, point, result);
        }
        else
        {
            if (!(*root.left).is_null())
            {
                this->inner_get_segments(*root.left, point, result);
            }
            if (root.line.lower_end == point)
            {
                result[0].push_back(root.line);
            }
            else
            {
                result[1].push_back(root.line);
            }
            result[2].push_back(root.line);

            if (!(*root.right).is_null())
            {
                this->inner_get_segments(*root.right, point, result);
            }
        }
    }
    void inner_find_left(Node root, vector<double> point, Node left)
    {
        if (root.is_null())
            return;
        else if (root.line.pointWRTLine(point[0], point[1]) > 0)
        {
            left.copy(root);
            this->inner_find_left(*root.right, point, left);
        }
        else if (root.line.pointWRTLine(point[0], point[1]) <= 0)
        {
            this->inner_find_left(*root.left, point, left);
        }
    }

    Line get_adjacent_left_line(vector<double> point)
    {
        Node left = Node();
        this->inner_find_left(this->root, point, left);
        return left.line;
    }

    Line get_adjacent_right_line(vector<double> point)
    {
        Node right = Node();
        this->inner_find_right(this->root, point, right);
        return right.line;
    }

    void inner_find_right(Node root, vector<double> point, Node right)
    {
        if (root.is_null())
            return;
        else if (root.line.pointWRTLine(point[0], point[1]) >= 0)
        {
            this->inner_find_right(*root.right, point, right);
        }
        else if (root.line.pointWRTLine(point[0], point[1]) < 0)
        {
            right.copy(root);
            this->inner_find_right(*root.left, point, right);
        }
    }
    void inner_leftmost(Node root, vector<double> point, Node leftmost)
    {
        if (root.is_null())
            return;
        else if (root.line.pointWRTLine(point[0], point[1]) > 0)
        {
            this->inner_leftmost(*root.right, point, leftmost);
        }
        else if (root.line.pointWRTLine(point[0], point[1]) < 0)
        {
            this->inner_leftmost(*root.left, point, leftmost);
        }
        else
        {
            this->inner_leftmost(*root.right, point, leftmost);
            leftmost.copy(root);
            this->inner_leftmost(*root.left, point, leftmost);
        }
    }
    void inner_rightmost(Node root, vector<double> point, Node rightmost)
    {
        if (root.is_null())
            return;
        else if (root.line.pointWRTLine(point[0], point[1]) > 0)
        {
            this->inner_rightmost(*root.right, point, rightmost);
        }
        else if (root.line.pointWRTLine(point[0], point[1]) < 0)
        {
            this->inner_rightmost(*root.left, point, rightmost);
        }
        else
        {
            this->inner_rightmost(*root.left, point, rightmost);
            rightmost.copy(root);
            this->inner_rightmost(*root.right, point, rightmost);
        }
    }
    Line get_leftmost(vector<double> point)
    {
        Node leftmost = Node();
        this->inner_leftmost(this->root, point, leftmost);
        return leftmost.line;
    }

    Line get_rightmost(vector<double> point)
    {
        Node rightmost = Node();
        this->inner_rightmost(this->root, point, rightmost);
        return rightmost.line;
    }
};
