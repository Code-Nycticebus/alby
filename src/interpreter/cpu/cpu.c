#include "cpu.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "cpu_instructions.h"
#include "cpu_op.h"

int cpu_run_program(Cpu *cpu, const CpuInstruction *program,
                    size_t num_instructions) {
  while (cpu->ip < num_instructions) {
    CpuError err = cpu_step(cpu, &program[cpu->ip++]);
    if (err != CPU_ERR_OK) {
      if (err == CPU_ERROR_EXIT) {
        return (int)cpu->reg[CPU_R1].i32;
      }
      fprintf(stderr, "CpuError: %s\n", cpu_err_to_cstr(err));
      cpu_dump(stderr, cpu);
      return err;
    }
  }
  fprintf(stderr, "CpuError: %s\n",
          cpu_err_to_cstr(CPU_ERR_INSTRUCTION_POINTER_OVERFLOW));
  cpu_dump(stderr, cpu);
  return CPU_ERR_INSTRUCTION_POINTER_OVERFLOW;
}

static void _dump_bytes(FILE *file, const Word *value) {
  const uint8_t *ptr = (const uint8_t *)value;
  for (size_t i = 0; i < sizeof(Word); ++i) {
    fprintf(file, "%02x ", ptr[sizeof(Word) - i - 1]);
  }
}

void cpu_dump(FILE *file, const Cpu *cpu) {
  fprintf(file, "Cpu %lu-bit:\n", CPU_ARCHITECTURE);
  fprintf(file, "  ip: %4lu\n", cpu->ip);
  fprintf(file, "  zf: %4ld\n", cpu->zf);

  fprintf(file, "  Registers:\n");
  for (size_t i = 0; i < CPU_REGISTER_COUNT; ++i) {
    fprintf(file, "    R%lu: ", i + 1);
    _dump_bytes(file, &cpu->reg[i]);
    fprintf(file, "\n");
  }

  fprintf(file, "  rsp: %4ld\n", cpu->rsp);
  fprintf(file, "  rsb: %4ld\n", cpu->rsb);
  fprintf(file, "  Stack:\n");

  size_t rsp = ((cpu->rsp / sizeof(Word)) + 1) * sizeof(Word);

  if (rsp) {
    for (size_t i = 0; i < rsp / sizeof(Word); ++i) {
      fprintf(file, "   | ");
      for (size_t bytes = 0; bytes < sizeof(Word); ++bytes) {
        fprintf(file, "%02X ",
                cpu->stack[(i * sizeof(Word)) + sizeof(Word) - bytes - 1]);
      }

      fprintf(file, " | ");
      for (size_t bytes = 0; bytes < sizeof(Word); ++bytes) {
        uint8_t current_byte =
            cpu->stack[(i * sizeof(Word)) + sizeof(Word) - bytes - 1];
        if (isprint((current_byte))) {
          fprintf(file, "%c", current_byte);
        } else {
          fprintf(file, ".");
        }
      }

      fprintf(file, " | %lu\n", i * sizeof(Word));
    }

    fprintf(file, "     ");
    for (size_t i = 0; i < sizeof(Word) - cpu->rsp % sizeof(Word) - 1; ++i) {
      fprintf(file, "   ");
    }
    fprintf(file, "^^\n");
  } else {
    fprintf(file, "    --- empty ---\n");
  }
}
