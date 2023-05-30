#pragma once

#include "cpu_defines.h"

#include <stdio.h>

typedef struct Cpu {
  size_t ip;
  int64_t zf;
  Word reg[CPU_REGISTER_COUNT];
  size_t rsp;
  size_t rsb;

  uint8_t stack[CPU_STACK_SIZE];
} Cpu;

int cpu_run_program(Cpu *cpu, const CpuInstruction *program,
                    size_t num_instructions);
void cpu_dump(FILE *file, const Cpu *cpu);
