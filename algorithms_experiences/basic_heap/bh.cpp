#include <vector>
#include <cstdio>
 
#define PREVIOUS_INDEX(I) I-1
 
class Heap
{
  
    public:
 
        Heap();
        Heap( int* arrayPtr, int length );
        virtual ~Heap(){};
 
        void insert( const int& v );
        int takeOutMin();
        void print();
 
    private:
 
        void heapify();
        void swap( const int& i1 , const int& i2 );
        void percolateUp( const int& index );
        void percolateDown( int index );
 
        std::vector<int> m_store;
};
  
Heap::Heap()
{
  
}
 
Heap::Heap( int* arrayPtr, int length ) {
      
    int i = 1;
     
    this->m_store.resize( length + 1 );
 
    // Just copy the values into our array.
    for (  ; i <= length; i++ )
    {
        this->m_store[i] = arrayPtr[ PREVIOUS_INDEX(i) ];
    } 
    
    // Rearranges values into a heap.
    this->heapify();
 
}
 
void Heap::heapify( )
{
    // First half size
    int i = ( this->m_store.size() >> 1 );
      
    // Running percolateDown on the first half of the 
    // elements, in reverse order.
    for ( ; i > 0; i-- )
    {
        this->percolateDown( i );    
    }
}
 
void Heap::percolateDown( int index )
{
    int min;
     
    // Only try to percolate down internal nodes.
    if ((2*index+1) <= this->m_store.size() )
    {
 
        auto minimum = [=]( const int a, const int indexa, const int b, const int indexb )->int
        {
           // Return the value associated with a.    
           if (a < b)  return indexa;
           // Return the value associated with b.
           else  return indexb;
        };
                 
        // Find the minimum value of the two children of this node.            
        min = minimum( this->m_store[2*index], 2*index, this->m_store[2*index+1], 2*index+1);
         
        // If this value is less than the current value, then we need to move
        // our current value down the heap.  
        if ( this->m_store[index] > this->m_store[min] )
        {
            this->swap( index, min );
         
            // This part is recursive and allows us to continue percolating
            // down the element in question.
            this->percolateDown( min );
        }
    }
     
    // Case where our current element has exactly one child, a left child.
    else if ( this->m_store.size() == 2*index )
    {         
        // Here we only compare the current item to its only child.
        // Clearly, no recursive call is needed since the child of this node
        // is a leaf. 
        if ( this->m_store[index] > this->m_store[2*index] )
        {
            this->swap( index, 2*index );
        }
    }
}
 
 
void Heap::percolateUp( const int& index )
{
    // Can only percolate up if the node isn't the root.
    if ( index > 1 )
    {              
        // See if our current node is smaller in value than its parent.        
        if ( this->m_store[index/2] > this->m_store[index] )
        {            
            // Move our node up one level.
            this->swap( index, index/2 );
             
            // See if it needs to be done again.
            this->percolateUp( index/2 );
        }
    }
}
 
void Heap::swap( const int& i1 , const int& i2 )
{
    const int temp = this->m_store[i1];
    this->m_store[i1] = this->m_store[i2];
    this->m_store[i2] = temp;
}
 
void Heap::insert( const int& v )
{  
    this->m_store.push_back( v );
    this->percolateUp( this->m_store.size() );
}
   
int Heap::takeOutMin()
{
    int retVal = -1;
     
    // We can only remove an element, if one exists in the heap!
    if ( this->m_store.size() > 0 )
    {
        // This is where the minimum is stored.
        retVal = this->m_store[1];
         
        // Copy the last value into this top slot.
        this->m_store[1] = this->m_store.back();
        this->m_store.pop_back();
 
        // Need to let this value move down to its rightful spot in the heap.
        this->percolateDown( 1 );
         
        // Now we can return our value.
        return retVal;
    }
     
    return retVal;
}
  
 
void Heap::print()
{
    int i = 1;
     
    for (; i <= this->m_store.size() - 1; i++) printf("%d ", this->m_store[i]);
 
    printf("\n");
} 
  
int main()
{
    int number_of_elements = 8;
    int a[] = {10,15,17,4,5,6,8,9};
     
    Heap h( a , 8 );
 
    printf( "Printing Heap\n" );
 
    h.print();
 
    printf( "---------------------------------------\n" );
 
    printf( "Heap sort by creating a heap out of the values\n" );
 
    // Runs a heap sort by creating a heap out of the values in the array, and then
    // extracting those values one-by-one from the heap back into the array in the
    // proper order.
    {    
        int iter, element;
 
        for(iter = 0;iter < number_of_elements;iter++)
        {
            printf( "%d ", h.takeOutMin() );
        }
  
        printf("\n");
    }
    return 0;
}
