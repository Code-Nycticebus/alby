#include <assert.h>
#include <stdio.h>

#include "../../../src/interpreter/cpu/cpu.h"
#include "../../../src/interpreter/cpu/cpu_instructions.h"

#define loop_start 3
#define loop_end 13
#define program_size 14

enum Variables {
  a = sizeof(Word) * 0,
  b = sizeof(Word) * 1,
};

#define max_iterations 50

int main(void) {
  Cpu cpu = {0};

  CpuInstruction program[] = {
      cpu_inst_i64_push(0), // mov a, 0;
      cpu_inst_i64_push(1), // mov b, 1;

      cpu_inst_i64_mov(CPU_R1, 0),              // mov r1, 0
      cpu_inst_i64_cmp(CPU_R1, max_iterations), // cmp r1, max_iterations
      cpu_inst_jge(loop_end),                   // jge loop_end

      cpu_inst_i64_movs(CPU_R2, a),      // mov r2, a
      cpu_inst_i64_movs(CPU_R3, b),      // mov r3, b
      cpu_inst_i64_addr(CPU_R2, CPU_R3), // add r2, r3

      cpu_inst_i64_smovr(b, CPU_R2), // mov b, r2
      cpu_inst_i64_smovr(a, CPU_R3), // mov a, r3

      cpu_inst_i64_add(CPU_R1, 1), // add r1, 1
      cpu_inst_jmp(loop_start),    // jmp loop_start

      cpu_inst_i64_pushr(CPU_R2), // push r2

      // cpu_inst_debug(), //
      cpu_inst_exit(0),
  };
  assert((sizeof(program) / sizeof(program[0])) == program_size &&
         "You changed the program. check the jmp values");

  cpu_run_program(&cpu, program, (sizeof(program) / sizeof(program[0])));

  assert(cpu.ip == program_size &&
         "Instruction pointer is not at the place it should be!");

  return 0;
}
