#include "avltree.hpp"

#include <iostream>
#include <algorithm>

#define __GET_HEIGHT(N) ((N)?N->height : -1)

void
dispose(node_ptr n)
{
    if ( n )
    {
        dispose( n->l );
        dispose( n->r );
        delete( n );
    }
}

node_ptr
find( int e, node_ptr n )
{
    if ( n == nullptr ) return nullptr;

    if ( e < n->data ) return find( e, n->l );
    else
    {
        if( e > n->data ) return find( e, n->r );
        else return n;
    }
}

static node_ptr single_rotate_with_left( node_ptr k2 )
{
    node_ptr k1 = nullptr;
 
    k1 = k2->l;
    k2->l = k1->r;
    k1->r = k2;
 
    k2->height = std::max( __GET_HEIGHT( k2->l ), __GET_HEIGHT( k2->r ) ) + 1;
    k1->height = std::max( __GET_HEIGHT( k1->l ), k2->height ) + 1;

    return k1;
} 

static node_ptr single_rotate_with_right( node_ptr k1 )
{
    node_ptr k2 = nullptr;
 
    k2 = k1->r;
    k1->r = k2->l;
    k2->l = k1;
 
    k1->height = std::max( __GET_HEIGHT( k1->l ), __GET_HEIGHT( k1->r ) ) + 1;
    k2->height = std::max( __GET_HEIGHT( k2->r ), k1->height ) + 1;
 
    return k2;
}

static inline node_ptr double_rotate_with_left( node_ptr k3 )
{
    k3->l = single_rotate_with_right( k3->l );
    return single_rotate_with_left( k3 );
}
 
static inline node_ptr double_rotate_with_right( node_ptr k1 )
{
    k1->r = single_rotate_with_left( k1->r );
    return single_rotate_with_right( k1 );
}


node_ptr
insert( int e, node_ptr n )
{
    auto create_node = [&](void)->node_ptr
    {
        node_ptr t = nullptr;

        try
        {
            t = new node;
        }
        catch (std::bad_alloc& ba)
        {
            std::cerr << "bad_alloc caught: " << ba.what() << '\n';
        }

        t->data = e;
        t->height = 0;
        t->l = nullptr;
        t->r = nullptr;
        return t;
    };

    for(;;)
    {        
        if ( !n )
        {
            n = create_node();
            break;
        }

        if ( e < n->data )
        {
            n->l = insert( e, n->l );
            if( __GET_HEIGHT( n->l ) - __GET_HEIGHT( n->r ) == 2 )
            {
                if( e < n->l->data )
                    n = single_rotate_with_left( n );
                else
                    n = double_rotate_with_left( n );
            }
            break;
        }

        if ( e > n->data )
        {
            n->r = insert( e, n->r );
            if( __GET_HEIGHT( n->r ) - __GET_HEIGHT( n->l ) == 2 )
            {
                if( e > n->r->data )
                    n = single_rotate_with_right( n );
                else
                    n = double_rotate_with_right( n );
            }
            break;
        }
    }

    n->height = std::max( __GET_HEIGHT( n->l ), __GET_HEIGHT( n->r ) ) + 1;
    return n;
}
