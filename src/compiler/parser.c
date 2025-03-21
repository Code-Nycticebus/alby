#include "parser.h"
#include "interpreter/cpu/cpu_instructions.h"

#include <assert.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define EXPECT_TOKEN(p, token, msg)                                            \
  do {                                                                         \
    Token t = lexer_next(&p->lexer);                                           \
    if (t.kind != token) {                                                     \
      return ResultErr(parser_error(p, t, msg));                               \
    }                                                                          \
  } while (0);

static ParserError parser_error(const Parser *parser, const Token tk,
                                const char *msg) {
  return (ParserError){
      .file = parser->filename,
      .line = parser->lexer.line,
      .bol = &parser->lexer.content[parser->lexer.bol],
      .tk = tk,
      .msg = msg,
  };
}

static int64_t parse_i64(Token tk) {
  assert(tk.kind == TOKEN_LIT_I64);
  return strtoll(tk.token, NULL, 0);
}

static ParserResult parse_exit(Parser *parser) {
  Token return_code = lexer_next(&parser->lexer);
  if (return_code.kind != TOKEN_LIT_I64) {
    return ResultErr(parser_error(parser, return_code,
                                  "return code has to be an i64 literal!"));
  }

  return ResultOk(cpu_inst_exit(parse_i64(return_code)));
}

static ParserResult parse_smov(Parser *parser) {
  // TODO maybe support rsp aswell
  EXPECT_TOKEN(parser, TOKEN_REGISTER_SB, "Register has to be 'rsb'");
  EXPECT_TOKEN(parser, TOKEN_PUNCT_PLUS, "Expected a '+' here");

  Token offset = lexer_next(&parser->lexer);
  if (offset.kind != TOKEN_LIT_I64) {
    return ResultErr(
        parser_error(parser, offset, "offset has to be an i64 literal!"));
  }

  EXPECT_TOKEN(parser, TOKEN_DEL_RSQUARE, "Expected closing bracket");
  EXPECT_TOKEN(parser, TOKEN_DEL_COMMA, "Expected comma delimiter");

  Token second_operant = lexer_next(&parser->lexer);
  if (token_is_register(second_operant.kind)) {
    return ResultOk(cpu_inst_i64_smovr(parse_i64(offset),
                                       second_operant.kind - TOKEN_REGISTER_1));
  }
  if (second_operant.kind == TOKEN_LIT_I64) {
    return ResultOk(
        cpu_inst_i64_smov(parse_i64(offset), parse_i64(second_operant)));
  }
  if (second_operant.kind == TOKEN_SYMBOL) {
    return ResultErr(parser_error(parser, second_operant,
                                  "Datasizes are not impemented yet!"));
  }

  return ResultErr(parser_error(
      parser, second_operant,
      "Second operant has to be an i64 literal, an address or a register"));
}

static ParserResult parse_mov(Parser *parser) {
  Token first_operant = lexer_next(&parser->lexer);
  if (first_operant.kind == TOKEN_DEL_LSQUARE) {
    return parse_smov(parser);
  }

  if (!token_is_register(first_operant.kind)) {
    return ResultErr(parser_error(parser, first_operant,
                                  "First operant has to be a register"));
  }

  EXPECT_TOKEN(parser, TOKEN_DEL_COMMA, "Expected comma delimiter");

  Token second_operant = lexer_next(&parser->lexer);
  if (token_is_register(second_operant.kind)) {
    return ResultOk(cpu_inst_i64_movr(first_operant.kind - TOKEN_REGISTER_1,
                                      second_operant.kind - TOKEN_REGISTER_1));
  }
  if (second_operant.kind == TOKEN_LIT_I64) {
    return ResultOk(cpu_inst_i64_mov(first_operant.kind - TOKEN_REGISTER_1,
                                     parse_i64(second_operant)));
  }
  if (second_operant.kind == TOKEN_SYMBOL) {
    return ResultErr(parser_error(parser, second_operant,
                                  "Datatypes are not impemented yet!"));
  }

  if (second_operant.kind == TOKEN_DEL_LSQUARE) {
    Token reg = lexer_next(&parser->lexer);
    if (reg.kind != TOKEN_REGISTER_SB) {
      return ResultErr(parser_error(
          parser, reg,
          "For now, only the address of the stack base pointer is supported!"));
    }

    EXPECT_TOKEN(parser, TOKEN_PUNCT_PLUS, "Expected '+'");

    Token offset = lexer_next(&parser->lexer);
    if (offset.kind != TOKEN_LIT_I64) {
      return ResultErr(
          parser_error(parser, offset, "Offset has to be an u64 literal"));
    }

    EXPECT_TOKEN(parser, TOKEN_DEL_RSQUARE, "Expected closing bracket");

    return ResultOk(
        cpu_inst_i64_movs(reg.kind - TOKEN_REGISTER_1, parse_i64(offset)));
  }

  return ResultErr(parser_error(
      parser, second_operant,
      "Second operant has to be an i64 literal, an address or a register"));
}

typedef CpuInstruction inst_i64_fn(Register, int64_t);
typedef CpuInstruction inst_i64r_fn(Register, Register);

typedef struct MathInstructionFunctions {
  inst_i64_fn *i64;
  inst_i64r_fn *i64r;
} MathInstructionFunctions;

static ParserResult parse_math(Parser *parser, MathInstructionFunctions fns) {
  Token reg = lexer_next(&parser->lexer);
  inst_i64_fn *inst_i_fn = fns.i64;
  inst_i64r_fn *inst_r_fn = fns.i64r;

  if (reg.kind == TOKEN_DEL_LSQUARE) {
    Token size_tk = lexer_next(&parser->lexer);
    if (size_tk.kind != TOKEN_LIT_I64) {
      return ResultErr(
          parser_error(parser, size_tk, "Size has to be a i64 literal"));
    }

    size_t size = parse_i64(size_tk);

    if (size == sizeof(int64_t) * 8) {
      inst_i_fn = fns.i64;
      inst_r_fn = fns.i64r;
    } else {
      return ResultErr(
          parser_error(parser, size_tk, "This size is not implemented"));
    }

    EXPECT_TOKEN(parser, TOKEN_DEL_RSQUARE, "Expected closing bracket");

    reg = lexer_next(&parser->lexer);
  }

  if (!token_is_register(reg.kind)) {
    return ResultErr(
        parser_error(parser, reg, "First operant has to be a register"));
  }
  EXPECT_TOKEN(parser, TOKEN_DEL_COMMA, "Expected comma delimiter");

  Token second_operant = lexer_next(&parser->lexer);

  if (token_is_register(second_operant.kind)) {
    return ResultOk(inst_r_fn(reg.kind - TOKEN_REGISTER_1,
                              second_operant.kind - TOKEN_REGISTER_1));
  }
  if (second_operant.kind == TOKEN_LIT_I64) {
    return ResultOk(
        inst_i_fn(reg.kind - TOKEN_REGISTER_1, parse_i64(second_operant)));
  }

  return ResultErr(
      parser_error(parser, second_operant,
                   "Second operant has to be a i64 literal or a register"));
}

static ParserResult parse_add(Parser *parser) {
  return parse_math(parser, (MathInstructionFunctions){
                                .i64 = cpu_inst_i64_add,
                                .i64r = cpu_inst_i64_addr,
                            });
}

static ParserResult parse_sub(Parser *parser) {
  return parse_math(parser, (MathInstructionFunctions){
                                .i64 = cpu_inst_i64_sub,
                                .i64r = cpu_inst_i64_subr,
                            });
}

static ParserResult parse_mul(Parser *parser) {
  return parse_math(parser, (MathInstructionFunctions){
                                .i64 = cpu_inst_i64_mul,
                                .i64r = cpu_inst_i64_mulr,
                            });
}

static ParserResult parse_div(Parser *parser) {
  return parse_math(parser, (MathInstructionFunctions){
                                .i64 = cpu_inst_i64_div,
                                .i64r = cpu_inst_i64_divr,
                            });
}

static ParserResult parse_push(Parser *parser) {
  Token reg = lexer_next(&parser->lexer);

  if (token_is_register(reg.kind)) {
    return ResultOk(cpu_inst_i64_pushr(reg.kind - TOKEN_REGISTER_1));
  }

  if (reg.kind == TOKEN_LIT_I64) {
    return ResultOk(cpu_inst_i64_push(parse_i64(reg)));
  }

  return ResultErr(parser_error(
      parser, reg, "First operant has to be a register or an i64 literal"));
}

static ParserResult parse_cmp(Parser *parser) {
  Token reg = lexer_next(&parser->lexer);
  if (!token_is_register(reg.kind)) {
    return ResultErr(
        parser_error(parser, reg, "First operant has to be a register"));
  }

  EXPECT_TOKEN(parser, TOKEN_DEL_COMMA, "Expected comma delimiter");

  Token second_operant = lexer_next(&parser->lexer);
  if (token_is_register(second_operant.kind)) {
    return ResultOk(cpu_inst_i64_cmpr(reg.kind - TOKEN_REGISTER_1,
                                      second_operant.kind - TOKEN_REGISTER_1));
  }
  if (second_operant.kind == TOKEN_LIT_I64) {
    return ResultOk(cpu_inst_i64_cmp(reg.kind - TOKEN_REGISTER_1,
                                     parse_i64(second_operant)));
  }
  return ResultErr(
      parser_error(parser, second_operant,
                   "Second operant has to be a i64 literal or a register"));
}

static ParserResult parse_jmps(Parser *parser,
                               CpuInstruction (*jmp_fn)(size_t)) {
  Token ptr = lexer_next(&parser->lexer);

  if (ptr.kind == TOKEN_LIT_I64) {
    return ResultOk(jmp_fn(parse_i64(ptr)));
  }

  if (ptr.kind == TOKEN_SYMBOL) {
    return ResultErr(parser_error(
        parser, ptr, "symbol as jump values are not implemented yet!"));
  }

  return ResultErr(
      parser_error(parser, ptr, "jump address has to be a literal integer"));
}

static ParserResult parse_jmp(Parser *parser) {
  return parse_jmps(parser, cpu_inst_jmp);
}
static ParserResult parse_je(Parser *parser) {
  return parse_jmps(parser, cpu_inst_je);
}
static ParserResult parse_jl(Parser *parser) {
  return parse_jmps(parser, cpu_inst_jl);
}
static ParserResult parse_jle(Parser *parser) {
  return parse_jmps(parser, cpu_inst_jle);
}
static ParserResult parse_jg(Parser *parser) {
  return parse_jmps(parser, cpu_inst_jg);
}
static ParserResult parse_jge(Parser *parser) {
  return parse_jmps(parser, cpu_inst_jge);
}

ParserResult parser_next(Parser *parser) {
  Token tk = lexer_next(&parser->lexer);

  switch (tk.kind) {

  case TOKEN_PUSH:
    return parse_push(parser);
  case TOKEN_MOV:
    return parse_mov(parser);

  case TOKEN_OP_ADD:
    return parse_add(parser);
  case TOKEN_OP_SUB:
    return parse_sub(parser);
  case TOKEN_OP_MUL:
    return parse_mul(parser);
  case TOKEN_OP_DIV:
    return parse_div(parser);

  case TOKEN_CMP:
    return parse_cmp(parser);

  case TOKEN_JMP:
    return parse_jmp(parser);
  case TOKEN_JE:
    return parse_je(parser);
  case TOKEN_JL:
    return parse_jl(parser);
  case TOKEN_JLE:
    return parse_jle(parser);
  case TOKEN_JG:
    return parse_jg(parser);
  case TOKEN_JGE:
    return parse_jge(parser);

  case TOKEN_DEBUG:
    return ResultOk(cpu_inst_debug());

  case TOKEN_EXIT:
    return parse_exit(parser);

  case TOKEN_EOF:
    parser->eof = true;
    return ResultOk(cpu_inst_exit(0));

  /* Skipping by recursing */
  case TOKEN_DEL_NEWLINE:
    return parser_next(parser);
  case TOKEN_COMMENT:
    return parser_next(parser);

  default:
    return ResultErr(parser_error(parser, tk, "Invalid token!"));
  }

  assert(0 && "Unreachable");
  parser->eof = true;
  return ResultErr(parser_error(parser, tk, "Unreachable!"));
}
