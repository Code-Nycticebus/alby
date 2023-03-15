#include "keywords.h"

#include <assert.h>
#include <stdio.h>

#include "lexer/token.h"
#include "trie.h"

// Gives you the len and str -> len, str
#define LEN_STR(str) (sizeof(str) - 1), str

static const struct {
  TokenKind kind;
  size_t len;
  const char *kw;
} keywords[] = {
    // Keywords

    {TOKEN_SYSCALL, LEN_STR("syscall")},
    {TOKEN_EXIT, LEN_STR("exit")},

    {TOKEN_MOV, LEN_STR("mov")},
    {TOKEN_PUSH, LEN_STR("push")},
    {TOKEN_POP, LEN_STR("pop")},

    {TOKEN_CMP, LEN_STR("cmp")},
    {TOKEN_JMP, LEN_STR("jmp")},
    {TOKEN_JE, LEN_STR("je")},
    {TOKEN_JL, LEN_STR("jl")},
    {TOKEN_JLE, LEN_STR("jle")},
    {TOKEN_JG, LEN_STR("jg")},
    {TOKEN_JGE, LEN_STR("jge")},
    // Operators

    {TOKEN_OP_ADD, LEN_STR("add")},
    {TOKEN_OP_SUB, LEN_STR("sub")},
    {TOKEN_OP_MUL, LEN_STR("mul")},
    {TOKEN_OP_DIV, LEN_STR("div")},

    // Registers
    {TOKEN_REGISTER_1, LEN_STR("r1")},
    {TOKEN_REGISTER_2, LEN_STR("r2")},
    {TOKEN_REGISTER_3, LEN_STR("r3")},
    {TOKEN_REGISTER_4, LEN_STR("r4")},
    {TOKEN_REGISTER_5, LEN_STR("r5")},
    {TOKEN_REGISTER_6, LEN_STR("r6")},
    {TOKEN_REGISTER_7, LEN_STR("r7")},
    {TOKEN_REGISTER_8, LEN_STR("r8")},
    {TOKEN_REGISTER_SP, LEN_STR("rsp")},
    {TOKEN_REGISTER_SB, LEN_STR("rbp")},

    {TOKEN_DEBUG, LEN_STR("debug")},
};

#undef LEN_STR
static const size_t keyword_count = sizeof(keywords) / sizeof(keywords[0]);

TrieNode *keyword_trie = NULL;

void keywords_init(void) {
  // TODO convert it to a singleton instead of a static class
  if (keyword_trie != NULL) {
    return;
  }
  keyword_trie = trie_init();

  for (size_t i = 0; i < keyword_count; i++) {
    trie_insert(keyword_trie, keywords[i].len, keywords[i].kw,
                keywords[i].kind);
  }

#if 0
    printf("Keywords initialized: %ld keywords | %%ld/%ld Nodes | %ld bytes\n", keyword_count,
         keyword_trie.stack.top, keyword_trie.stack.size, sizeof(nodes));
#endif /* if 0 */
}

TokenKind keywords_get_token(size_t size, const char *str) {
  return (TokenKind)trie_search(keyword_trie, size, str);
}
