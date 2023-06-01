#pragma once

#include <stddef.h>

#include "result.h"

#include "interpreter/cpu/cpu_instructions.h"
#include "lexer/lexer.h"

typedef struct Parser {
  bool eof;

  const char *filename;
  Lexer lexer;
} Parser;

ParserResult parser_next(Parser *parser);
