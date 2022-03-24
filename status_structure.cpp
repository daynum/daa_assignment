/*a node will have lines passing through it, left node, right node, and it's height*/
#include <line.cpp>
class Node
{
public:
    Line line;
    Node *left;
    Node *right;
    int height;
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
};

class StatusStructure
{
public:
    StatusStructure()
    {
        Node head = nullptr;
    }
    StatusStructure(Line myline)
    {
        Node head;
        head.line = myline;
        head.left = nullptr;
        head.right = nullptr;
        head.height = 1;
    }
    Node insert(Node root, Line myline)
    {
        Node node;
        node.line = myline;
        node.left = nullptr;
        node.right = nullptr;
        node.height = 1;

        if (root.line.upper_end.y > node.line.upper_end.y)
            root.right = &node;
        else if (root.line.upper_end.y < node.line.upper_end.y)
            root.left = &node;
        if (root.left->height > root.right->height)
        {
            root.height = root.left->height + 1;
        }
        else
        {
            root.height = root.right->height + 1;
        }

        // implement balancing of the tree, using right and left rotate
    }
    Node insert(Node root)
    {
        if (root.line.upper_end.x + root.line.upper_end.x + root.line.lower_end.y + root.line.lower_end.y != 0)
        {
            // Traverse the tree and insert node appropriately
        }

        // implement balancing of the tree, using right and left rotate
    }
    Node rotateLeft(Node node)
    {
        Node x = *node.right;
        Node temp = *x.left;
        *x.left = node;
        *node.right = temp;
        if (node.left->height > node.right->height)
        {
            node.height = node.left->height + 1;
        }
        else
        {
            node.height = node.right->height + 1;
        }

        if (x.left->height > x.right->height)
        {
            x.height = x.left->height + 1;
        }
        else
        {
            x.height = x.right->height + 1;
        }

        return x;
    }
    Node rotateRight(Node node)
    {
        Node x = *node.left;
        Node temp = *x.right;
        *x.left = node;
        *node.right = temp;
        if (node.left->height > node.right->height)
        {
            node.height = node.left->height + 1;
        }
        else
        {
            node.height = node.right->height + 1;
        }

        if (x.left->height > x.right->height)
        {
            x.height = x.left->height + 1;
        }
        else
        {
            x.height = x.right->height + 1;
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
    int checkBalance(Node root)
    {
        return root.left->height - root.right->height;
    }

    // implement right and left adjacent finders
    // leftmost and rightmost finders
    // simple inorder traversal function
    // searching segments by points
};
