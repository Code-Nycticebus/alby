
#include "cpu_inst.h"
#include <bits/stdint-intn.h>

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

CpuInstruction cpu_inst_i64_cmp(Register reg, int64_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_CMPI,
      .value1 = {.r = reg},
      .value2 = {.i64 = value},
  };
}

CpuInstruction cpu_inst_i64_cmpr(Register reg1, Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_CMPR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}

/* Math */

CpuInstruction cpu_inst_i64_add(const Register reg, const int64_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_ADDI,
      .value1 = {.r = reg},
      .value2 = {.i64 = value},
  };
}

CpuInstruction cpu_inst_i64_addr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_ADDR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}

CpuInstruction cpu_inst_i64_sub(const Register reg, const int64_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_SUBI,
      .value1 = {.r = reg},
      .value2 = {.i64 = value},
  };
}

CpuInstruction cpu_inst_i64_subr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_SUBR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}

CpuInstruction cpu_inst_i64_mul(const Register reg, const int64_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_MULI,
      .value1 = {.r = reg},
      .value2 = {.i64 = value},
  };
}

CpuInstruction cpu_inst_i64_mulr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_MULR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}

CpuInstruction cpu_inst_i64_div(const Register reg, const int64_t value) {
  return (CpuInstruction){
      .operation = CPU_OP_DIVI,
      .value1 = {.r = reg},
      .value2 = {.i64 = value},
  };
}

CpuInstruction cpu_inst_i64_divr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_DIVR,
      .value1 = {.r = reg1},
      .value2 = {.r = reg2},
  };
}
/* Util */
CpuInstruction cpu_inst_syscall(const Word syscall) {
  return (CpuInstruction){
      .operation = CPU_OP_SYSCALL,
      .value1 = syscall,
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
