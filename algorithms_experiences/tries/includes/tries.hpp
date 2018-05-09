#ifndef __TRIE_DATA_STRUCTURE_HPP__
#define __TRIE_DATA_STRUCTURE_HPP__

#include <unordered_map>

#define TRIE_STUFF_SUCCEED                 0
#define TRIE_SYSTEM_ERROR                 -1
#define TRIE_PARAM_NULL_PTR_ERROR         -6001
#define TRIE_NOT_FOUND_WORD_ERROR         -6002
#define TRIE_NOT_VALID_WORD_ERROR         -6003

struct Trie
{
    bool is_whole_word;
    std::unordered_map< char, Trie* > children;
};


typedef struct Trie * TriePtr;


extern int
trie_insert( TriePtr *, const std::string );

extern int
trie_search( TriePtr, const std::string );

#endif /*  __TRIE_DATA_STRUCTURE_HPP__ */
