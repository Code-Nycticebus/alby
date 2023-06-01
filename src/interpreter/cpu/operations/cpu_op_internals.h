#include "interpreter/cpu/cpu_op.h"

#include <assert.h>
#include <stdlib.h>

#include "interpreter/cpu/cpu_defines.h"
#include "interpreter/cpu/cpu_instructions.h"

/**********************
 *      Generic       *
 **********************/

CpuError cpu_op_jmp(Cpu *cpu, size_t ptr);

CpuError cpu_op_je(Cpu *cpu, size_t ptr);

CpuError cpu_op_jl(Cpu *cpu, size_t ptr);
CpuError cpu_op_jle(Cpu *cpu, size_t ptr);

CpuError cpu_op_jg(Cpu *cpu, size_t ptr);
CpuError cpu_op_jge(Cpu *cpu, size_t ptr);

CpuError cpu_op_syscall(Cpu *cpu, int64_t syscall);
CpuError cpu_op_exit(Cpu *cpu, Word code);

/**********************
 *        I64         *
 **********************/

/* Memory */

CpuError cpu_op_i64_mov(Cpu *cpu, Register reg, int64_t value);
CpuError cpu_op_i64_movr(Cpu *cpu, Register reg1, Register reg2);
CpuError cpu_op_i64_movs(Cpu *cpu, Register reg, size_t stack_offset);
CpuError cpu_op_i64_push(Cpu *cpu, int64_t value);
CpuError cpu_op_i64_pushr(Cpu *cpu, Register reg);
CpuError cpu_op_i64_smov(Cpu *cpu, size_t offset, int64_t value);
CpuError cpu_op_i64_smovr(Cpu *cpu, size_t offset, Register reg);
CpuError cpu_op_i64_pop(Cpu *cpu, Register reg);
CpuError cpu_op_i64_cmp(Cpu *cpu, Register reg, int64_t value);
CpuError cpu_op_i64_cmpr(Cpu *cpu, Register reg1, Register reg2);

/* Math */

CpuError cpu_op_i64_add(Cpu *cpu, Register reg, int64_t value);
CpuError cpu_op_i64_addr(Cpu *cpu, Register reg1, Register reg2);
CpuError cpu_op_i64_sub(Cpu *cpu, Register reg, int64_t value);
CpuError cpu_op_i64_subr(Cpu *cpu, Register reg1, Register reg2);
CpuError cpu_op_i64_mul(Cpu *cpu, Register reg, int64_t value);
CpuError cpu_op_i64_mulr(Cpu *cpu, Register reg1, Register reg2);
CpuError cpu_op_i64_div(Cpu *cpu, Register reg, int64_t value);
CpuError cpu_op_i64_divr(Cpu *cpu, Register reg1, Register reg2);

/**********************
 *        i8         *
 **********************/

/* Memory */

CpuError cpu_op_i8_mov(Cpu *cpu, Register reg, int8_t value);
CpuError cpu_op_i8_movr(Cpu *cpu, Register reg1, Register reg2);
CpuError cpu_op_i8_movs(Cpu *cpu, Register reg, size_t stack_offset);
CpuError cpu_op_i8_push(Cpu *cpu, int8_t value);
CpuError cpu_op_i8_pushr(Cpu *cpu, Register reg);
CpuError cpu_op_i8_smov(Cpu *cpu, size_t offset, int8_t value);
CpuError cpu_op_i8_smovr(Cpu *cpu, size_t offset, Register reg);
CpuError cpu_op_i8_pop(Cpu *cpu, Register reg);
CpuError cpu_op_i8_cmp(Cpu *cpu, Register reg, int8_t value);
CpuError cpu_op_i8_cmpr(Cpu *cpu, Register reg1, Register reg2);

/* Math */

CpuError cpu_op_i8_add(Cpu *cpu, Register reg, int8_t value);
CpuError cpu_op_i8_addr(Cpu *cpu, Register reg1, Register reg2);
CpuError cpu_op_i8_sub(Cpu *cpu, Register reg, int8_t value);
CpuError cpu_op_i8_subr(Cpu *cpu, Register reg1, Register reg2);
CpuError cpu_op_i8_mul(Cpu *cpu, Register reg, int8_t value);
CpuError cpu_op_i8_mulr(Cpu *cpu, Register reg1, Register reg2);
CpuError cpu_op_i8_div(Cpu *cpu, Register reg, int8_t value);
CpuError cpu_op_i8_divr(Cpu *cpu, Register reg1, Register reg2);
