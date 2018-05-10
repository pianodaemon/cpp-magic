#include "tries.hpp"

#define __CHAR_FOUND_ON_CHILDREN(P,W)       P->children.find( W ) \
        != P->children.end()
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
            rc = TRIE_PARAM_NULL_PTR_ERROR;
            break;
        }

        if ( word.empty() )
        {
            rc = TRIE_NOT_VALID_WORD_ERROR;
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


extern int
trie_search( TriePtr head_ptr, const std::string word )
{
    int rc = TRIE_STUFF_SUCCEED;

    if ( head_ptr == nullptr )
    {
        rc = TRIE_PARAM_NULL_PTR_ERROR;

        if ( word.empty() )
        {
            rc = TRIE_NOT_VALID_WORD_ERROR;
        }
    }
    else
    {
        auto curr_ptr = head_ptr;

        for( const char& c : word )
        {
            if ( ( curr_ptr = curr_ptr->children[ c ] ) == nullptr )
            {
                rc = TRIE_NOT_FOUND_WORD_ERROR;
                break;
            }
        }

        if ( !curr_ptr->is_whole_word ) rc = TRIE_NOT_FOUND_WORD_ERROR;
    }

    return rc;
}

static int
trie_empty( TriePtr const curr_ptr )
{
    for ( auto it : curr_ptr->children )
    {
        if ( it.second != nullptr ) return TRIE_NOT_EMPTY_ERROR;
    }

    return TRIE_STUFF_SUCCEED;
}


static bool
trie_erase( TriePtr& curr_ptr, const char * s_ptr )
{
    auto attempt_destroy = [&]()->bool
    {
        if ( trie_empty( curr_ptr ) == TRIE_STUFF_SUCCEED )
        {
            delete curr_ptr;
            curr_ptr = nullptr;
            return true;
        }

        return false;
    };

    const char& c = *s_ptr;

    if ( c && ( __CHAR_FOUND_ON_CHILDREN( curr_ptr, c ) ) )
    {
        auto res = trie_erase( curr_ptr->children[ c ], s_ptr + 1 );

        if ( res && ( !curr_ptr->is_whole_word ) )
            return attempt_destroy();
    }

    if ( curr_ptr->is_whole_word && ( c == 0 ) )
    {
        if ( attempt_destroy() )
            return true;

        curr_ptr->is_whole_word = false;
    }

    return false;
}


extern int
trie_delete( TriePtr * start_ptr, const std::string word )
{
    int rc = TRIE_STUFF_SUCCEED;

    do {

        if ( start_ptr == nullptr )
        {
            rc = TRIE_PARAM_NULL_PTR_ERROR;
            break;
        }

        if ( word.empty() )
        {
            rc = TRIE_NOT_VALID_WORD_ERROR;
            break;
        }

        if ( *start_ptr == nullptr )
        {
            rc = TRIE_PARAM_NULL_PTR_ERROR;
            break;
        }

        if ( trie_erase( *start_ptr, word.c_str() ) )
            break;
        else
            rc = TRIE_NOT_FOUND_WORD_ERROR;

    } while (0);

    return rc;
}
