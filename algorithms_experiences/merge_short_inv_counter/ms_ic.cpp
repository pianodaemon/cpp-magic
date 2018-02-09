#include <cstdio>
#include <cstdlib>
 
#define ARRAY_LENGTH(ARRAY) sizeof(ARRAY)/sizeof(ARRAY[0])
 
 
class MergeSort
{
    public:
 
        MergeSort();
        virtual ~MergeSort();
 
        static void perform( int* arrayPtr , const int arrayLength , bool verbose = false );
 
    private:
 
        static long long separate( int* arrayPtr, int n )
        {
            long long rv = 0;
 
            auto merge = []( int* arrayPtr, const int arrayLength, const int offSetRightSubArray )->long long
            {
                long long invCounter = 0;
 
                int i; // Index for left sub-Array
                int j; // Index for right sub-Array
                int k; // Index for resultant merged sub-Array
 
                int *tempArray = new int[arrayLength];
 
                for (i = 0, j = offSetRightSubArray, k = 0; k < arrayLength; k++)
                {
                     
                    if ( j == arrayLength )
                    {
                        tempArray[k] = arrayPtr[i++];
                    }
                    else
                    {
                        if ( i == offSetRightSubArray )
                        {
                            tempArray[k] = arrayPtr[j++];
                            invCounter = invCounter + (offSetRightSubArray - i);
                        }
                        else
                        {
                            if ( arrayPtr[j] < arrayPtr[i] )
                            {
                                tempArray[k] = arrayPtr[j++];
                                invCounter = invCounter + (offSetRightSubArray - i);
                            }
                            else
                            {
                                tempArray[k] = arrayPtr[i++];
                            }
                        }
                    }
                }
 
                for (i = 0; i < arrayLength; i++) arrayPtr[i] = tempArray[i];
 
                delete(tempArray);
 
                return invCounter;
            };
 
            if (n < 2)
            {
                // nothing to do
            }
            else
            {
                rv += separate( arrayPtr , (n >> 1) );
                rv += separate( arrayPtr + (n >> 1) , n - (n >> 1) );
                rv += merge( arrayPtr , n, (n >> 1) );
            }
 
            return rv;
        };
};
 
 
void MergeSort::perform( int* arrayPtr , const int arrayLength , bool verbose )
{
    if (verbose)
    {
        printf("Array Before: ");
        for (int i = 0; i < arrayLength; i++) printf("%d%s", *(arrayPtr + i), i == arrayLength - 1 ? "\n" : " ");
    }
     
    long long c = MergeSort::separate( arrayPtr , arrayLength );
 
    if (verbose)
    {
        printf("Array After: ");
        for (int i = 0; i < arrayLength; i++) printf("%d%s", *(arrayPtr + i), i == arrayLength - 1 ? "\n" : " ");
    }
 
    if (verbose)
    {
        printf("Number of Invertions detected: %lld\n", c );
    }
}
 
 
  
int main ()
{
 
    int b[] = {4, 65, 2, -31, 0, 99, 2, 83, 782, 1};
    int a[] = {1,3,5,2,4, 6};
     
    MergeSort::perform( a , ARRAY_LENGTH( a ) , true );
    MergeSort::perform( b , ARRAY_LENGTH( b ) , true );
     
    return 0;
 
}
