#include <assert.h>
#include <stdio.h>

#include "../../../src/interpreter/cpu/cpu.h"
#include "../../../src/interpreter/cpu/cpu_instructions.h"

int main(void) {
  Cpu cpu = {0};

  CpuInstruction program[] = {
      cpu_inst_i8_mov(CPU_R1, 127),
      cpu_inst_debug(),
      cpu_inst_exit(0),
  };

  cpu_run_program(&cpu, program, (sizeof(program) / sizeof(program[0])));

  return 0;
}
