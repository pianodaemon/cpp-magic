#include <stdio.h>
#include <stdlib.h>
 
// MACROS FOR EXAMPLE (START)
#define MACRO_A 1000
#define MACRO_B 2000
#define MACRO_C 3000
#define MACRO_D 4000
// MACROS FOR EXAMPLE (END)
 
#define C_STRING_MACRO_IDENTIFIER(x) #x
#define LOOKUP_TABLE_ELEMENT_NUMBER 4
 
typedef struct Elem
{    
    int   key;         /* key */
    char* value;       /* information */
} TElem;
 
 
TElem one   = { .key = MACRO_D , .value = C_STRING_MACRO_IDENTIFIER( MACRO_D ) };
TElem two   = { .key = MACRO_C , .value = C_STRING_MACRO_IDENTIFIER( MACRO_C ) };
TElem three = { .key = MACRO_B , .value = C_STRING_MACRO_IDENTIFIER( MACRO_B ) };
TElem four  = { .key = MACRO_A , .value = C_STRING_MACRO_IDENTIFIER( MACRO_A ) };
 
static const TElem* lookupTable[LOOKUP_TABLE_ELEMENT_NUMBER];
 
static int comparison( const TElem** ap1, const TElem** ap2 )
{
    const TElem* a1 = *ap1;
    const TElem* a2 = *ap2;
 
    if (a1->key > a2->key)
    {
        return 1;
    }
 
    if (a1->key < a2->key)
    {
        return -1;
    }
 
    return 0;
}
 
 
static const TElem* performSearch( const TElem* ap )
{
    const TElem** aq = bsearch( &ap, lookupTable, LOOKUP_TABLE_ELEMENT_NUMBER, sizeof( const TElem* ), (int (*)(const void *, const void *)) &comparison );
 
    if ( aq != NULL )
    {
        return *aq;
    }
    else
    {
        return NULL;
    }
}
 
extern const TElem* elementLookupGet( const int key )
{
    const TElem e = { key , NULL };
 
    return performSearch( (void *) &e );
}
 
 
static void performSort()
{
    qsort( lookupTable, LOOKUP_TABLE_ELEMENT_NUMBER, sizeof( const TElem* ), (int (*)(const void *, const void *)) &comparison );
}
 
extern void elementLookupRegister()
{
    int num = 0;
 
    lookupTable[ num++ ] = &one;
    lookupTable[ num++ ] = &two;
    lookupTable[ num++ ] = &three;
    lookupTable[ num++ ] = &four;
 
    performSort();
}
 
 
int main()
{
     
    elementLookupRegister();
 
    const TElem* myPtr = elementLookupGet( 1000 );
    printf("%s\n", myPtr->value);
 
    myPtr = elementLookupGet( 2000 );
    printf("%s\n", myPtr->value);
 
    myPtr = elementLookupGet( 3000 );
    printf("%s\n", myPtr->value);
 
    myPtr = elementLookupGet( 4000 );
    printf("%s\n", myPtr->value);
 
    return 0;
}
