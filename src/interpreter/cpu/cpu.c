#include "cpu.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpu_inst.h"
#include "cpu_op.h"

int cpu_run_program(Cpu *cpu, const CpuInstruction *program) {
  while (true) {
    CpuError err = cpu_step(cpu, &program[cpu->ip++]);
    if (err != CPU_ERR_OK) {
      if (err == CPU_ERROR_EXIT) {
        return (int)cpu->reg[CPU_R1];
      }
      fprintf(stderr, "CpuError: %s\n", cpu_err_to_cstr(err));
      cpu_dump(stderr, cpu);
      return err;
    }
  }
  return CPU_ERR_OK;
}

static void _dump_bytes(FILE *file, const Word *value) {
  const uint8_t *ptr = (const uint8_t *)value;
  for (size_t i = 0; i < sizeof(Word); ++i) {
    fprintf(file, "%02x ", ptr[sizeof(Word) - i - 1]);
  }
}

void cpu_dump(FILE *file, const Cpu *cpu) {
  fprintf(file, "Cpu %lu-bit:\n", CPU_ARCHITECTURE);
  fprintf(file, "  ip: %lu\n", cpu->ip);
  fprintf(file, "  zf: %ld\n", cpu->zf);

  fprintf(file, "  Registers:\n");
  for (size_t i = 0; i < CPU_R_COUNT; ++i) {
    fprintf(file, "    R%lu: ", i + 1);
    _dump_bytes(file, &cpu->reg[i]);
    fprintf(file, "\n");
  }

  fprintf(file, "  Stack:\n");
  if (cpu->sp) {
    for (size_t i = 0; i < cpu->sp; ++i) {
      fprintf(file, "    %3lu: ", cpu->sp - i - 1);
      _dump_bytes(file, &cpu->stack[cpu->sp - i - 1]);
      fprintf(file, " -> %lu \n", cpu->stack[cpu->sp - i - 1]);
    }
  } else {
    fprintf(file, "    --- empty ---\n");
  }
}
