#include <assert.h>
#include <stdio.h>

#include "../../../src/interpreter/cpu/cpu.h"
#include "../../../src/interpreter/cpu/cpu_inst.h"

const Word max_iterations = 100;

enum Variables {
  a = 0,
};

#define LABEL 5

int main(void) {
  Cpu cpu = {0};

  CpuInstruction program[] = {
      cpu_inst_movi(CPU_R1, 0),              // size_t i = 0
      cpu_inst_cmpi(CPU_R1, max_iterations), // R1 < 10
      cpu_inst_jge(LABEL),                   // if (R1 < 10)
      cpu_inst_addi(CPU_R1, 0x1),            // i++
      cpu_inst_jmp(1),                       // jmp back

      cpu_inst_pushi(0),         // size_t a = 0;
      cpu_inst_smovr(a, CPU_R1), // a = i;

      // LABEL
      cpu_inst_exit(0),
  };

  cpu_run_program(&cpu, program);
  assert(cpu.stack[a] == max_iterations);

  return 0;
}
