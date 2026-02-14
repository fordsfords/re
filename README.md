# re

Small portable [Regular Expression](https://en.wikipedia.org/wiki/Regular_expression) (regex) library written in C. 

## Table of contents

<!-- mdtoc-start -->
&bull; [re](#re)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Table of contents](#table-of-contents)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Introduction](#introduction)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [What Is Changed](#what-is-changed)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Known Limitations](#known-limitations)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Regular Expression Syntax](#regular-expression-syntax)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [API](#api)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [re_compile](#re_compile)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [re_match](#re_match)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [re_free](#re_free)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Example](#example)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Error Handling](#error-handling)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [License](#license)  
<!-- TOC created by '../mdtoc/mdtoc.pl ./README.md' (see https://github.com/fordsfords/mdtoc) -->
<!-- mdtoc-end -->

## Introduction

This repo started as a copy of https://github.com/kokke/tiny-regex-c
but I wanted to make several changes that would be incompatible with
the original design goals, so I evolved my own version of it.
Note that kokke's tiny-regex-c repo is [public domain](https://unlicense.org/).

See [kokke's repo](https://github.com/kokke/tiny-regex-c) for more information about
the original project.

Big thanks to kokke for writing it and putting it in the public domain,
and to Claude.ai for assisting me!
I hereby certify that I have personally reviewed and tested all code
written by Claude.ai during this collaboration.

### What Is Changed

I'm not writing for the embedded space, and I wanted a more convenient API.

* `re_compile()` now mallocs a structure (two, actually).
  - Added API for `re_free()`.
  - This allows having multiple compiled patterns active.
* The code is now thread-safe.
* `re_match()` returns 1 for a match, 0 for non-match.
  - Match index and length are returned via output parameters.
* Now uses `E()` error handler to report internal errors
  rather than silently counting them as non-matches.
* Question mark is now "greedy".
* Dot now matches carriage return but not newline.
  - This matches what perl/python do by default.
* `$` now matches at end of string or just before a trailing newline.
  - THis matches perl/python default behavior.
* General code cleanup.

The same expression syntax is accepted.
Almost all of the core code is unchanged;
I am implicitly leveraging kokke's deep testing without doing it myself;
I admit this is not best practice (I should re-do it).

## Known Limitations

This module implements a useful subset of regular expression functionality.
But it is not complete.

* Alternation (`|`) is not supported.
* No {m,n} quantifiers.
* No support for capturing groups or named captures.
* Anchors ^ and $ used mid-pattern produce undefined behavior instead of being treated correctly. 
* The character class buffer has a compile-time maximum length of 256
  (shared across all character classes in a single pattern).
  Patterns that exceed this will trigger an error.
* `E()` calls `exit(1)` on error. There is no way to recover from
  a malformed pattern or allocation failure at runtime.
  (This can be changed; see [Error Handling](#error-handling))
* A variety of invalid patterns are not detected and instead just fail to match.
  The caller is expected to pass in valid patterns.
  - For example, dangling qualifiers (like "*" without a token in front of it).
  - Another example: "(abc)|(xyz)" simply matches those literal characters;
    it doesn't warn that it is using unsupported RE functionality.

If you need a richer implementation, consider [PCRE](https://www.pcre.org/).

## Regular Expression Syntax

This is a small but useful subset of full regular expressions.

| Pattern      | Description |
|:-------------|:------------|
| `.`          | Dot, matches any character except newline (`\n`) |
| `^`          | Start anchor, matches beginning of string |
| `$`          | End anchor, matches end of string |
| `*`          | Asterisk, match zero or more (greedy) |
| `+`          | Plus, match one or more (greedy) |
| `?`          | Question, match zero or one (greedy) |
| `[abc]`      | Character class, match if one of {'a', 'b', 'c'} |
| `[^abc]`     | Inverted class, match if NOT one of {'a', 'b', 'c'} |
| `[a-zA-Z]`   | Character ranges, the character set of the ranges { a-z \| A-Z } |
| `\s`         | Whitespace, \t \f \r \n \v and spaces |
| `\S`         | Non-whitespace |
| `\w`         | Alphanumeric, [a-zA-Z0-9_] |
| `\W`         | Non-alphanumeric |
| `\d`         | Digits, [0-9] |
| `\D`         | Non-digits |

A backslash before any other character matches that character literally,
allowing you to escape metacharacters like `\.`, `\*`, `\[`, etc.

Remember that when coding a pattern in a C program, you need to escape the
backslash. For example, to code the pattern "d+\.d+", you need to use:
```c
re_t *re = re_compile("d+\\.d+");
```

## API

```c
#include "re.h"
```

### re_compile

```c
re_t *re_compile(const char* pattern);
```

Compiles a regex pattern string into an internal representation for matching.

Returns a pointer to an allocated `re_t` structure.
The caller must eventually pass this pointer to `re_free()`.

On internal error (e.g. allocation failure, malformed pattern),
the function prints a diagnostic to stderr and calls `exit(1)`.

### re_match

```c
int re_match(re_t *re, const char* text, int *idx_out, int *len_out);
```

Searches `text` for the first match of the compiled pattern `re`.

Returns 1 if a match is found, 0 otherwise.

On a successful match, `*idx_out` is set to the index of the first
matching character in `text`, and `*len_out` is set to the length
of the match.
Either output pointer may be NULL if you don't need that value.

### re_free

```c
void re_free(re_t *re);
```

Frees the memory allocated by `re_compile()`.

## Example

```c
#include "re.h"

int main() {
  re_t *pattern = re_compile("[Hh]ello [Ww]orld\\s*[!]?");

  int idx, len;
  if (re_match(pattern, "ahem.. 'hello world !' ..", &idx, &len)) {
    printf("match at idx %d, %d chars long.\n", idx, len);
  }

  /* You can also ignore the output parameters: */
  if (re_match(pattern, "Hello World", NULL, NULL)) {
    printf("matched.\n");
  }

  re_free(pattern);
  return 0;
}
```

## Error Handling

The `E()` macro is used throughout in the compile code
for error conditions such as allocation failures, buffer overflows,
and malformed patterns.
If the condition passed to `E()` is true, it prints the file, line number,
and the failing expression to stderr, then calls `exit(1)`.

```c
#define E(e_expr_) do { \
  if (e_expr_) { \
    fprintf(stderr, "ERROR [%s:%d]: '%s'\n", __FILE__, __LINE__, #e_expr_); \
    exit(1); \
  } \
} while (0)
```

While this form of error handling serves my needs,
there is a valid argument that a pure library function should not `exit()`.
Perhaps shouldn't even `fprintf()` - for example,
embedded systems generally don't have stderr defined.

Feel free to replace `E()` with whatever you want.
For example, you could change it to `E_GO()` and replace the
`exit()` with `goto e_go`. Then include an `e_go:` label
with proper cleanup and `return NULL`. Note that the caller
has to then check for errors and ... do what? Recover? How?

This makes the most sense for an interactive program where the user
enters the pattern - you don't want to exit if the user makes
a mistake. But for a daemon or tool, the only advantage of
passing back errors is that the caller could do its own cleanup
(e.g. gracefully closing files) before exiting.

## License

I want there to be NO barriers to using this code, so I am releasing it to the public domain.  But "public domain" does not have an internationally agreed upon definition, so I use CC0:

This work is dedicated to the public domain under CC0 1.0 Universal:
http://creativecommons.org/publicdomain/zero/1.0/

To the extent possible under law, Steven Ford has waived all copyright
and related or neighboring rights to this work. In other words, you can 
use this code for any purpose without any restrictions.
This work is published from: United States.
Project home: https://github.com/fordsfords/re
