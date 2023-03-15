#include "result.h"

#include "lexer/lexer.h"

#include <assert.h>
#include <stdio.h>

static void _print_repeat(const char c, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    putchar(c);
  }
}

static void _print_ln(const char *bol) {
  for (const char *c = bol; *c != '\0' && *c != '\n'; ++c) {
    putchar(*c);
  }
  putchar('\n');
}

void parser_error_print(const ParserError *err) {

  printf("%s:%ld:\n\n", err->file, err->line);
  _print_ln(err->bol);
  assert(err->tk.token >= err->bol && "Token is not on same line as bol");
  _print_repeat(' ', (size_t)(err->tk.token - err->bol));
  _print_repeat('^', err->tk.len);
  putchar('\n');
  putchar('\n');

  printf("%s: %s\n", token_kind_to_string(err->tk.kind), err->msg);
}
