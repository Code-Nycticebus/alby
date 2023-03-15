#include "lexer/token.h"
/* This is the generated output of my lexer. */

const size_t output_tokens_count = 4;

const TokenKind *output_tokens[] = {
  (TokenKind[]){
    TOKEN_MOV,
    TOKEN_REGISTER_1,
    TOKEN_DEL_COMMA,
    TOKEN_LIT_I64,
    TOKEN_DEL_NEWLINE,
    TOKEN_MOV,
    TOKEN_REGISTER_2,
    TOKEN_DEL_COMMA,
    TOKEN_LIT_I64,
    TOKEN_DEL_NEWLINE,
    TOKEN_OP_SUB,
    TOKEN_REGISTER_1,
    TOKEN_DEL_COMMA,
    TOKEN_REGISTER_2,
    TOKEN_DEL_NEWLINE,
    TOKEN_EOF, 
  },
  (TokenKind[]){
    TOKEN_MOV,
    TOKEN_REGISTER_1,
    TOKEN_DEL_COMMA,
    TOKEN_LIT_I64,
    TOKEN_DEL_NEWLINE,
    TOKEN_OP_ADD,
    TOKEN_REGISTER_1,
    TOKEN_DEL_COMMA,
    TOKEN_LIT_I64,
    TOKEN_DEL_NEWLINE,
    TOKEN_EOF, 
  },
  (TokenKind[]){
    TOKEN_EXIT,
    TOKEN_LIT_I64,
    TOKEN_DEL_NEWLINE,
    TOKEN_EOF, 
  },
  (TokenKind[]){
    TOKEN_SYMBOL,
    TOKEN_SYMBOL,
    TOKEN_DEL_EQ,
    TOKEN_LIT_I64,
    TOKEN_DEL_NEWLINE,
    TOKEN_EOF, 
  },
};
