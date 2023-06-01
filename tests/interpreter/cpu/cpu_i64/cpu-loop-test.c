#include <assert.h>
#include <stdio.h>

#include "../../../../src/interpreter/cpu/cpu.h"
#include "../../../../src/interpreter/cpu/cpu_instructions.h"

const int64_t max_iterations = 0x1ff;

enum Variables {
  a = 0,
};

#define LABEL 5

int main(void) {
  Cpu cpu = {0};

  CpuInstruction program[] = {
      cpu_inst_i64_mov(CPU_R1, 0),              // size_t i = 0
      cpu_inst_i64_cmp(CPU_R1, max_iterations), // R1 < 10
      cpu_inst_jge(LABEL),                      // if (R1 < 10)
      cpu_inst_i64_add(CPU_R1, 0x1),            // i++
      cpu_inst_jmp(1),                          // jmp back

      cpu_inst_i64_pushr(CPU_R1), // size_t a = 0;

      // cpu_inst_debug(),
      // LABEL
      cpu_inst_exit(0),
  };

  cpu_run_program(&cpu, program, (sizeof(program) / sizeof(program[0])));
  assert(*(int64_t *)&cpu.stack[a] == max_iterations);

  return 0;
}
