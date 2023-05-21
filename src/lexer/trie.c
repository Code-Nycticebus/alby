#include "trie.h"

#include <assert.h>

#define TRIE_MAX_NODES 100

static struct {
  size_t top;
  TrieNode nodes[TRIE_MAX_NODES];
} trie_node_stack;

// Allocs a Node from the static trie_node_stack
static TrieNode *trie_stack_alloc_node(void) {
  assert(trie_node_stack.top < TRIE_MAX_NODES && "Too many Tree Nodes");
  return &trie_node_stack.nodes[trie_node_stack.top++];
}

bool trie_insert(TrieNode *root, size_t size, const char *str, Value value) {
  // Endpoint handling
  if (size == 0) {
    if (root->value == 0) {
      root->value = value;
      return true;
    }
    return true;
  }
  // Process next char
  assert(TRIE_CHAR_MIN <= *str && *str <= TRIE_CHAR_MAX &&
         "The char is not in range");
  // Translates index
  size_t index = (size_t)(*str - TRIE_CHAR_MIN); // TODO translate function

  // if no node is there get new node from the stack
  if (root->children[index] == NULL) {
    root->children[index] = trie_stack_alloc_node();
  }

  // Recursively traverse children until all slots are filled
  return trie_insert(root->children[index], size - 1, str + 1, value);
}

Value trie_search(const TrieNode *root, size_t size, const char *str) {
  // Endpoint handling
  if (size == 0) {
    return root->value;
  }

  // If the characters are not in range return false
  if (!(TRIE_CHAR_MIN <= *str && *str <= TRIE_CHAR_MAX)) {
    return 0;
  }
  size_t index = (size_t)(*str - TRIE_CHAR_MIN);

  if (root->children[index] == NULL) {
    return 0;
  }
  return trie_search(root->children[index], size - 1, str + 1);
}

TrieNode *trie_init(void) { return trie_stack_alloc_node(); }
