#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <map>

#define NODE_BLACK 0
#define NODE_RED   1


class RedBlackTreeError : public std::runtime_error
{
    public:
        RedBlackTreeError( std::string explanation ) : runtime_error( explanation ) {}
};


enum rbtree_errors {
    RBTREE_SUCCEED = 0,
    RBTREE_SYSTEM_ERROR = -1,
    RBTREE_NODE_NULL_PTR = -1000,
};

static std::map<enum rbtree_errors, std::string> error_msgs = {
    { RBTREE_SYSTEM_ERROR, "A fatal error related with system limits has happened" },
    { RBTREE_NODE_NULL_PTR, "The pointer that references the node has been detected null" },
};


static std::string
explain_err( int err_code )
{
    bool found = false;
    std::map<enum rbtree_errors, std::string > ::const_iterator it = error_msgs.find((enum rbtree_errors)err_code);

    if (it != error_msgs.end())
    {
        /* element was found !*/
        found = true;
    }

    return (found) ? error_msgs[(enum rbtree_errors)err_code] : "An unknow error has been detected";
}


struct Node {
    struct Node* l_ptr;
    struct Node* r_ptr;
    struct Node* parent_ptr;
    unsigned int color;
    int data;
};

typedef struct Node* NodePtr;

class RedBlackTree
{
public:

    void insertNode( int data )
    {
        NodePtr node_ptr = nullptr;
        NodePtr parent_ptr = this->findoutPlacement( data );
        int erc = setupNode( node_ptr, parent_ptr, data );

        if ( erc < RBTREE_SUCCEED )
        {
            throw RedBlackTreeError( explain_err( erc ) );
        }

        if ( parent_ptr != nullptr )
        {
            if ( data < parent_ptr->data )
            {
                parent_ptr->l_ptr = node_ptr;
            }
            else
            {
                parent_ptr->r_ptr = node_ptr;
            }
        }
        else
        {
            // It just applies for the first node pointer to arrive
            this->root_ptr = node_ptr;
        }

        return;
    }

private:

    int setupNode( NodePtr& node_ptr, NodePtr& parent_ptr, int data )
    {
        node_ptr = new Node;

        if ( node_ptr == nullptr )
        {
            return RBTREE_NODE_NULL_PTR;
        }

        node_ptr->parent_ptr = parent_ptr;

        node_ptr->l_ptr  = nullptr;
        node_ptr->r_ptr  = nullptr;
        node_ptr->color  = NODE_RED;
        node_ptr->data   = data;

        return RBTREE_SUCCEED;
    }

    NodePtr findoutPlacement( int data ) const
    {
        // find where node belongs

        auto repoint_node = []( NodePtr& n_ptr, int& d )->void
        {
            // repoint current_ptr to left or right pointer
            n_ptr = ( d < n_ptr->data ) ? n_ptr->l_ptr : n_ptr->r_ptr;
        };

        NodePtr current_ptr = this->root_ptr;
        NodePtr parent_ptr = nullptr;

        while ( current_ptr != nullptr )
        {
            if ( data == current_ptr->data )
            {
                return current_ptr;
            }

            parent_ptr = current_ptr;
            repoint_node( current_ptr, data );
        }

        return parent_ptr;
    }

    NodePtr root_ptr = nullptr;
};

int main()
{
    int a, maxnum, ct;
    RedBlackTree rbt;
    maxnum = 10;

    for (int ct = maxnum; ct; ct--) {
        a = rand() % 9 + 1;
        rbt.insertNode(a);
    }
    return 0;
}
