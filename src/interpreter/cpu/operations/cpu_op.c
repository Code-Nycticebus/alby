#include "interpreter/cpu/cpu_op.h"

#include <assert.h>
#include <stdlib.h>

#include "interpreter/cpu/cpu_defines.h"
#include "interpreter/cpu/cpu_instructions.h"

#include "cpu_op_internals.h"

/* Jumps */

CpuError cpu_op_jmp(Cpu *cpu, size_t ptr) {
  cpu->ip = ptr;
  return CPU_ERR_OK;
}

CpuError cpu_op_je(Cpu *cpu, size_t ptr) {
  if (cpu->zf == 0) {
    cpu->ip = ptr;
  }
  return CPU_ERR_OK;
}

CpuError cpu_op_jl(Cpu *cpu, size_t ptr) {
  if (cpu->zf < 0) {
    cpu->ip = ptr;
  }
  return CPU_ERR_OK;
}
CpuError cpu_op_jle(Cpu *cpu, size_t ptr) {
  if (cpu->zf <= 0) {
    cpu->ip = ptr;
  }
  return CPU_ERR_OK;
}

CpuError cpu_op_jg(Cpu *cpu, size_t ptr) {
  if (0 < cpu->zf) {
    cpu->ip = ptr;
  }
  return CPU_ERR_OK;
}
CpuError cpu_op_jge(Cpu *cpu, size_t ptr) {
  if (0 <= cpu->zf) {
    cpu->ip = ptr;
  }
  return CPU_ERR_OK;
}

/* Utils */

CpuError cpu_op_syscall(Cpu *cpu, const int64_t syscall) {
  (void)cpu;
  switch (syscall) {
  default:
    break;
  }
  assert(0 && "Unhandled Syscall");
  return CPU_ERR_INVALID_INSTRUCTION;
}

CpuError cpu_op_exit(Cpu *cpu, const Word code) {
  cpu->reg[CPU_R1] = code;
  return CPU_ERROR_EXIT;
}
