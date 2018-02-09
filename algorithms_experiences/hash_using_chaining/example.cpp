#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
 
#define HT_SUCCESS                               0
#define HT_ERROR_NULL_PTR                       -1001
#define HT_ERROR_TABLE_SIZE_IS_LESS_THAN_ZERO   -1002
 
#define ALLOC_MEMORY(OBJ_TYPE,N)                malloc( sizeof( OBJ_TYPE ) * N )
 
#define CALC_BUFFER_SIZE_FOR_DIGITS(NUM)        NUM > 0 ? (log10(NUM) + 2) : 2
 
#define FALSE 0
#define TRUE  1
 
 
typedef struct Elem
{
    char* word;         /* key */
    int  count;         /* information */
} TElem;
 
typedef struct Elem* TElemPtr;
 
typedef struct List
{
    TElemPtr data;
    struct List* next;
} TList;
 
typedef struct List* TListPtr; 
 
typedef struct Chain
{
    TListPtr list;
} TChain;
 
typedef struct Chain* TChainPtr;
 
typedef struct Table
{
    int size;
    int numElems;
    TChainPtr* array;
} TTable;
 
typedef struct Table* TTablePtr;
 
/* hash function */
/* uses pseudo-random number generation */
int hash( const char* s, int m )
{
    int a = 1664525;
    int b = 1013904223; /* inlined random number generator */
    int r = 0xdeadbeef;            /* initial seed */
    int i = 0;
    int h = 0;
    int len = strlen(s);
 
    for (; i < len; i++)
    {
        h = r * h + ( s[i] );   /* mod 2^32 */
        r = r * a + b;          /* mod 2^32, linear congruential random no */
    }
 
    h = h % m;          /* reduce to range */
 
    if (h < 0) h += m;       /* make positive, if necessary */
   
    return h;
}
 
 
int initElem( TElemPtr* elemPtr , unsigned int number )
{
    int rc = HT_SUCCESS;
 
    if ( elemPtr == NULL )
    {
        rc = HT_ERROR_NULL_PTR;
    }
    else
    {
        unsigned int bufferSize = CALC_BUFFER_SIZE_FOR_DIGITS( number );
 
        TElemPtr ePtr = ALLOC_MEMORY( TElem , 1 );
 
        if ( elemPtr == NULL )
        {
            rc = HT_ERROR_NULL_PTR;
        }
        else
        {           
            ePtr->word = ALLOC_MEMORY( char ,  bufferSize );
 
            if ( ePtr->word != NULL )
            {
                if ( number >= 0 )
                {
                    ePtr->count = number;
                    snprintf( ePtr->word ,  bufferSize , "%d", ePtr->count );
                    *elemPtr = ePtr;
                }
                else
                {
                    rc = HT_ERROR_TABLE_SIZE_IS_LESS_THAN_ZERO;
                }
            }
            else
            {
                rc = HT_ERROR_NULL_PTR;
            }
        }
    }
 
    return rc;
}
 
void freeElem( TElemPtr elemPtr )
{
    if ( elemPtr != NULL )
    {
        free( elemPtr->word );
        free ( elemPtr );
    }
}
 
 
int initChain( TChainPtr* cPtr )
{
    int rc = HT_SUCCESS;
 
    if ( cPtr == NULL )
    {
        rc = HT_ERROR_NULL_PTR;
    }
    else
    {
        TChainPtr chainPtr = NULL;
         
        chainPtr = ALLOC_MEMORY( TChain , 1 );
 
        if ( chainPtr == NULL )
        {
            rc = HT_ERROR_NULL_PTR;
        }
        else
        {
            chainPtr->list = NULL;
            *cPtr = chainPtr;
        }
    }
 
    return rc;
}
 
void freeChain( TChainPtr cPtr )
{
    if ( cPtr != NULL )
    {
        TListPtr head = cPtr->list;
        TListPtr tmp = NULL;
 
        while( head != NULL )
        {
            tmp = head;
            head = head->next;
            freeElem( tmp->data );
            free(tmp);
        }
     
        free( cPtr );
    }
}
 
 
int initTable( TTablePtr* tablePtr , const int initialSize )
{
    int rc = HT_SUCCESS;
 
    if ( tablePtr == NULL ) 
    {
        rc = HT_ERROR_NULL_PTR;
    }
    else
    {        
        TTablePtr tPtr = ALLOC_MEMORY( TTable , 1 );       
 
        if ( tPtr )
        {
            tPtr->array = ALLOC_MEMORY( TChainPtr , initialSize );
 
            if ( tPtr->array )
            {
                memset( tPtr->array , 0 , ( sizeof( TChainPtr ) * initialSize ));           
 
                tPtr->size = initialSize;
                tPtr->numElems = 0;
 
                *tablePtr = tPtr;
            }
            else
            {
                rc = HT_ERROR_NULL_PTR;
            }
        }
        else
        {
            rc = HT_ERROR_NULL_PTR;
        }       
    }
 
    return rc;
}
 
void freeTable( TTablePtr tPtr )
{
    if ( tPtr != NULL )
    {
        unsigned int iter = 0;
 
        for( ; iter <= tPtr->size; iter++ )
        {
            freeChain( tPtr->array[ iter ] );
        }
     
        free( tPtr );
    }
}
 
 
TListPtr findOnChain( const TChainPtr cPtr, const char* key )
{
    if ( cPtr && key )
    {
        TListPtr p = cPtr->list;
 
        while (!( p == NULL ))
        {
            const char* elemtKey = p->data->word;
 
            if ( strcmp( key, elemtKey ) == 0 ) return p;
             
            p = p->next;
        }
    }
 
    return NULL;
}
 
 
TElemPtr insertOnChain( TChainPtr cPtr, const TElemPtr e )
{
    const char* keyToSearch = e->word;
    TListPtr p = findOnChain( cPtr , keyToSearch );
 
    if ( p == NULL )
    {
        /* insert new element at the beginning */
 
        TListPtr newItem = ALLOC_MEMORY( TList ,  1 );
 
        newItem->data = e;
        newItem->next = cPtr->list;
        cPtr->list = newItem;
 
        return NULL;        /* did not overwrite entry */
    }
    else
    {
        /* overwrite existing entry with given key */
 
        TElemPtr oldElement = p->data;
        p->data = e;
 
        return oldElement;      /* return old entry */
    }
}
 
TElemPtr insertOnTable( TTablePtr table , const TElemPtr e ) 
{
    TElemPtr oldElement = NULL;
 
    const char* key = e->word;
 
    int h = hash( key , table->size );
 
    if ( table->array[h] == NULL )
    {
        TChainPtr cPtr = NULL;
 
        int rv = initChain( &cPtr );
 
        if ( rv < HT_SUCCESS )
        {
            printf("There is no longer available memory\n");
            assert(FALSE);
        }
        else
        {
            table->array[h] = cPtr;
        }
    }
 
    oldElement = insertOnChain( table->array[h] , e );
 
    if ( oldElement == NULL ) table->numElems++;
 
    return oldElement;  
}
 
 
TElemPtr searchOnChain( const TChainPtr cPtr, const char* key )
{
    TElemPtr e = NULL;
 
    if ( cPtr && key )
    {
        TListPtr p = findOnChain( cPtr , key );
 
        if (!( p == NULL ))
        {
            e = p->data;
        }
    }
 
    return e;
}
 
 
TElemPtr searchOnTable( const TTablePtr table, const char* key )
{
    TElemPtr e = NULL;
 
    if ( table && key )
    {
        int h = hash( key, table->size );
 
        if (!( table->array[h] == NULL ))
        {
            e = searchOnChain( table->array[h] , key );
        }
    }   
 
    return e;
}
 
int populateTable( TTablePtr tablePtr , const unsigned int n, unsigned int j )
{
    int rc = HT_SUCCESS;
     
    unsigned int i = 0;
 
    for ( ; i < n; i++ )
    {
        TElemPtr ePtr = NULL;
        rc = initElem( &ePtr , j*n+i );
 
        if ( rc < HT_SUCCESS ) break;
 
        insertOnTable( tablePtr , ePtr );
    }
 
    return rc;
}
 
 
void verification( TTablePtr tablePtr , const unsigned int n, unsigned int j )
{
    unsigned int i = 0;
 
    for ( ; i < n; i++ )
    {
        printf(" i = %d \n", i );
 
        {
            unsigned int dn = CALC_BUFFER_SIZE_FOR_DIGITS( j*n+i );
 
            char* key = ALLOC_MEMORY( char , dn );
 
            snprintf( key , dn , "%d", j*n+i );
 
            if (! ( searchOnTable( tablePtr, key )->count == j*n+i ) )
            {
                printf("missed existing element\n");
                assert(FALSE);
            }
        }
             
        {
            unsigned int dn = CALC_BUFFER_SIZE_FOR_DIGITS( (j+1)*n+i );
 
            char* key = ALLOC_MEMORY( char , dn );
 
            snprintf( key , dn , "%d", (j+1)*n+i );
 
            if (! ( searchOnTable( tablePtr, key ) == NULL ) )
            {
                printf("found nonexistent element\n");
                assert(FALSE);
            }
        }
    }
}
 
 
int main()
{
    int n = (512)+1; // start with 1<<10 for timing; 1<<9 for -d
    int num_tests = 10; // start with 1000 for timing; 10 for -d
    int j;
 
    printf("Testing array of size %d", n/5);
    printf(" with %d values %d times\n", n, num_tests );
 
    int rc = HT_SUCCESS;
 
    for (j = 0; j < num_tests; j++)
    {
        TTablePtr tablePtr = NULL;
 
        rc = initTable( &tablePtr , n/5 );
 
        if ( rc ==  HT_SUCCESS )
        {
            rc = populateTable( tablePtr , n , j );
             
            if ( rc <  HT_SUCCESS )
            {
                break;
            }           
 
            verification( tablePtr , n, j );            
        }
 
        freeTable( tablePtr );
     
    }
 
    if ( rc < HT_SUCCESS )
    {
        printf("Something bad happened!\n");
    }
    else
    {
        printf("All tests passed!\n");
    }
 
    return 0;
}
