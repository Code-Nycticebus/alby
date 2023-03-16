#include <assert.h>
#include <stdio.h>

#include "../../../src/interpreter/cpu/cpu.h"
#include "../../../src/interpreter/cpu/cpu_inst.h"

enum Variables {
  A = sizeof(Word) * 0,
  B = sizeof(Word) * 1,
};

const Word first = 35;
const Word second = 34;

int main(void) {
  Cpu cpu = {0};

  /*
   * Word a = 0;
   * Word b = second;
   *
   * a += first;
   * a += b;
   *
   * exit(a);
   */
  CpuInstruction program[] = {
      cpu_inst_pushi(0),      // A == Offset 1
      cpu_inst_pushi(second), // B == Offset 0

      cpu_inst_movs(CPU_R1, A), // A -> R1
      cpu_inst_movs(CPU_R2, B), // B -> R2

      cpu_inst_addi(CPU_R1, first),  // R1 += first
      cpu_inst_addr(CPU_R1, CPU_R2), // R1 += R2

      cpu_inst_smovr(A, CPU_R1), // R1 -> A
      cpu_inst_debug(),
      cpu_inst_exit(0),
  };

  cpu_run_program(&cpu, program);

  assert(cpu.stack[A] == first + second);
  assert(cpu.reg[CPU_R2] == second);

  return 0;
}
