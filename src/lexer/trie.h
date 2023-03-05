#ifndef _TRIE_H
#define _TRIE_H

/* Kind of a static const Trie datastructure to load and search for keywords */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define TRIE_CHAR_MIN '0'
#define TRIE_CHAR_MAX 'z'
#define TRIE_MAX_CHARS (TRIE_CHAR_MAX - TRIE_CHAR_MIN)

typedef uint64_t Value;

typedef struct TrieNode {
  struct TrieNode *children[TRIE_MAX_CHARS];
  Value value;
} TrieNode;

// This allocates a root node from a static stack
TrieNode *trie_init(void);

// Inserts node. Gets new nodes from a static node stack
bool trie_insert(TrieNode *root, size_t size, const char *str, Value value);

// Searches in the trie
Value trie_search(const TrieNode *root, size_t size, const char *str);

#endif // !_TRIE_H
