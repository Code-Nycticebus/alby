#pragma once

#include "cpu.h"

typedef enum CpuError {
  CPU_ERR_OK,
  CPU_ERROR_EXIT,
  CPU_ERR_INVALID_INSTRUCTION,
  CPU_ERR_STACK_OVERFLOW,
  CPU_ERR_STACK_UNDERFLOW,
  CPU_ERR_ZERO_DIVISION,
} CpuError;

CpuError cpu_step(Cpu *cpu, const CpuInstruction *inst);

const char *cpu_err_to_cstr(CpuError err);
