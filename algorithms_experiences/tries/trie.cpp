#include "tries.hpp"

#define __CHAR_NOT_FOUND_ON_CHILDREN(P,W)   P->children.find( W ) \
        == P->children.end()

static TriePtr
trie_incept()
{
    auto n_ptr = new Trie;
    if ( n_ptr ) n_ptr->is_whole_word = false;
    return n_ptr;
}

inline static int
trie_populate( TriePtr curr_ptr, const char * s_ptr )
{
    int rc = TRIE_STUFF_SUCCEED;

    for( ; *s_ptr ; s_ptr++ )
    {
        if ( __CHAR_NOT_FOUND_ON_CHILDREN( curr_ptr, *s_ptr ) )
        {
            TriePtr t_ptr = trie_incept();
            if ( t_ptr == nullptr )
                return TRIE_SYSTEM_ERROR;

            curr_ptr->children[ *s_ptr ] = t_ptr;
        }

        curr_ptr = curr_ptr->children[ *s_ptr ];
    }

    curr_ptr->is_whole_word = true;

    return rc;
}

extern int
trie_insert( TriePtr * head_ptr, const std::string word )
{
    int rc = TRIE_STUFF_SUCCEED;

    do {

        if ( head_ptr == nullptr )
        {
            rc = TRIE_PARAM_NULL_PTR;
            break;
        }

        if ( *head_ptr == nullptr )
        {
            if ( ( *head_ptr = trie_incept() ) == nullptr )
            {
                rc = TRIE_SYSTEM_ERROR;
                break;
            }
        }

        rc = trie_populate( *head_ptr, word.c_str() );

    } while (0);

    return rc;
}
