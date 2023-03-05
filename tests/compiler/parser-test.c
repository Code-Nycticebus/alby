#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../src/compiler/parser.h"

// #define PROGRAM_MAX 100
// static CpuInstruction program_buffer[PROGRAM_MAX];
// static size_t program_inst_count = 0;

#include "../../src/lexer/lexer.h"
#include "../../src/lexer/token.h"

int main(void) {
  // const char str[] = "mov r1, 500\n"
  //                    "mov r2, 80\n"
  //                    "sub r1, r2\n";
  // Lexer lexer = lexer_init(sizeof(str) - 1, str);
  // for (Token tk = lexer_next(&lexer); tk.kind != TOKEN_EOF;
  //      tk = lexer_next(&lexer)) {
  //   printf("(%s): %.*s\n", token_kind_to_string(tk.kind), (int)tk.len,
  //          tk.token);

  //   switch (tk.kind) {
  //   case TOKEN_MOV: {
  //     Token op1 = lexer_next(&lexer);
  //     assert(TOKEN_REGISTER_1 <= op1.kind && op1.kind <= TOKEN_REGISTER_SB &&
  //            "First operant has to be a register");
  //     Token comma = lexer_next(&lexer);
  //     assert(comma.kind == TOKEN_DEL_COMMA && "Missing comma delimiter");
  //     Token op2 = lexer_next(&lexer);
  //     if (op2.kind == TOKEN_LIT_I64) {
  //       // TODO figure out what base the toke is
  //       const int base = 10;
  //       uint64_t res = strtoul(op2.token, NULL, base);
  //       program_buffer[program_inst_count++] =
  //           cpu_inst_movi(op1.kind - TOKEN_REGISTER_1, res);
  //     }
  //     break;
  //   }
  //   case TOKEN_OP_SUB: {
  //     Token reg = lexer_next(&lexer);
  //     assert(TOKEN_REGISTER_1 <= reg.kind && reg.kind <= TOKEN_REGISTER_SB &&
  //            "first operant has to be a register");
  //     Token comma = lexer_next(&lexer);
  //     assert(comma.kind == TOKEN_DEL_COMMA && "Comma missing");
  //     Token second = lexer_next(&lexer);
  //     if (second.kind == TOKEN_LIT_I64) {
  //       const int base = 10;
  //       uint64_t res = strtoul(second.token, NULL, base);
  //       program_buffer[program_inst_count++] =
  //           cpu_inst_subi(reg.kind - TOKEN_REGISTER_1, res);
  //     } else if (TOKEN_REGISTER_1 <= second.kind &&
  //                second.kind <= TOKEN_REGISTER_SB) {
  //       program_buffer[program_inst_count++] = cpu_inst_subr(
  //           reg.kind - TOKEN_REGISTER_1, second.kind - TOKEN_REGISTER_1);
  //     } else {
  //       assert(0 && "Subi wubi fucki wucky");
  //     }
  //     break;
  //   }
  //   case TOKEN_DEL_NEWLINE:
  //     break;
  //   default:
  //     assert(0 && "Unexpected token!");
  //     break;
  //   }
  // }
  // program_buffer[program_inst_count++] = cpu_inst_debug();
  // program_buffer[program_inst_count++] = cpu_inst_movi(CPU_R1, 0);
  // program_buffer[program_inst_count] = cpu_inst_syscall(SYS_exit);

  // Cpu cpu = {0};
  // cpu_run_program(&cpu, program_buffer);

  return 0;
}
