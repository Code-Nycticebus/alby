#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../src/lexer/lexer.h"
#include "../../src/lexer/token.h"

#include "../../src/interpreter/cpu/cpu.h"
#include "../../src/interpreter/cpu/cpu_inst.h"

#define PROGRAM_MAX 100

typedef struct ParserError {
  const char *msg;
  size_t line;
  const char *bol;
  Token tk;
} ParserError;

typedef struct ParserResult {
  bool is_ok;
  union {
    ParserError err;
    CpuInstruction ok;
  } res;
} ParserResult;

static void _display_line(const char *bol) {
  for (const char *c = bol; *c != '\0' && *c != '\n'; ++c) {
    putchar(*c);
  }
  putchar('\n');
}

static void parser_error_display(ParserError *err) {
  printf("src/from_memory.ergot:%ld:\n", err->line);
  printf("%s\n", err->msg);
  _display_line(err->bol);
  printf("%*c%*c\n", (int)(err->tk.token - err->bol), ' ', (int)err->tk.len,
         '^');
}

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

  if (!(TOKEN_REGISTER_1 <= op1.kind && op1.kind <= TOKEN_REGISTER_8)) {
    ParserError err = {
        .msg = "The first operant has to be a register.",
        .tk = op1,
        .bol = &lexer->content[lexer->bol],
        .line = lexer->line,
    };
    parser_error_display(&err);
    exit(1);
  }

  Token comma = lexer_next(lexer);
  assert(comma.kind == TOKEN_DEL_COMMA && "Missing comma delimiter");
  Token op2 = lexer_next(lexer);
  if (op2.kind == TOKEN_LIT_I64) {
    return cpu_inst_movi(op1.kind - TOKEN_REGISTER_1, parse_ui64(op2));
  }
  return cpu_inst_exit(1);
}

static CpuInstruction parse_add(Lexer *lexer) {
  Token reg = lexer_next(lexer);
  assert(TOKEN_REGISTER_1 <= reg.kind && reg.kind <= TOKEN_REGISTER_SB &&
         "First operant has to be a register");
  if (lexer_next(lexer).kind != TOKEN_DEL_COMMA) {
    assert(0 && "Missing comma delimiter");
  }
  // Number or Register
  Token second = lexer_next(lexer);
  if (second.kind == TOKEN_LIT_I64) {
    return cpu_inst_addi(reg.kind - TOKEN_REGISTER_1, parse_ui64(second));
  }
  return cpu_inst_exit(1);
}

static int parser(size_t len, const char *source_code, Program *program) {
  Lexer lexer = lexer_init(len, source_code);

  for (Token tk = lexer_next(&lexer); tk.kind != TOKEN_EOF;
       tk = lexer_next(&lexer)) {
    switch (tk.kind) {
    case TOKEN_MOV: {
      printf("PARSING MOV\n");
      CpuInstruction inst = parse_mov(&lexer);
      assert(inst.operation == CPU_OP_MOVI || inst.operation == CPU_OP_MOVR);
      program->inst[program->size++] = inst;
      break;
    }
    case TOKEN_OP_ADD: {
      CpuInstruction inst = parse_add(&lexer);
      assert(inst.operation == CPU_OP_ADDI || inst.operation == CPU_OP_ADDR);
      program->inst[program->size++] = inst;
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
  const char str[] = "mov r2, 34\n"
                     "add r2, 35\n"
                     "mov 3, 34\n";

  Program program = {0};

  if (parser(sizeof(str) - 1, str, &program)) {
    printf("Not succ compile!\n");
    return 1;
  }

  run_program(program.inst);

  return 0;
}
