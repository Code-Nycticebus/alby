#include "lexer/token.h"
#include "lexer/trie.h"

#define SUFFIX "suffix"
#define SUFFIX_SIZE (sizeof(SUFFIX) - 1)

// For concatinating the string literal and SUFFIX while also getting the real
// len from the keyword
#define STR_ARGUMENT(str) ((sizeof(str) / sizeof(str[0])) - 1), str SUFFIX

static struct {
  TokenKind kind;
  const size_t size;
  const char *keyword;
} keywords[] = {
    {TOKEN_PUSH, STR_ARGUMENT("push")},
    {TOKEN_OP_ADD, STR_ARGUMENT("add")},
};

#undef STR_ARGUMENT

const size_t keyword_count = sizeof(keywords) / sizeof(keywords[0]);

#include <assert.h>
int main(void) {
#define NODE_COUNT 10
  TrieNode *keyword_trie = trie_init();

  for (size_t i = 0; i < keyword_count; ++i) {
    assert(trie_insert(keyword_trie, keywords[i].size, keywords[i].keyword,
                       keywords[i].kind) &&
           "Could not insert correctly");
  }

  for (size_t i = 0; i < keyword_count; i++) {
    assert((TokenKind)trie_search(keyword_trie, keywords[i].size,
                                  keywords[i].keyword) == keywords[i].kind &&
           "Did not search the correct value");

    assert(
        !trie_search(keyword_trie, keywords[i].size - 1, keywords[i].keyword) &&
        "Should not find anything when searching for substring");
    assert(!trie_search(keyword_trie, keywords[i].size + SUFFIX_SIZE,
                        keywords[i].keyword) &&
           "Should not find anything when searched is substring");
  }
  return 0;
}
