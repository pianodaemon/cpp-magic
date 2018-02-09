/*
Algorithm "Union Find with weighted union"

https://en.wikipedia.org/wiki/Disjoint-set_data_structure

Applications:

1) It is used to determine the connected components in a graph.
We can determine whether 2 nodes are in the same connected component
or not in the graph. We can also determine that by adding an edge
between 2 nodes whether it leads to cycle in the graph or not.

2) It is used to determine the cycles in the graph.

code writter: pianodaemon
mailto: j4nusx@yahoo.com
*/

#include <vector>
#include <iostream>

#define __INITIAL_SUBSET_SIZE 1

class union_find
{
    public:
        union_find( const size_t l );
        virtual ~union_find();
        bool do_find( const unsigned int a, const unsigned int b );
        void do_union( const unsigned int a, const unsigned int b );

    protected:
        inline unsigned int seek_root( unsigned int i );

        std::vector<int> m_v;
        std::vector<int> m_s;
};

union_find::union_find( const size_t l ) : m_v(l), m_s(l, __INITIAL_SUBSET_SIZE)
{
    /* Initially there are N subsets containing
       single element in each subset. The initial
       size of each subset will be one */
    unsigned int c = 0;
    for ( int& i : this->m_v ) i = c++;
}

union_find::~union_find() { }

unsigned int
union_find::seek_root( unsigned int i )
{
    /* The 'root element' of each subset,
       which is a only special element in
       that subset having itself as the parent. */

    while( this->m_v[i] != i )
    {
       /* during the quest, an interesting optimization
          takes place "path compression" by setting
          each i to point to its grandparent */
       this->m_v[i] = this->m_v[ this->m_v[i] ];

       i = this->m_v[i];
    }
    return i;
}

bool
union_find::do_find( const unsigned int a, const unsigned int b )
{
    /* if 'a' and 'b' have same root,
       means they are connected. */
    return ( this->seek_root( a ) == this->seek_root( b ) ) ? true : false;
}

void
union_find::do_union( const unsigned int a, const unsigned int b )
{
    auto weighted = [&]( const unsigned int& root_l, const unsigned int& root_r )
    {
        this->m_v[root_l] = this->m_v[root_r];
        this->m_s[root_r] += this->m_s[root_l];
    };

    auto root_a = this->seek_root( a );
    auto root_b = this->seek_root( b );

    /* connectting two subsets according to the number
       of elements in subset ("balance by linking root
       of smaller subset to the root of larger subset").
       Such strategy allows us to avoid tall subsets */
    if ( this->m_s[root_a] < this->m_s[root_b] ) weighted( root_a, root_b );
    else weighted( root_b, root_a );
}

int main()
{
    /* An small test case */
    auto uf = union_find(7);

    uf.do_union(1, 2);
    std::cout << ("union 1 2") << std::endl;

    uf.do_union(3, 4);
    std::cout << ("union 3 4") << std::endl;

    uf.do_union(1, 0);
    std::cout << ("union 1 0") << std::endl;

    uf.do_union(1, 3);
    std::cout << ("union 1 3") << std::endl;

    if ( uf.do_find(6, 4) )
    {
        std::cout << "There's connection" << std::endl;
    }
    else
    {
        std::cout << "There is not connection" << std::endl;
    }

    return 0;
}
