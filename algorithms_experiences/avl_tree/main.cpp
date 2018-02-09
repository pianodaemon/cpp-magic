#include <stdio.h>
#include "avltree.hpp"


void display_avl(node* t)
{
    if (t == NULL)
        return;
    printf("%d",t->data);
 
    if(t->l != NULL)
        printf("(L:%d)",t->l->data);
    if(t->r != NULL)
        printf("(R:%d)",t->r->data);
    printf("\n");
 
    display_avl(t->l);
    display_avl(t->r);
} 

int main()
{
    node *t , *p;
    int i;
    int j = 0;
    const int max = 10;
 
    printf("--- C AVL Tree Demo  ---\n");
 
    t = NULL;
 
    printf("Insert: ");
    for( i = 0; i < max; i++, j = ( j + 7 ) % max )
    {
 
        t = insert( j, t );
        printf("%d ",j);
 
    }
    printf(" into the tree\n\n");
 
    display_avl(t);
 
    dispose(t);
 
    return 0;
}
