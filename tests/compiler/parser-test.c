#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../src/compiler/parser.h"

#include "../../src/lexer/lexer.h"
#include "../../src/lexer/token.h"

#include "../../src/interpreter/cpu/cpu.h"
#include "../../src/interpreter/cpu/cpu_inst.h"

#define PROGRAM_MAX 100

typedef struct Program {
  size_t size;
  CpuInstruction inst[PROGRAM_MAX];
} Program;

static uint64_t parse_ui64(Token tk) {
  assert(tk.kind == TOKEN_LIT_I64);
  const int base = 10;
  return strtoul(tk.token, NULL, base);
}

static CpuInstruction parse_mov(Lexer *lexer) {
  Token op1 = lexer_next(lexer);

  printf("TOKEN: %.*s -> %s\n", (int)op1.len, op1.token,
         token_kind_to_string(op1.kind));
  assert(TOKEN_REGISTER_1 <= op1.kind && op1.kind <= TOKEN_REGISTER_SB &&
         "First operant has to be a register");
  Token comma = lexer_next(lexer);
  assert(comma.kind == TOKEN_DEL_COMMA && "Missing comma delimiter");
  Token op2 = lexer_next(lexer);
  if (op2.kind == TOKEN_LIT_I64) {
    return cpu_inst_movi(op1.kind - TOKEN_REGISTER_1, parse_ui64(op2));
  }
  if (TOKEN_REGISTER_1 <= op2.kind && op2.kind <= TOKEN_REGISTER_8) {
    return cpu_inst_movr(op1.kind - TOKEN_REGISTER_1,
                         op2.kind - TOKEN_REGISTER_1);
  }
  return cpu_inst_exit(1);
}

static CpuInstruction parse_sub(Lexer *lexer) {
  Token reg = lexer_next(lexer);
  assert(TOKEN_REGISTER_1 <= reg.kind && reg.kind <= TOKEN_REGISTER_SB &&
         "first operant has to be a register");
  Token comma = lexer_next(lexer);
  assert(comma.kind == TOKEN_DEL_COMMA && "Comma missing");
  Token second = lexer_next(lexer);
  if (second.kind == TOKEN_LIT_I64) {
    return cpu_inst_subi(reg.kind - TOKEN_REGISTER_1, parse_ui64(second));
  }
  if (TOKEN_REGISTER_1 <= second.kind && second.kind <= TOKEN_REGISTER_SB) {
    return cpu_inst_subr(reg.kind - TOKEN_REGISTER_1,
                         second.kind - TOKEN_REGISTER_1);
  }
  return cpu_inst_exit(1);
}

static int parser(size_t len, const char *source_code, Program *program) {
  Lexer lexer = lexer_init(len, source_code);
  for (Token tk = lexer_next(&lexer); tk.kind != TOKEN_EOF;
       tk = lexer_next(&lexer)) {
    printf("(%s): %.*s\n", token_kind_to_string(tk.kind), (int)tk.len,
           tk.token);

    switch (tk.kind) {
    case TOKEN_MOV: {
      printf("PARSING MOV\n");
      CpuInstruction inst = parse_mov(&lexer);
      assert(inst.operation == CPU_OP_MOVI || inst.operation == CPU_OP_MOVR);
      program->inst[program->size++] = inst;
      break;
    }
    case TOKEN_OP_SUB: {
      CpuInstruction inst = parse_sub(&lexer);
      assert(inst.operation == CPU_OP_SUBI || inst.operation == CPU_OP_SUBR);
      program->inst[program->size++] = inst;
      break;
    }
    case TOKEN_DEL_NEWLINE:
      break;
    default:
      assert(0 && "Unexpected token!");
      break;
    }
  }
  program->inst[program->size++] = cpu_inst_debug();
  program->inst[program->size] = cpu_inst_exit(0);
  return 0;
}

static int run_program(CpuInstruction *program) {
  Cpu cpu = {0};
  return cpu_run_program(&cpu, program);
}

int main(void) {
  const char str[] = "mov r1, 489\n"
                     "mov r2, 34\n"
                     "sub r1, r2\n"
                     "sub r1, 35\n";

  Program program = {0};

  if (parser(sizeof(str) - 1, str, &program)) {
    printf("Not succ compile!\n");
    return 1;
  }

  run_program(program.inst);
  return 0;
}
