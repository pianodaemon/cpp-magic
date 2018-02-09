#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cfloat>
#include <algorithm>
 
#define MINOR_POINT_NUMBER_ALLOWED 2
#define LIMIT_TO_APPLY_BRUTE_FORCE 3
 
#define ARRAY_LENGTH(ARRAY) sizeof(ARRAY)/sizeof(ARRAY[0])
#define USE_MINOR(A,B)  (A < B)? A : B
 
#define __LAMBDA_COMPARISON_FOR_QSORT(AXIS) []( Point2D p1, Point2D p2 ) -> bool { return ( p1.AXIS < p2.AXIS ); }
#define AXIS_COMPARISON_FOR_SORT_X  __LAMBDA_COMPARISON_FOR_QSORT(first)
#define AXIS_COMPARISON_FOR_SORT_Y  __LAMBDA_COMPARISON_FOR_QSORT(second)
 
#define DIST_BETWEEN_PTS(X0,Y0,X1,Y1)  sqrt((X0 - X1)*(X0 - X1) + (Y0 - Y1)*(Y0 - Y1))
 
#define GET_X(P) P.first
#define GET_Y(P) P.second
 
#define SET_X(P,V) P.first = V
#define SET_Y(P,V) P.second = V
 
typedef std::pair<int,int> Point2D;
typedef Point2D* Point2DPtr;
  
class ClosestPair
{
    public:
 
        ClosestPair();
 
        virtual ~ClosestPair(){}
 
        void addPoint2D( const int x, const int y );
        void reset();
        float searchClosest();
 
 
    private:
         
        float closestUtil( Point2DPtr pxArrayPtr , Point2DPtr pyArrayPtr , const int size );
 
    private:
 
        std::vector< Point2D > m_points;
};
 
ClosestPair::ClosestPair()
{
    this->reset();
}
 
void ClosestPair::addPoint2D( const int x, const int y )
{
    Point2D point;
    SET_X( point , x );
    SET_Y( point , y );
    this->m_points.push_back( point );
}
 
void ClosestPair::reset()
{
    this->m_points.clear();
}
 
float ClosestPair::searchClosest()
{
    float rv = -1;
 
    unsigned int n = this->m_points.size();
 
    if ( n > MINOR_POINT_NUMBER_ALLOWED )
    {
        // vectors store their elements contiguously (like array):
        const Point2DPtr firstElementVectorPtr = &this->m_points[0];
 
        Point2D px[n];
        Point2D py[n];
         
        memcpy( px , firstElementVectorPtr , n * sizeof( Point2D ) );
        memcpy( py , firstElementVectorPtr , n * sizeof( Point2D ) );
 
        std::sort( px, &( px[n] ), AXIS_COMPARISON_FOR_SORT_X );
        std::sort( py, &( py[n] ), AXIS_COMPARISON_FOR_SORT_Y );
 
        // Use recursive function closestUtil() to find the smallest distance
        rv = this->closestUtil( px , py , n );
    }
 
    return rv;
}
 
float ClosestPair::closestUtil( Point2DPtr pxArrayPtr , Point2DPtr pyArrayPtr , const int size )
{
    float rv = 0;
 
    auto bruteForce = []( Point2D pts[] , const int n )->float
    {
        float min = FLT_MAX;
 
        for (int i = 0; i < n; ++i)
        {
            for (int j = (i + 1) ; j < n; ++j)
            {
                if ( DIST_BETWEEN_PTS(GET_X(pts[i]), GET_Y(pts[i]), GET_X(pts[j]), GET_Y(pts[j]) ) < min )
                {                                                     
                    min = DIST_BETWEEN_PTS(
                            GET_X(pts[i]),
                            GET_Y(pts[i]),
                            GET_X(pts[j]),
                            GET_Y(pts[j])
                    );
                }
            }
        }
 
        return min;
    };
 
    auto stripClosest = []( Point2D strip[], int n, float d)->float
    {
        float min = d;  // Initialize the minimum distance as d
      
        // Pick all points one by one and try the next points till the difference
        // between y coordinates is smaller than d.
        // This is a proven fact that this loop runs at most 6 times
        for (int i = 0; i < n; ++i)
            for (int j = i+1; j < n && (GET_Y(strip[j]) - GET_Y(strip[i])) < min; ++j)
                if (DIST_BETWEEN_PTS(GET_X(strip[i]), GET_Y(strip[i]), GET_X(strip[j]), GET_Y(strip[j]) ) < min)
                    min = DIST_BETWEEN_PTS(GET_X(strip[i]), GET_Y(strip[i]), GET_X(strip[j]), GET_Y(strip[j]) );
      
        return min;
    };
 
    if ( size <= LIMIT_TO_APPLY_BRUTE_FORCE )
    {       
        rv = bruteForce( pxArrayPtr , size );
    }
    else
    {
        // Find the middle point
        int mid = ( size >> 1 );
        Point2D midPoint = pxArrayPtr[mid];
 
        // Divide points in y sorted array around the vertical line.
        // Assumption: All x coordinates are distinct.
        Point2D pyl[mid+1];   // y sorted points on left of vertical line
        Point2D pyr[size-mid-1];  // y sorted points on right of vertical line
        int li = 0, ri = 0;  // indexes of left and right subarrays
        for (int i = 0; i < size; i++)
        {
          if (GET_X(pyArrayPtr[i]) <= GET_X(midPoint))
             pyl[li++] = pyArrayPtr[i];
          else
             pyr[ri++] = pyArrayPtr[i];
        }
      
        // Consider the vertical line passing through the middle point
        // calculate the smallest distance dl on left of middle point and
        // dr on right side
        float dl = this->closestUtil(pxArrayPtr, pyl, mid);
        float dr = this->closestUtil(pxArrayPtr + mid, pyr, size-mid);
      
        // Find the smaller of two distances
        float d = USE_MINOR(dl, dr);
      
        // Build an array strip[] that contains points close (closer than d)
        // to the line passing through the middle point
        Point2D strip[size];
        int j = 0;
        for (int i = 0; i < size; i++)
            if (abs(GET_X(pyArrayPtr[i]) - GET_X(midPoint)) < d)
                strip[j] = pyArrayPtr[i], j++;
      
        // Find the closest points in strip.  Return the minimum of d and closest
        // distance is strip[]
        rv = USE_MINOR(d, stripClosest(strip, j, d) );
    }
 
    return rv;
}
 
int main()
{
 
    ClosestPair cp;
 
    cp.addPoint2D( 2  , 3  );
    cp.addPoint2D( 12 , 30 );
    cp.addPoint2D( 40 , 50 );    
    cp.addPoint2D( 5  , 1  );
    cp.addPoint2D( 12 , 10 );
    cp.addPoint2D( 3  , 4  );
 
    std::cout << "The smallest distance is " << cp.searchClosest() << std::endl;
 
    return 0;
}
