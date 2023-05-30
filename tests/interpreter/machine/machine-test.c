

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "../../../src/interpreter/cpu/cpu.h"
#include "../../../src/interpreter/cpu/cpu_inst.h"
#include "interpreter/cpu/cpu_defines.h"

#define max_iterations 5
#define STRLEN 6

enum Constants {
  CONSTANT_STR_LEN,
  CONSTANT_STR = CONSTANT_STR_LEN + sizeof(Word),
  CONSTANT_COUNT = CONSTANT_STR + STRLEN,
};

int main(void) {
  Cpu cpu = {
      .ip = 1,
      .reg = {{0}},
      .rsb = CONSTANT_COUNT,
      .rsp = CONSTANT_COUNT,
      .stack =
          {
              [CONSTANT_STR_LEN] = STRLEN,
              [CONSTANT_STR + 0] = 'H',
              [CONSTANT_STR + 1] = 'e',
              [CONSTANT_STR + 2] = 'l',
              [CONSTANT_STR + 3] = 'l',
              [CONSTANT_STR + 4] = 'o',
              [CONSTANT_STR + 5] = '\0',
          },

  };

  CpuInstruction program[] = {
      cpu_inst_i64_push(0xcafebabe), // mov b, 1;
      cpu_inst_i64_push(0xdeadbeef), // mov b, 1;

      cpu_inst_i64_movs(CPU_R2, CONSTANT_STR),

      cpu_inst_i64_pop(CPU_R1),
      cpu_inst_debug(), //
      cpu_inst_exit(0),
  };

  cpu_run_program(&cpu, program, (sizeof(program) / sizeof(program[0])));

  return 0;
}
