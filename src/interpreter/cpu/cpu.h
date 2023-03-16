#pragma once

#include "cpu_defines.h"

#include <stdio.h>

typedef struct Cpu {
  size_t ip;
  int64_t zf;
  Word reg[CPU_R_COUNT];

  uint8_t stack[CPU_STACK_SIZE];
} Cpu;

int cpu_run_program(Cpu *cpu, const CpuInstruction *program);
void cpu_dump(FILE *file, const Cpu *cpu);
