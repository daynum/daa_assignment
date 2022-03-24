/*a node will have lines passing through it, left node, right node, and it's height*/
#include <line.cpp>
class Node
{
public:
    Line line;
    Node *left;
    Node *right;
    int height;
    Node()
    {
        this->line = Line();
        this->height = 0;
        this->left = nullptr;
        this->right = nullptr;
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
    void deleteNode(Node node)
    {
        if (node.left != nullptr)
        {
            insert(*node.left);
        }
        if (node.right != nullptr)
        {
            insert(*node.right);
        }

        // node = nullptr;
        // fix by converting struct Node to a class
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
        return root.left->height - root.right->height;
    }

    // implement right and left adjacent finders
    // leftmost and rightmost finders
    // simple inorder traversal function
    // searching segments by points
};
