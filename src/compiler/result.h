#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "interpreter/cpu/cpu_inst.h"
#include "lexer/token.h"

typedef struct ParserError {
  const char *file;
  const char *msg;
  size_t line;
  const char *bol;
  Token tk;
} ParserError;

typedef struct ParserResult {
  bool ok;
  union {
    ParserError err;
    CpuInstruction ok;
  } value;
} ParserResult;

#define ResultOk(...)                                                          \
  (ParserResult) { .ok = true, .value.ok = (__VA_ARGS__) }

#define ResultErr(...)                                                         \
  (ParserResult) { .ok = false, .value.err = (__VA_ARGS__) }

void parser_error_print(const ParserError *err);
