#ifndef _TOKEN_H
#define _TOKEN_H

#include <stddef.h>

typedef enum TokenKind {
  TOKEN_EOF,

  /* === Text based Tokens === */
  TOKEN_SYMBOL,
  // Keywords
  TOKEN_SYSCALL,
  TOKEN_EXIT,

  TOKEN_MOV,
  TOKEN_SMOV,
  TOKEN_PUSH,
  TOKEN_POP,

  TOKEN_CMP,
  TOKEN_JMP,
  TOKEN_JE,
  TOKEN_JL,
  TOKEN_JLE,
  TOKEN_JG,
  TOKEN_JGE,

  /* === Literal types === */
  TOKEN_LIT_I64,

  /* === Operators === */
  TOKEN_OP_ADD,
  TOKEN_OP_SUB,
  TOKEN_OP_MUL,
  TOKEN_OP_DIV,

  /* === Delimiters === */
  TOKEN_DEL_SEMICOLON,
  TOKEN_DEL_COLON,
  TOKEN_DEL_QUOT_DUB,
  TOKEN_DEL_QUOT_SIN,
  TOKEN_DEL_HASH,

  TOKEN_DEL_NEWLINE,

  TOKEN_DEL_DOT,
  TOKEN_DEL_COMMA,
  TOKEN_DEL_EQ,
  TOKEN_DEL_PIPE,

  TOKEN_DEL_LPAREN,
  TOKEN_DEL_RPAREN,
  TOKEN_DEL_LCURLY,
  TOKEN_DEL_RCURLY,
  TOKEN_DEL_LSQUARE,
  TOKEN_DEL_RSQUARE,

  /* Registers */
  TOKEN_REGISTER_1,
  TOKEN_REGISTER_2,
  TOKEN_REGISTER_3,
  TOKEN_REGISTER_4,
  TOKEN_REGISTER_5,
  TOKEN_REGISTER_6,
  TOKEN_REGISTER_7,
  TOKEN_REGISTER_8,
  TOKEN_REGISTER_SP,
  TOKEN_REGISTER_SB,

  TOKEN_DEBUG,

  TOKEN_COMMENT,
  TOKEN_INVALID = -1,
} TokenKind;

typedef struct Token {
  TokenKind kind;

  const char *token;
  size_t len;
} Token;

#endif // !_TOKEN_H
