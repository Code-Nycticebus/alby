#pragma once

#include "cpu_defines.h"

struct CpuInstruction {
  CpuOp operation;
  Word value1;
  Word value2;
};

/* Memory */

CpuInstruction cpu_inst_movi(Register reg, Word value);
CpuInstruction cpu_inst_movr(Register reg1, Register reg2);
CpuInstruction cpu_inst_movs(Register reg, size_t stack_offset);

CpuInstruction cpu_inst_pushi(Word value);
CpuInstruction cpu_inst_pushr(Register reg);

CpuInstruction cpu_inst_smovi(size_t stack_offset, Word value);
CpuInstruction cpu_inst_smovr(size_t stack_offset, Register reg);

CpuInstruction cpu_inst_pop(Register reg);

/* Program flow */

CpuInstruction cpu_inst_jmp(size_t ptr);
CpuInstruction cpu_inst_je(size_t ptr);
CpuInstruction cpu_inst_jl(size_t ptr);
CpuInstruction cpu_inst_jle(size_t ptr);
CpuInstruction cpu_inst_jg(size_t ptr);
CpuInstruction cpu_inst_jge(size_t ptr);
CpuInstruction cpu_inst_cmpi(Register reg, Word value);
CpuInstruction cpu_inst_cmpr(Register reg1, Register reg2);

/* Math */
CpuInstruction cpu_inst_addi(Register reg, Word value);
CpuInstruction cpu_inst_addr(Register reg1, Register reg2);

CpuInstruction cpu_inst_subi(Register reg, Word value);
CpuInstruction cpu_inst_subr(Register reg1, Register reg2);

CpuInstruction cpu_inst_muli(Register reg, Word value);
CpuInstruction cpu_inst_mulr(Register reg1, Register reg2);

CpuInstruction cpu_inst_divi(Register reg, Word value);
CpuInstruction cpu_inst_divr(Register reg1, Register reg2);

/* Utils */
CpuInstruction cpu_inst_syscall(Word syscall);
CpuInstruction cpu_inst_debug(void);
CpuInstruction cpu_inst_exit(Word code);
