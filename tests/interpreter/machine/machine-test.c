

#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

#include "../../../src/interpreter/cpu/cpu.h"
#include "../../../src/interpreter/cpu/cpu_instructions.h"
#include "interpreter/cpu/cpu_defines.h"

#define OFFSET 9

char msg[] = "Fuck you\0"
             "Who cares";

int main(void) {
  Cpu cpu = {
      .ip = 0,
      .reg = {{0}},
      .rsb = 0,
      .rsp = sizeof(msg),
      .stack = {0},
  };

  for (size_t i = 0; i < sizeof(msg); ++i) {
    cpu.stack[i] = msg[i];
  }

  CpuInstruction program[] = {
      cpu_inst_i64_push(-1), // mov b, 1;
      cpu_inst_i64_mov(CPU_R2, (uintptr_t)&cpu.stack[0]),
      cpu_inst_i64_mov(CPU_R3, (uintptr_t)&cpu.stack[OFFSET]),

      cpu_inst_i64_pop(CPU_R4),
      cpu_inst_i64_sub(CPU_R4, 0),

      cpu_inst_i8_mov(CPU_R5, 0x22),
      cpu_inst_i8_mov(CPU_R5, 0x22),
      cpu_inst_i8_push(0x7F),

      cpu_inst_debug(), //
      cpu_inst_exit(0),
  };

  cpu_run_program(&cpu, program, (sizeof(program) / sizeof(program[0])));
  printf("%s | %s\n", (char *)cpu.reg[CPU_R2].ptr, (char *)cpu.reg[CPU_R3].ptr);
  printf("%ld\n", cpu.reg[CPU_R4].i64);
  return 0;
}
