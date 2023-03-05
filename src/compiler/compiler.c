#include "compiler.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FOR TESTING
#include "../interpreter/cpu/cpu_inst.h"

int compile(const char *filename) {
  FILE *input = fopen(filename, "rb");
  if (input == NULL) {
    fprintf(stderr, "Error: Could not open file '%s': %s\n", filename,
            strerror(errno));
    exit(1);
  }
  fclose(input);

  enum Variables {
    a = 0,
    b,
  };
  const uint32_t max_iterations = 1010;

  const uint32_t loop_start = 3;
  const uint32_t loop_end = 12;

  CpuInstruction program[] = {
      cpu_inst_pushi(0), // mov a, 0;
      cpu_inst_pushi(1), // mov b, 1;

      cpu_inst_movi(CPU_R1, 0),              // mov r1, 0
      cpu_inst_cmpi(CPU_R1, max_iterations), // cmp r1, max_iterations
                                             // :loop_start
      cpu_inst_jge(loop_end),                // jge loop_end

      cpu_inst_movs(CPU_R2, a),      // mov r2, a
      cpu_inst_movs(CPU_R3, b),      // mov r3, b
      cpu_inst_addr(CPU_R2, CPU_R3), // add r2, r3

      cpu_inst_smovr(b, CPU_R2), // mov b, r2
      cpu_inst_smovr(a, CPU_R3), // mov a, r3
      cpu_inst_addi(CPU_R1, 1),  // add r1, 1
      cpu_inst_jmp(loop_start),  // jmp loop_start
                                 // :loop_end

      cpu_inst_pushr(CPU_R2), // push r2 # Pushes result on the stack
      cpu_inst_debug(),
      cpu_inst_exit(0),
  };
  const size_t program_size = sizeof(program) / sizeof(program[0]);

  FILE *output = fopen("main.lsd", "wb");
  if (output == NULL) {
    fprintf(stderr, "Error: Could not open file '%s': %s\n", "main.lsd",
            strerror(errno));
    exit(1);
  }

  fprintf(output, "#!./.build/bin/alby\n");
  (void)fwrite(program, sizeof(CpuInstruction), program_size, output);

  fclose(output);
  return 0;
}
