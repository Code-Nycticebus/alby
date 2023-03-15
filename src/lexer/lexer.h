#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "token.h"

typedef struct Lexer {
  const char *content;
  size_t size;

  size_t cursor;
  size_t line;
  size_t bol;
} Lexer;

const char *token_kind_to_string(TokenKind kind);

bool token_is_register(TokenKind kind);

// Creates lexer generator that wraps around the content
Lexer lexer_init(size_t size, const char *content);

// consumes the interator and steps forward by one
Token lexer_next(Lexer *lexer);
