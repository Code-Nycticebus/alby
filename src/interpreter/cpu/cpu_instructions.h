#pragma once

#include "interpreter/cpu/cpu.h"
#include "interpreter/cpu/cpu_defines.h"

struct CpuInstruction {
  CpuOp operation;
  Word value1;
  Word value2;
};

/*************************
 *      Generic          *
 *************************/
/* Program flow */
CpuInstruction cpu_inst_jmp(size_t ptr);
CpuInstruction cpu_inst_je(size_t ptr);
CpuInstruction cpu_inst_jl(size_t ptr);
CpuInstruction cpu_inst_jle(size_t ptr);
CpuInstruction cpu_inst_jg(size_t ptr);
CpuInstruction cpu_inst_jge(size_t ptr);

/* Utils */
CpuInstruction cpu_inst_syscall(int64_t syscall);
CpuInstruction cpu_inst_debug(void);
CpuInstruction cpu_inst_exit(int32_t code);

/*************************
 *          i64          *
 *************************/
/* Memory */

CpuInstruction cpu_inst_i64_mov(Register reg, int64_t value);
CpuInstruction cpu_inst_i64_movr(Register reg1, Register reg2);
CpuInstruction cpu_inst_i64_movs(Register reg, size_t stack_offset);

CpuInstruction cpu_inst_i64_push(int64_t value);
CpuInstruction cpu_inst_i64_pushr(Register reg);

CpuInstruction cpu_inst_i64_smov(size_t stack_offset, int64_t value);
CpuInstruction cpu_inst_i64_smovr(size_t stack_offset, Register reg);

CpuInstruction cpu_inst_i64_pop(Register reg);

/* Compare */

CpuInstruction cpu_inst_i64_cmp(Register reg, int64_t value);
CpuInstruction cpu_inst_i64_cmpr(Register reg1, Register reg2);

/* Math */

CpuInstruction cpu_inst_i64_add(Register reg, int64_t value);
CpuInstruction cpu_inst_i64_addr(Register reg1, Register reg2);

CpuInstruction cpu_inst_i64_sub(Register reg, int64_t value);
CpuInstruction cpu_inst_i64_subr(Register reg1, Register reg2);

CpuInstruction cpu_inst_i64_mul(Register reg, int64_t value);
CpuInstruction cpu_inst_i64_mulr(Register reg1, Register reg2);

CpuInstruction cpu_inst_i64_div(Register reg, int64_t value);
CpuInstruction cpu_inst_i64_divr(Register reg1, Register reg2);
