#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler/parser.h"
#include "compiler/result.h"
#include "interpreter/cpu/cpu_defines.h"
#include "lexer/lexer.h"

#define BIG_ASS_NUMBER 0xAABBCCDDEEFF

#define _str(x) #x
#define STR(x) _str(x)

#define NEXT(__parser_result, __parser)                                        \
  do {                                                                         \
    *(__parser_result) = parser_next(__parser);                                \
    if (__parser_result->ok == false) {                                        \
      parser_error_print(&__parser_result->value.err);                         \
      return -1;                                                               \
    }                                                                          \
  } while (0);

#define TEST_MOV_STR                                                           \
  "mov r2, 34\n"                                                               \
  "mov r1, r2\n"

static int test_mov(Parser *p, ParserResult *pr) {
  NEXT(pr, p);
  assert(pr->value.ok.operation == CPU_OP_MOVI &&
         "Parser did not parse operation correctly");
  assert(pr->value.ok.value2.i64 == 34 &&
         "Parser did not parse the integer correctly");
  NEXT(pr, p);
  assert(pr->value.ok.operation == CPU_OP_MOVR &&
         "Parser did not parse correctly");
  assert(pr->value.ok.value1.r == CPU_R1 && "Parser parsed wrong register!");
  assert(pr->value.ok.value2.r == CPU_R2 &&
         "Parser parsed wrong second register");
  return 0;
}

#define TEST_ADD_STR                                                           \
  "add r2, [64] 35\n"                                                          \
  "add r1, r2\n"

static int test_add(Parser *p, ParserResult *pr) {
  NEXT(pr, p);
  assert(pr->value.ok.operation == CPU_OP_ADDI &&
         "Parser did not parse operation correctly");
  assert(pr->value.ok.value2.i64 == 35 &&
         "Parser did not parse the integer correctly");
  NEXT(pr, p);
  assert(pr->value.ok.operation == CPU_OP_ADDR &&
         "Parser did not parse operation correctly");
  assert(pr->value.ok.value1.r == CPU_R1 && "Parser parsed wrong register!");
  assert(pr->value.ok.value2.r == CPU_R2 &&
         "Parser parsed wrong second register");
  return 0;
}

#define TEST_SUB_STR                                                           \
  "sub r1, 69\n"                                                               \
  "sub r1, r2\n"

static int test_sub(Parser *p, ParserResult *pr) {
  NEXT(pr, p);
  assert(pr->value.ok.operation == CPU_OP_SUBI &&
         "Parser did not parse operation correctly");
  assert(pr->value.ok.value2.i64 == 69 &&
         "Parser did not parse the integer correctly");
  NEXT(pr, p);
  assert(pr->value.ok.operation == CPU_OP_SUBR &&
         "Parser did not parse operation correctly");
  assert(pr->value.ok.value1.r == CPU_R1 && "Parser parsed wrong register!");
  assert(pr->value.ok.value2.r == CPU_R2 &&
         "Parser parsed wrong second register");
  return 0;
}
#define TEST_MUL_STR                                                           \
  "mul r1, 69\n"                                                               \
  "mul r1, r2\n"

static int test_mul(Parser *p, ParserResult *pr) {
  NEXT(pr, p);
  assert(pr->value.ok.operation == CPU_OP_MULI &&
         "Parser did not parse operation correctly");
  assert(pr->value.ok.value2.i64 == 69 &&
         "Parser did not parse the integer correctly");
  NEXT(pr, p);
  assert(pr->value.ok.operation == CPU_OP_MULR &&
         "Parser did not parse operation correctly");
  assert(pr->value.ok.value1.r == CPU_R1 && "Parser parsed wrong register!");
  assert(pr->value.ok.value2.r == CPU_R2 &&
         "Parser parsed wrong second register");
  return 0;
}

#define TEST_DIV_STR                                                           \
  "div r1, 69\n"                                                               \
  "div r1, r2\n"

static int test_div(Parser *p, ParserResult *pr) {
  NEXT(pr, p);
  assert(pr->value.ok.operation == CPU_OP_DIVI &&
         "Parser did not parse operation correctly");
  assert(pr->value.ok.value2.i64 == 69 &&
         "Parser did not parse the integer correctly");
  NEXT(pr, p);
  assert(pr->value.ok.operation == CPU_OP_DIVR &&
         "Parser did not parse operation correctly");
  assert(pr->value.ok.value1.r == CPU_R1 && "Parser parsed wrong register!");
  assert(pr->value.ok.value2.r == CPU_R2 &&
         "Parser parsed wrong second register");
  return 0;
}

#define TEST_PUSH_STR                                                          \
  "push r1\n"                                                                  \
  "push " STR(BIG_ASS_NUMBER) "\n"

static int test_push(Parser *p, ParserResult *pr) {
  NEXT(pr, p);
  assert(pr->value.ok.operation == CPU_OP_PUSHR &&
         "Parser did not parse operation correctly");
  assert(pr->value.ok.value1.r == CPU_R1 && "Parser parsed wrong register!");
  NEXT(pr, p);
  assert(pr->value.ok.operation == CPU_OP_PUSHI &&
         "Parser did not parse operation correctly");
  assert(pr->value.ok.value1.i64 == BIG_ASS_NUMBER &&
         "Did not parse the big number correctly");
  return 0;
}

#define TEST_JMP_STR                                                           \
  "jmp 0\n"                                                                    \
  "je 1\n"                                                                     \
  "jl 2\n"                                                                     \
  "jle 3\n"                                                                    \
  "jg 4\n"                                                                     \
  "jge 5\n"

static int test_jmp(Parser *p, ParserResult *pr) {
  CpuOp ops[] = {
      CPU_OP_JMP, CPU_OP_JE, CPU_OP_JL, CPU_OP_JLE, CPU_OP_JG, CPU_OP_JGE,
  };

  size_t s = sizeof(ops) / sizeof(ops[0]);

  for (size_t i = 0; i < s; ++i) {
    NEXT(pr, p);
    assert(pr->value.ok.operation == ops[i] &&
           "Parser did not parse operation correctly");
  }

  return 0;
}

#define TEST(test_fn, __parser, __parser_result)                               \
  do {                                                                         \
    if (test_fn(__parser, __parser_result)) {                                  \
      return -1;                                                               \
    }                                                                          \
  } while (0);

const char str[] = TEST_MOV_STR TEST_ADD_STR TEST_SUB_STR TEST_MUL_STR
    TEST_DIV_STR TEST_PUSH_STR TEST_JMP_STR;

static int test_math(Parser *p, ParserResult *pr) {
  TEST(test_add, p, pr);
  TEST(test_sub, p, pr);
  TEST(test_mul, p, pr);
  TEST(test_div, p, pr);
  return 0;
}

int main(void) {
  Parser p = {
      .filename = "memory",
      .lexer = lexer_init(sizeof(str) - 1, str),
      .eof = false,
  };
  ParserResult pr;

  TEST(test_mov, &p, &pr);
  TEST(test_math, &p, &pr);
  TEST(test_push, &p, &pr);
  TEST(test_jmp, &p, &pr);

  NEXT((&pr), (&p));
  assert(p.eof == true && "EOF should be reached!");
  return 0;
}
