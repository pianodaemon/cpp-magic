#include <stdio.h>
#include <stdlib.h>

#define ALLOC_MEMORY(OBJ_TYPE,N)                malloc( sizeof( OBJ_TYPE ) * N )

typedef enum color
{
    RED,
    BLACK
} color_t;

struct node
{
    unsigned long item;
    color_t c;
    struct node *l_ptr;
    struct node *r_ptr;
};

typedef struct node node_t;

extern int
rbtree_init( node_t **t_ptr, unsigned long item )
{
    int rc = 0;

    for(;;)
    {
        if ( t_ptr == NULL )
        {
            rc = -1;
            break;
        }

        *t_ptr = (node_t *)ALLOC_MEMORY( struct node, 1 );

        if ( *t_ptr == NULL )
        {
            rc = -1;
            break;
        }

        {
            node_t *s_ptr = NULL;

            s_ptr = *t_ptr;
            s_ptr->l_ptr = s_ptr->r_ptr = NULL;
            s_ptr->c = BLACK;
            s_ptr->item = item;
        }
        break;
    }

    return rc;
}

static node_t*
node_free_rec( node_t *t_ptr )
{
    if ( !t_ptr ) goto CONCLUDE;

    node_free_rec( t_ptr->l_ptr );
    node_free_rec( t_ptr->r_ptr );
    free( t_ptr );
    
    CONCLUDE: return NULL;
}

extern node_t*
node_free( node_t *t_ptr )
{
    t_ptr->r_ptr = node_free_rec( t_ptr->r_ptr ); 
    return t_ptr;
}

extern node_t*
node_find( node_t *t_ptr, const unsigned long item )
{
    if ( !t_ptr ) return NULL;

    if ( item < t_ptr->item )
        return node_find( t_ptr->l_ptr, item );
    else if ( item > t_ptr->item )
        return node_find( t_ptr->r_ptr, item );

    return t_ptr;
}

int main()
{
    node_t* t = NULL;
    rbtree_init( &t, 2 );
    printf( "%d\n", t->item );
    return 0;
}
