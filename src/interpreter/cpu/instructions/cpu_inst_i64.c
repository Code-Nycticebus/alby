#include "interpreter/cpu/cpu_instructions.h"

CpuInstruction cpu_inst_i64_mov(const Register reg, const int64_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_MOVI,
      .value1 = {.r = reg},
      .value2 = {.i64 = value},
  };
}

CpuInstruction cpu_inst_i64_movr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_MOVR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}
CpuInstruction cpu_inst_i64_movs(const Register reg,
                                 const size_t stack_offset) {
  return (CpuInstruction){
      .operation = CPU_OP_MOVS,
      .value1 = {.r = reg},
      .value2 = {.size_t = stack_offset},
  };
}

CpuInstruction cpu_inst_i64_push(const int64_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_PUSHI,
      .value1 = {.i64 = value},
  };
}

CpuInstruction cpu_inst_i64_pushr(const Register reg) {
  return (CpuInstruction){
      .operation = CPU_OP_PUSHR,
      .value1 = {.r = reg},
  };
}

CpuInstruction cpu_inst_i64_smov(size_t stack_offset, const int64_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_SMOVI,
      .value1 = {.size_t = stack_offset},
      .value2 = {.i64 = value},
  };
}

CpuInstruction cpu_inst_i64_smovr(const size_t stack_offset,
                                  const Register reg) {
  return (CpuInstruction){
      .operation = CPU_OP_SMOVR,
      .value1 = {.size_t = stack_offset},
      .value2 = {.r = reg},
  };
}

CpuInstruction cpu_inst_i64_pop(const Register reg) {
  return (CpuInstruction){
      .operation = CPU_OP_POP,
      .value1 = {.r = reg},
  };
}
