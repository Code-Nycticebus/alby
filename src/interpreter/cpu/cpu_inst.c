
#include "cpu_inst.h"

CpuInstruction cpu_inst_movi(const Register reg, const Word value) {
  return (CpuInstruction){
      .operation = CPU_OP_MOVI,
      .value1 = reg,
      .value2 = value,
  };
}

CpuInstruction cpu_inst_movr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_MOVR,
      .value1 = reg1,
      .value2 = reg2,
  };
}
CpuInstruction cpu_inst_movs(const Register reg, const size_t stack_offset) {
  return (CpuInstruction){
      .operation = CPU_OP_MOVS,
      .value1 = reg,
      .value2 = stack_offset,
  };
}

CpuInstruction cpu_inst_pushi(const Word value) {
  return (CpuInstruction){
      .operation = CPU_OP_PUSHI,
      .value1 = value,
  };
}

CpuInstruction cpu_inst_pushr(const Register reg) {
  return (CpuInstruction){
      .operation = CPU_OP_PUSHR,
      .value1 = reg,
  };
}

CpuInstruction cpu_inst_smovi(size_t stack_offset, const Word value) {
  return (CpuInstruction){
      .operation = CPU_OP_SMOVI,
      .value1 = stack_offset,
      .value2 = value,
  };
}

CpuInstruction cpu_inst_smovr(const size_t stack_offset, const Register reg) {
  return (CpuInstruction){
      .operation = CPU_OP_SMOVR,
      .value1 = stack_offset,
      .value2 = reg,
  };
}

CpuInstruction cpu_inst_pop(const Register reg) {
  return (CpuInstruction){
      .operation = CPU_OP_POP,
      .value1 = reg,
  };
}

/* Program flow */

CpuInstruction cpu_inst_jmp(size_t ptr) {
  return (CpuInstruction){
      .operation = CPU_OP_JMP,
      .value1 = ptr,
  };
}

CpuInstruction cpu_inst_je(size_t ptr) {
  return (CpuInstruction){
      .operation = CPU_OP_JE,
      .value1 = ptr,
  };
}

CpuInstruction cpu_inst_jl(size_t ptr) {
  return (CpuInstruction){
      .operation = CPU_OP_JL,
      .value1 = ptr,
  };
}

CpuInstruction cpu_inst_jle(size_t ptr) {
  return (CpuInstruction){
      .operation = CPU_OP_JLE,
      .value1 = ptr,
  };
}

CpuInstruction cpu_inst_jg(size_t ptr) {
  return (CpuInstruction){
      .operation = CPU_OP_JG,
      .value1 = ptr,
  };
}

CpuInstruction cpu_inst_jge(size_t ptr) {
  return (CpuInstruction){
      .operation = CPU_OP_JGE,
      .value1 = ptr,
  };
}

CpuInstruction cpu_inst_cmpi(Register reg, Word value) {
  return (CpuInstruction){
      .operation = CPU_OP_CMPI,
      .value1 = reg,
      .value2 = value,
  };
}

CpuInstruction cpu_inst_cmpr(Register reg1, Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_CMPR,
      .value1 = reg1,
      .value2 = reg2,
  };
}

/* Math */

CpuInstruction cpu_inst_addi(const Register reg, const Word value) {
  return (CpuInstruction){
      .operation = CPU_OP_ADDI,
      .value1 = reg,
      .value2 = value,
  };
}

CpuInstruction cpu_inst_addr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_ADDR,
      .value1 = reg1,
      .value2 = reg2,
  };
}

CpuInstruction cpu_inst_subi(const Register reg, const Word value) {
  return (CpuInstruction){
      .operation = CPU_OP_SUBI,
      .value1 = reg,
      .value2 = value,
  };
}

CpuInstruction cpu_inst_subr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_SUBR,
      .value1 = reg1,
      .value2 = reg2,
  };
}

CpuInstruction cpu_inst_muli(const Register reg, const Word value) {
  return (CpuInstruction){
      .operation = CPU_OP_MULI,
      .value1 = reg,
      .value2 = value,
  };
}

CpuInstruction cpu_inst_mulr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_MULR,
      .value1 = reg1,
      .value2 = reg2,
  };
}

CpuInstruction cpu_inst_divi(const Register reg, const Word value) {
  return (CpuInstruction){
      .operation = CPU_OP_DIVI,
      .value1 = reg,
      .value2 = value,
  };
}

CpuInstruction cpu_inst_divr(const Register reg1, const Register reg2) {
  return (CpuInstruction){
      .operation = CPU_OP_DIVR,
      .value1 = reg1,
      .value2 = reg2,
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

CpuInstruction cpu_inst_exit(const Word code) {
  return (CpuInstruction){
      .operation = CPU_OP_EXIT,
      .value1 = code,
  };
}
