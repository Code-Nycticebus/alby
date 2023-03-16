#include "parser.h"
#include "interpreter/cpu/cpu_inst.h"

#include <assert.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

static uint64_t parse_u64(Token tk) {
  // TODO better error handling on int parsing
  assert(tk.kind == TOKEN_LIT_I64);
  const int base = 10;
  return strtoul(tk.token, NULL, base);
}

static ParserResult parse_exit(Parser *parser) {
  Token return_code = lexer_next(&parser->lexer);
  if (return_code.kind != TOKEN_LIT_I64) {
    return ResultErr(parser_error(parser, return_code,
                                  "return code has to be an i64 literal!"));
  }

  return ResultOk(cpu_inst_exit(parse_u64(return_code)));
}

static ParserResult parse_smov(Parser *parser) {
  Token reg = lexer_next(&parser->lexer);
  if (reg.kind != TOKEN_REGISTER_SB) {
    return ResultErr(
        parser_error(parser, reg, "First operant has to be the rsb register"));
  }
  Token plus = lexer_next(&parser->lexer);
  if (plus.kind != TOKEN_PUNCT_PLUS) {
    return ResultErr(parser_error(parser, plus, "Expected a plus here!"));
  }

  Token offset = lexer_next(&parser->lexer);

  if (offset.kind != TOKEN_LIT_I64) {
    return ResultErr(
        parser_error(parser, offset, "return code has to be an i64 literal!"));
  }

  Token end_addr = lexer_next(&parser->lexer);
  if (end_addr.kind != TOKEN_DEL_RSQUARE) {
    return ResultErr(
        parser_error(parser, end_addr, "Expected closing bracket!"));
  }

  Token comma = lexer_next(&parser->lexer);
  if (comma.kind != TOKEN_DEL_COMMA) {
    return ResultErr(parser_error(parser, comma, "Expected a comma delimeter"));
  }

  Token second_operant = lexer_next(&parser->lexer);
  if (token_is_register(second_operant.kind)) {
    return ResultOk(cpu_inst_smovr(parse_u64(offset),
                                   second_operant.kind - TOKEN_REGISTER_1));
  }
  if (second_operant.kind == TOKEN_LIT_I64) {
    return ResultOk(
        cpu_inst_smovi(parse_u64(offset), parse_u64(second_operant)));
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

  if (!(TOKEN_REGISTER_1 <= first_operant.kind &&
        first_operant.kind <= TOKEN_REGISTER_8)) {
    return ResultErr(parser_error(parser, first_operant,
                                  "First operant has to be a register"));
  }
  Token comma = lexer_next(&parser->lexer);
  if (comma.kind != TOKEN_DEL_COMMA) {
    return ResultErr(parser_error(parser, comma, "Expected a comma delimeter"));
  }

  Token second_operant = lexer_next(&parser->lexer);
  if (token_is_register(second_operant.kind)) {
    return ResultOk(cpu_inst_movr(first_operant.kind - TOKEN_REGISTER_1,
                                  second_operant.kind - TOKEN_REGISTER_1));
  }
  if (second_operant.kind == TOKEN_LIT_I64) {
    return ResultOk(cpu_inst_movi(first_operant.kind - TOKEN_REGISTER_1,
                                  parse_u64(second_operant)));
  }
  if (second_operant.kind == TOKEN_SYMBOL) {
    return ResultErr(parser_error(parser, second_operant,
                                  "Datatypes are not impemented yet!"));
  }

  return ResultErr(parser_error(
      parser, second_operant,
      "Second operant has to be an i64 literal, an address or a register"));
}

static ParserResult parse_add(Parser *parser) {
  Token reg = lexer_next(&parser->lexer);
  if (!token_is_register(reg.kind)) {
    return ResultErr(
        parser_error(parser, reg, "First operant has to be a register"));
  }

  Token comma = lexer_next(&parser->lexer);
  if (comma.kind != TOKEN_DEL_COMMA) {
    return ResultErr(parser_error(parser, comma, "Expected a comma delimeter"));
  }

  Token second_operant = lexer_next(&parser->lexer);
  if (token_is_register(second_operant.kind)) {
    return ResultOk(cpu_inst_addr(reg.kind - TOKEN_REGISTER_1,
                                  second_operant.kind - TOKEN_REGISTER_1));
  }
  if (second_operant.kind == TOKEN_LIT_I64) {
    return ResultOk(
        cpu_inst_addi(reg.kind - TOKEN_REGISTER_1, parse_u64(second_operant)));
  }

  return ResultErr(
      parser_error(parser, second_operant,
                   "Second operant has to be a i64 literal or a register"));
}

static ParserResult parse_sub(Parser *parser) {
  Token reg = lexer_next(&parser->lexer);
  if (!token_is_register(reg.kind)) {
    return ResultErr(
        parser_error(parser, reg, "First operant has to be a register"));
  }

  Token comma = lexer_next(&parser->lexer);
  if (comma.kind != TOKEN_DEL_COMMA) {
    return ResultErr(parser_error(parser, comma, "Expected a comma delimeter"));
  }

  Token second_operant = lexer_next(&parser->lexer);
  if (token_is_register(second_operant.kind)) {
    return ResultOk(cpu_inst_subr(reg.kind - TOKEN_REGISTER_1,
                                  second_operant.kind - TOKEN_REGISTER_1));
  }
  if (second_operant.kind == TOKEN_LIT_I64) {
    return ResultOk(
        cpu_inst_subi(reg.kind - TOKEN_REGISTER_1, parse_u64(second_operant)));
  }

  return ResultErr(
      parser_error(parser, second_operant,
                   "Second operant has to be a i64 literal or a register"));
}

static ParserResult parser_push(Parser *parser) {
  Token reg = lexer_next(&parser->lexer);
  if (token_is_register(reg.kind)) {
    return ResultOk(cpu_inst_pushr(reg.kind - TOKEN_REGISTER_1));
  }

  if (reg.kind == TOKEN_LIT_I64) {
    return ResultOk(cpu_inst_pushi(parse_u64(reg)));
  }

  return ResultErr(
      parser_error(parser, reg, "First operant has to be a register"));
}

static ParserResult parse_cmp(Parser *parser) {
  Token reg = lexer_next(&parser->lexer);
  if (!token_is_register(reg.kind)) {
    return ResultErr(
        parser_error(parser, reg, "First operant has to be a register"));
  }

  Token comma = lexer_next(&parser->lexer);
  if (comma.kind != TOKEN_DEL_COMMA) {
    return ResultErr(parser_error(parser, comma, "Expected a comma delimeter"));
  }

  Token second_operant = lexer_next(&parser->lexer);
  if (token_is_register(second_operant.kind)) {
    return ResultOk(cpu_inst_cmpr(reg.kind - TOKEN_REGISTER_1,
                                  second_operant.kind - TOKEN_REGISTER_1));
  }
  if (second_operant.kind == TOKEN_LIT_I64) {
    return ResultOk(
        cpu_inst_cmpi(reg.kind - TOKEN_REGISTER_1, parse_u64(second_operant)));
  }
  return ResultErr(
      parser_error(parser, second_operant,
                   "Second operant has to be a i64 literal or a register"));
}

static ParserResult parse_jmp(Parser *parser) {
  Token ptr = lexer_next(&parser->lexer);
  if (ptr.kind != TOKEN_LIT_I64) {
    return ResultErr(
        parser_error(parser, ptr, "jump address has to be a literal integer"));
  }

  // TODO error handling on jump address
  Word jmp_address = parse_u64(ptr);

  return ResultOk(cpu_inst_jmp(jmp_address));
}

static ParserResult parse_jge(Parser *parser) {
  Token ptr = lexer_next(&parser->lexer);
  if (ptr.kind != TOKEN_LIT_I64) {
    return ResultErr(
        parser_error(parser, ptr, "jump address has to be a literal integer"));
  }

  // TODO error handling on jump address
  Word jmp_address = parse_u64(ptr);

  return ResultOk(cpu_inst_jge(jmp_address));
}

ParserResult parser_next(Parser *parser) {
  Token tk = lexer_next(&parser->lexer);

  switch (tk.kind) {

  case TOKEN_PUSH:
    return parser_push(parser);

  case TOKEN_MOV:
    return parse_mov(parser);

  case TOKEN_OP_ADD:
    return parse_add(parser);

  case TOKEN_OP_SUB:
    return parse_sub(parser);

  case TOKEN_CMP:
    return parse_cmp(parser);

  case TOKEN_JMP:
    return parse_jmp(parser);

  case TOKEN_JGE:
    return parse_jge(parser);

  case TOKEN_DEBUG:
    return ResultOk(cpu_inst_debug());

  case TOKEN_EXIT:
    return parse_exit(parser);

  case TOKEN_EOF:
    parser->eof = true;
    return ResultOk(cpu_inst_exit(0));

    // TODO find a better way to skip newlines!
  case TOKEN_DEL_NEWLINE:
    return parser_next(parser);

  case TOKEN_COMMENT:
    return parser_next(parser);

  default:
    parser->eof = true;
    return ResultErr(parser_error(parser, tk, "Invalid token!"));
  }

  assert(0 && "Unreachable");
  parser->eof = true;
  return ResultErr((ParserError){
      .file = parser->filename,
      .line = parser->lexer.line,
      .bol = &parser->lexer.content[parser->lexer.bol],
      .tk = tk,
      .msg = "NOT IMPLEMENTED",
  });
}
