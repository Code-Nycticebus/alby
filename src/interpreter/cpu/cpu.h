#pragma once

#include "cpu_defines.h"

#include <stdio.h>

typedef union {
  uint8_t u8;
  int8_t i8;
  uint32_t u32;
  int32_t i32;
  uint64_t u64;
  int64_t i64;
  Register r;
  size_t size_t;
  void *ptr;
} Word;

#define CPU_ARCHITECTURE (sizeof(Word) * 8)

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
