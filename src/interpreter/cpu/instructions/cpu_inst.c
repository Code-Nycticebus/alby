
#include "interpreter/cpu/cpu_instructions.h"

/* Program flow */

CpuInstruction cpu_inst_jmp(size_t ptr) {
  return (CpuInstruction){
      .operation = CPU_OP_JMP,
      .value1 = {.size_t = ptr},
  };
}

CpuInstruction cpu_inst_je(size_t ptr) {
  return (CpuInstruction){
      .operation = CPU_OP_JE,
      .value1 = {.size_t = ptr},
  };
}

CpuInstruction cpu_inst_jl(size_t ptr) {
  return (CpuInstruction){
      .operation = CPU_OP_JL,
      .value1 = {.size_t = ptr},
  };
}

CpuInstruction cpu_inst_jle(size_t ptr) {
  return (CpuInstruction){
      .operation = CPU_OP_JLE,
      .value1 = {.size_t = ptr},
  };
}

CpuInstruction cpu_inst_jg(size_t ptr) {
  return (CpuInstruction){
      .operation = CPU_OP_JG,
      .value1 = {.size_t = ptr},
  };
}

CpuInstruction cpu_inst_jge(size_t ptr) {
  return (CpuInstruction){
      .operation = CPU_OP_JGE,
      .value1 = {.size_t = ptr},
  };
}

/* Util */
CpuInstruction cpu_inst_syscall(const int64_t syscall) {
  return (CpuInstruction){
      .operation = CPU_OP_SYSCALL,
      .value1 = {.i64 = syscall},
  };
}

CpuInstruction cpu_inst_debug(void) {
  return (CpuInstruction){
      .operation = CPU_OP_DEBUG,
  };
}

CpuInstruction cpu_inst_exit(const int32_t code) {
  return (CpuInstruction){
      .operation = CPU_OP_EXIT,
      .value1 = {.i32 = code},
  };
}
