
#include "interpreter/cpu/cpu_instructions.h"

CpuInstruction cpu_inst_i8_mov(const Register reg, const int8_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_MOV,
      .value1 = {.r = reg},
      .value2 = {.i8 = value},
  };
}

CpuInstruction cpu_inst_i8_movr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_MOVR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}
CpuInstruction cpu_inst_i8_movs(const Register reg, const size_t stack_offset) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_MOVS,
      .value1 = {.r = reg},
      .value2 = {.size_t = stack_offset},
  };
}

CpuInstruction cpu_inst_i8_push(const int8_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_PUSHI,
      .value1 = {.i8 = value},
  };
}

CpuInstruction cpu_inst_i8_pushr(const Register reg) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_PUSHR,
      .value1 = {.r = reg},
  };
}

CpuInstruction cpu_inst_i8_smov(size_t stack_offset, const int8_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_SMOVI,
      .value1 = {.size_t = stack_offset},
      .value2 = {.i8 = value},
  };
}

CpuInstruction cpu_inst_i8_smovr(const size_t stack_offset,
                                 const Register reg) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_SMOVR,
      .value1 = {.size_t = stack_offset},
      .value2 = {.r = reg},
  };
}

CpuInstruction cpu_inst_i8_pop(const Register reg) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_POP,
      .value1 = {.r = reg},
  };
}

CpuInstruction cpu_inst_i8_cmp(Register reg, int8_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_CMPI,
      .value1 = {.r = reg},
      .value2 = {.i8 = value},
  };
}

CpuInstruction cpu_inst_i8_cmpr(Register reg1, Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_CMPR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}

/* Math */

CpuInstruction cpu_inst_i8_add(const Register reg, const int8_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_ADDI,
      .value1 = {.r = reg},
      .value2 = {.i8 = value},
  };
}

CpuInstruction cpu_inst_i8_addr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_ADDR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}

CpuInstruction cpu_inst_i8_sub(const Register reg, const int8_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_SUBI,
      .value1 = {.r = reg},
      .value2 = {.i8 = value},
  };
}

CpuInstruction cpu_inst_i8_subr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_SUBR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}

CpuInstruction cpu_inst_i8_mul(const Register reg, const int8_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_MULI,
      .value1 = {.r = reg},
      .value2 = {.i8 = value},
  };
}

CpuInstruction cpu_inst_i8_mulr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_MULR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}

CpuInstruction cpu_inst_i8_div(const Register reg, const int8_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_DIVI,
      .value1 = {.r = reg},
      .value2 = {.i8 = value},
  };
}

CpuInstruction cpu_inst_i8_divr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_I8_DIVR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}
