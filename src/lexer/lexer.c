#include "lexer.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

#include "keywords.h"
#include "lexer/token.h"

static const struct {
  TokenKind kind;
  const char c;
} punctuation[] = {
    {TOKEN_DEL_SEMICOLON, ';'}, {TOKEN_DEL_COLON, ':'},
    {TOKEN_DEL_QUOT_DUB, '"'},  {TOKEN_DEL_QUOT_SIN, '\''},
    {TOKEN_DEL_HASH, '#'},

    {TOKEN_DEL_DOT, '.'},       {TOKEN_DEL_COMMA, ','},
    {TOKEN_DEL_EQ, '='},        {TOKEN_DEL_PIPE, '|'},

    {TOKEN_DEL_LPAREN, '('},    {TOKEN_DEL_RPAREN, ')'},
    {TOKEN_DEL_LCURLY, '{'},    {TOKEN_DEL_RCURLY, '}'},
    {TOKEN_DEL_LSQUARE, '['},   {TOKEN_DEL_RSQUARE, ']'},

    {TOKEN_PUNCT_PLUS, '+'},    {TOKEN_PUNCT_MINUS, '-'},

};
static const size_t punctuation_count =
    sizeof(punctuation) / sizeof(punctuation[0]);

Lexer lexer_init(size_t size, const char *content) {
  keywords_init();
  return (Lexer){
      .content = content,
      .size = size,
      .cursor = 0,
      .line = 1, // Why not just start from line one?
      .bol = 0,
  };
}

bool token_is_register(TokenKind kind) {
  assert(TOKEN_REGISTER_SB - TOKEN_REGISTER_1 == 9 &&
         "Number of registers has updated!");
  return (TOKEN_REGISTER_1 <= kind && kind <= TOKEN_REGISTER_SB);
}

#define TOKEN_KIND_TO_STRING(token)                                            \
  case token:                                                                  \
    return #token

const char *token_kind_to_string(TokenKind kind) {
  switch (kind) {
    TOKEN_KIND_TO_STRING(TOKEN_EOF);
    TOKEN_KIND_TO_STRING(TOKEN_SYMBOL);

    TOKEN_KIND_TO_STRING(TOKEN_SYSCALL);
    TOKEN_KIND_TO_STRING(TOKEN_EXIT);

    TOKEN_KIND_TO_STRING(TOKEN_MOV);
    TOKEN_KIND_TO_STRING(TOKEN_PUSH);
    TOKEN_KIND_TO_STRING(TOKEN_POP);

    TOKEN_KIND_TO_STRING(TOKEN_CMP);
    TOKEN_KIND_TO_STRING(TOKEN_JMP);
    TOKEN_KIND_TO_STRING(TOKEN_JE);
    TOKEN_KIND_TO_STRING(TOKEN_JL);
    TOKEN_KIND_TO_STRING(TOKEN_JLE);
    TOKEN_KIND_TO_STRING(TOKEN_JG);
    TOKEN_KIND_TO_STRING(TOKEN_JGE);

    TOKEN_KIND_TO_STRING(TOKEN_LIT_I64);

    TOKEN_KIND_TO_STRING(TOKEN_OP_ADD);
    TOKEN_KIND_TO_STRING(TOKEN_OP_SUB);
    TOKEN_KIND_TO_STRING(TOKEN_OP_MUL);
    TOKEN_KIND_TO_STRING(TOKEN_OP_DIV);

    TOKEN_KIND_TO_STRING(TOKEN_PUNCT_PLUS);
    TOKEN_KIND_TO_STRING(TOKEN_PUNCT_MINUS);

    TOKEN_KIND_TO_STRING(TOKEN_DEL_SEMICOLON);
    TOKEN_KIND_TO_STRING(TOKEN_DEL_COLON);
    TOKEN_KIND_TO_STRING(TOKEN_DEL_QUOT_DUB);
    TOKEN_KIND_TO_STRING(TOKEN_DEL_QUOT_SIN);
    TOKEN_KIND_TO_STRING(TOKEN_DEL_DOT);
    TOKEN_KIND_TO_STRING(TOKEN_DEL_COMMA);
    TOKEN_KIND_TO_STRING(TOKEN_DEL_HASH);
    TOKEN_KIND_TO_STRING(TOKEN_DEL_EQ);
    TOKEN_KIND_TO_STRING(TOKEN_DEL_PIPE);
    TOKEN_KIND_TO_STRING(TOKEN_DEL_NEWLINE);
    TOKEN_KIND_TO_STRING(TOKEN_DEL_LPAREN);
    TOKEN_KIND_TO_STRING(TOKEN_DEL_RPAREN);
    TOKEN_KIND_TO_STRING(TOKEN_DEL_LCURLY);
    TOKEN_KIND_TO_STRING(TOKEN_DEL_RCURLY);
    TOKEN_KIND_TO_STRING(TOKEN_DEL_LSQUARE);
    TOKEN_KIND_TO_STRING(TOKEN_DEL_RSQUARE);

    TOKEN_KIND_TO_STRING(TOKEN_REGISTER_1);
    TOKEN_KIND_TO_STRING(TOKEN_REGISTER_2);
    TOKEN_KIND_TO_STRING(TOKEN_REGISTER_3);
    TOKEN_KIND_TO_STRING(TOKEN_REGISTER_4);
    TOKEN_KIND_TO_STRING(TOKEN_REGISTER_5);
    TOKEN_KIND_TO_STRING(TOKEN_REGISTER_6);
    TOKEN_KIND_TO_STRING(TOKEN_REGISTER_7);
    TOKEN_KIND_TO_STRING(TOKEN_REGISTER_8);
    TOKEN_KIND_TO_STRING(TOKEN_REGISTER_SP);
    TOKEN_KIND_TO_STRING(TOKEN_REGISTER_SB);

    TOKEN_KIND_TO_STRING(TOKEN_DEBUG);
    TOKEN_KIND_TO_STRING(TOKEN_COMMENT);

    TOKEN_KIND_TO_STRING(TOKEN_INVALID);
  }
  assert(0 && "Unreachable");
  return "Unknown";
}

#undef TOKEN_KIND_TO_STRING

static bool token_is_symbol_start(char c) { return isalpha(c) || c == '_'; }
static bool token_is_symbol(char c) { return isalnum(c) || c == '_'; }

static char lexer_char_get(Lexer *lexer) {
  return lexer->content[lexer->cursor];
}

static char lexer_char_step(Lexer *lexer) {
  assert(lexer->cursor < lexer->size);
  char tmp = lexer_char_get(lexer);
  // Maybe i need this twice ok?
  if (tmp == '\n') {
    lexer->line += 1;
    lexer->bol = lexer->cursor;
  }

  lexer->cursor++;
  return lexer_char_get(lexer);
}

static char lexer_trim_left(Lexer *lexer) {
  char current = lexer_char_get(lexer);
  while (lexer->cursor < lexer->size && isspace(current) && current != '\n') {
    current = lexer_char_step(lexer);
  }
  return lexer_char_get(lexer);
}

Token lexer_next(Lexer *l) {

  // First thing it does is cut the comming whitespace
  char current = lexer_trim_left(l);
  if (current == '\n') {
    l->line += 1;
    l->bol = ++l->cursor;

    return (Token){
        .kind = TOKEN_DEL_NEWLINE,
        .token = &l->content[l->cursor],
        .len = 1,
    };
  }
  // Endpoint check
  if (!(l->cursor < l->size)) {
    return (Token){.kind = TOKEN_EOF};
  }

  // Comments
  if (current == ';') {
    Token tk = {
        .kind = TOKEN_COMMENT,
        .token = &l->content[l->cursor],
        .len = 0,
    };
    while (lexer_char_get(l) != '\n') {
      tk.len++;
      lexer_char_step(l);
    }
    return tk;
  }

  // symbol handling
  if (token_is_symbol_start(current)) {
    Token t = {.kind = TOKEN_SYMBOL, .token = &l->content[l->cursor]};
    while (l->cursor < l->size && token_is_symbol(lexer_char_get(l))) {
      t.len += 1;
      lexer_char_step(l);
    }
    TokenKind k = keywords_get_token(t.len, t.token);
    if (k) {
      t.kind = k;
    }
    return t;
  }

  // digit handling
  if (isdigit(current)) {
    Token t = {.kind = TOKEN_LIT_I64, .token = &l->content[l->cursor]};
    while (l->cursor < l->size && isalnum(lexer_char_get(l))) {
      t.len += 1;
      lexer_char_step(l);
    }
    return t;
  }

  // Punctuation handling
  if (ispunct(current) ||
      current == '\n') { // TODO remove newline handling from punctuation
    for (size_t i = 0; i < punctuation_count; ++i) {
      if (lexer_char_get(l) == punctuation[i].c) {
        return (Token){
            .kind = punctuation[i].kind,
            .len = 1,
            .token = &l->content[l->cursor++],
        };
      }
    }
  }

  return (Token){
      .kind = TOKEN_INVALID,
      .len = 1,
      .token = &l->content[l->cursor++],
  };
}
