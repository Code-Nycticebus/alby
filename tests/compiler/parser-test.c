#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler/parser.h"
#include "compiler/result.h"
#include "interpreter/cpu/cpu_defines.h"
#include "lexer/lexer.h"

#define NEXT(__parser_result, __parser)                                        \
  do {                                                                         \
    __parser_result = parser_next(&__parser);                                  \
    if (__parser.eof == true) {                                                \
      continue;                                                                \
    }                                                                          \
    if (__parser_result.ok == false) {                                         \
      parser_error_print(&__parser_result.value.err);                          \
      return -1;                                                               \
    }                                                                          \
  } while (0);

int main(void) {
  const char str[] = "mov r2, 34\n"
                     "add r2, 35\n";

  Parser p = {
      .filename = "memory",
      .lexer = lexer_init(sizeof(str), str),
      .eof = false,
  };
  ParserResult pr;

  NEXT(pr, p);
  assert(pr.value.ok.operation == CPU_OP_MOVI &&
         "Parser did not parse operation correctly");
  assert(pr.value.ok.value2 == 34 &&
         "Parser did not parse the integer correctly");

  NEXT(pr, p);
  assert(pr.value.ok.operation == CPU_OP_ADDI &&
         "Parser did not parse operation correctly");

  NEXT(pr, p);
  assert(p.eof == true && "EOF should be reached!");
  return 0;
}
