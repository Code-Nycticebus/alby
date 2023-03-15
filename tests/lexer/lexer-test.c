#include "lexer/lexer.h"

#include "lexer-output.c"
#include "lexer/token.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static const char *input[] = {
    // Small program 1
    "mov r1, 500\n"
    "mov r2, 80\n"
    "sub r1, r2\n",
    // Small program 2
    "mov r1, 34\n add r1, 35\n",

    // Syscall
    "exit 0\n",
    // Ambitions
    "i64 a = 420\n", // -> push 420
};

static size_t input_count = sizeof(input) / sizeof(input[0]);

static void generate_output(Lexer *lexer, FILE *output) {
  fprintf(output, "  (TokenKind[]){\n");
  for (Token t = lexer_next(lexer); t.kind != TOKEN_EOF;
       t = lexer_next(lexer)) {
    fprintf(output, "    %s,\n", token_kind_to_string(t.kind));
  }
  fprintf(output, "    TOKEN_EOF, \n");
  fprintf(output, "  },\n");
}

static void generate_file(void) {
  FILE *file = fopen("./tests/lexer/lexer-output.c", "w");
  if (file == NULL) {
    perror("Could not open file!");
    return;
  }

  fprintf(file,
          "#include \"lexer/token.h\"\n"
          "/* This is the generated output of my lexer. */\n"
          "\n"
          "const size_t output_tokens_count = %ld;\n"
          "\n"
          "const TokenKind *output_tokens[] = {\n",
          input_count);

  for (size_t i = 0; i < input_count; i++) {
    generate_output((Lexer[]){lexer_init(strlen(input[i]), input[i])}, file);
  }

  fprintf(file, "};\n");

  fclose(file);
}

typedef struct Failure {
  bool is_ok;
  size_t line;
  size_t row;
  size_t len;
  const char *bol;
  TokenKind expected;
  TokenKind result;
} Failure;

static void _repeat(int times, const char c) {
  for (int i = 0; i < times; i++) {
    (void)putchar(c);
  }
}

static void failure_display(Failure *fail) {
  printf("line: %ld\n", fail->line);

  printf("%s -> %s\n", token_kind_to_string(fail->expected),
         token_kind_to_string(fail->result));
  int line_len = 0;
  while (fail->bol[line_len] != '\0' && fail->bol[line_len] != '\n') {
    ++line_len;
  }
  printf("%.*s\n", line_len, fail->bol);

  _repeat((int)fail->row, ' ');
  _repeat((int)fail->len, '^');
  putchar('\n');
}

/* Compares the lexers-token stream output with the tokens it should produce. */
static Failure compare_output(Lexer *lexer, const TokenKind *output) {
  Token t = lexer_next(lexer);
  // Iterates through tokens and lexers, until one of them hits a TOKEN_EOF
  for (int i = 0; output[i] != TOKEN_EOF; i++) {
    if (output[i] != t.kind || t.kind == TOKEN_INVALID) {
      return (Failure){
          .is_ok = false,
          .line = lexer->line,
          .row = (lexer->cursor - t.len) - lexer->bol,
          .len = t.len,
          .bol = &lexer->content[lexer->bol],
          .result = t.kind,
          .expected = output[i],
      };
    }
    t = lexer_next(lexer);
  }
  // the two streams are the same if both streams are TOKEN_EOF
  return (Failure){.is_ok = (t.kind == TOKEN_EOF), .bol = NULL};
}

int main(int argc, char **argv) {
  (void)argv;
  // if any commandline arguments come in, just generate output
  bool generate_output_flag =
      argc > 1 && argv[1][0] == '-' && argv[1][1] == 'u'; // -u flag

  if (generate_output_flag || output_tokens_count != input_count) {
    generate_file();
    return 0;
  }

  // Iterates over inputs
  for (size_t i = 0; i < input_count; ++i) {
    // Creates new lexer
    Lexer lexer = lexer_init(strlen(input[i]), input[i]);
    Failure f = compare_output(&lexer, output_tokens[i]);
    if (f.is_ok == false) {
      printf("Failure input %ld\n", i + 1);

      if (f.bol != NULL) {
        failure_display(&f);
      } else {
        printf("Output was too short\n");
      }
    }
  }

  return 0;
}
