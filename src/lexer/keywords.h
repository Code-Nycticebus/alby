#ifndef _KEYWORDS_H
#define _KEYWORDS_H

#include <stddef.h>

#include "token.h"

void keywords_init(void);
TokenKind keywords_get_token(size_t size, const char *str);

#endif // !_KEYWORDS_H
