#ifndef __AVLTREE_HPP__
#define __AVLTREE_HPP__

struct node
{
    int data;
    int height;
    struct node* l;
    struct node* r;
};

typedef struct node* node_ptr;

void
dispose(node_ptr n);

node_ptr
find( int e, node_ptr n );

node_ptr
insert( int e, node_ptr n );

#endif /* __AVLTREE_HPP__  */
