#include <assert.h>
#include <stdio.h>

#include "../../../src/interpreter/cpu/cpu.h"
#include "../../../src/interpreter/cpu/cpu_inst.h"

enum Variables {
  A = sizeof(int64_t) * 0,
  B = sizeof(int64_t) * 1,
};

const int64_t first = 0x1;
const int64_t second = 0x2;

int main(void) {
  Cpu cpu = {0};

  /*
   * int64_t a = 0;
   * int64_t b = second;
   *
   * a += first;
   * a += b;
   *
   * exit(a);
   */
  CpuInstruction program[] = {
      cpu_inst_i64_push(0),      // A == Offset 1
      cpu_inst_i64_push(second), // B == Offset 0

      cpu_inst_i64_movs(CPU_R1, A), // A -> R1
      cpu_inst_i64_movs(CPU_R2, B), // B -> R2

      cpu_inst_i64_add(CPU_R1, first),   // R1 += first
      cpu_inst_i64_addr(CPU_R1, CPU_R2), // R1 += R2

      cpu_inst_i64_smovr(A, CPU_R1), // R1 -> A
      // cpu_inst_debug(),
      cpu_inst_exit(0),
  };

  cpu_run_program(&cpu, program, (sizeof(program) / sizeof(program[0])));
  assert(*(int64_t *)&cpu.stack[A] == first + second);
  assert(cpu.reg[CPU_R2].i64 == second);

  return 0;
}
