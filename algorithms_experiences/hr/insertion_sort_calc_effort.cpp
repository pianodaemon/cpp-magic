#include <iostream>
#include <vector>
#include <set>

#define PRIOR_SHELF(S) S-1

static void
insertion_sort( std::vector<int>& arr )
{
    int j;
    for ( auto i = 0; i < arr.size(); i++ )
    {
        j = i;
        while ( j > 0 && ( arr[ j ] < arr[ PRIOR_SHELF( j ) ] ) )
        {
            std::swap( arr[ j ], arr[ PRIOR_SHELF( j ) ] );
            j--;
        }
    }
}

class sortable
{
    public:
        unsigned int m_pos;
        int m_value;

    public:
        sortable() : m_pos(0), m_value(0)
       	{
       	    // nothing to do yet
       	}

       	void setup( const unsigned int p, const int v )
       	{
       	     this->m_pos = p;
             this->m_value = v; 
       	}

      	virtual	~sortable() {}

       	bool operator <	( const sortable& other ) const
       	{
       	    if ( other.m_value != this->m_value )
            {
                return ( other.m_value > this->m_value );
            }
            else
            {
                return ( other.m_pos > this->m_pos );
            }
       	}
};


static size_t
calc_effort( std::vector<sortable>& arr )
{
    size_t ev = 0;
    std::set<sortable> s( arr.begin(), arr.end() );
    for ( auto& e : arr )
    {
        auto it = s.find(e);
        ev += std::distance( s.begin(), it );
        s.erase(it);
    }

    return ev;
}

int main()
{

    std::vector<int> x( { 1, 1, 1, 2, 2 } );
    std::vector<sortable> v( x.size() );

    for( unsigned int i = 0; i < v.size(); i++ )
    {
        v[i].setup( i, x[i] );
    }

    std::cout << calc_effort( v ) << std::endl;
    return 0;
}
