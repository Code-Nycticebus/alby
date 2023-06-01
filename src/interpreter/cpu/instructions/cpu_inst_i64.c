#include "interpreter/cpu/cpu_instructions.h"

CpuInstruction cpu_inst_i64_mov(const Register reg, const int64_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_MOV,
      .value1 = {.r = reg},
      .value2 = {.i64 = value},
  };
}

CpuInstruction cpu_inst_i64_movr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_MOVR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}
CpuInstruction cpu_inst_i64_movs(const Register reg,
                                 const size_t stack_offset) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_MOVS,
      .value1 = {.r = reg},
      .value2 = {.size_t = stack_offset},
  };
}

CpuInstruction cpu_inst_i64_push(const int64_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_PUSHI,
      .value1 = {.i64 = value},
  };
}

CpuInstruction cpu_inst_i64_pushr(const Register reg) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_PUSHR,
      .value1 = {.r = reg},
  };
}

CpuInstruction cpu_inst_i64_smov(size_t stack_offset, const int64_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_SMOVI,
      .value1 = {.size_t = stack_offset},
      .value2 = {.i64 = value},
  };
}

CpuInstruction cpu_inst_i64_smovr(const size_t stack_offset,
                                  const Register reg) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_SMOVR,
      .value1 = {.size_t = stack_offset},
      .value2 = {.r = reg},
  };
}

CpuInstruction cpu_inst_i64_pop(const Register reg) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_POP,
      .value1 = {.r = reg},
  };
}

CpuInstruction cpu_inst_i64_cmp(Register reg, int64_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_CMPI,
      .value1 = {.r = reg},
      .value2 = {.i64 = value},
  };
}

CpuInstruction cpu_inst_i64_cmpr(Register reg1, Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_CMPR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}

/* Math */

CpuInstruction cpu_inst_i64_add(const Register reg, const int64_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_ADDI,
      .value1 = {.r = reg},
      .value2 = {.i64 = value},
  };
}

CpuInstruction cpu_inst_i64_addr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_ADDR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}

CpuInstruction cpu_inst_i64_sub(const Register reg, const int64_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_SUBI,
      .value1 = {.r = reg},
      .value2 = {.i64 = value},
  };
}

CpuInstruction cpu_inst_i64_subr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_SUBR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}

CpuInstruction cpu_inst_i64_mul(const Register reg, const int64_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_MULI,
      .value1 = {.r = reg},
      .value2 = {.i64 = value},
  };
}

CpuInstruction cpu_inst_i64_mulr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_MULR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}

CpuInstruction cpu_inst_i64_div(const Register reg, const int64_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_DIVI,
      .value1 = {.r = reg},
      .value2 = {.i64 = value},
  };
}

CpuInstruction cpu_inst_i64_divr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_I64_DIVR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}
