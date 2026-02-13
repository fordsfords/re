# re

Small portable [Regular Expression](https://en.wikipedia.org/wiki/Regular_expression) (regex) library written in C. 

## Table of contents

<!-- mdtoc-start -->
&bull; [re](#re)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Table of contents](#table-of-contents)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Introduction](#introduction)  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&bull; [What I changed.](#what-i-changed)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Regular Expression Syntax](#regular-expression-syntax)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [Usage](#usage)  
&nbsp;&nbsp;&nbsp;&nbsp;&bull; [License](#license)  
<!-- TOC created by '../mdtoc/mdtoc.pl ./README.md' (see https://github.com/fordsfords/mdtoc) -->
<!-- mdtoc-end -->

## Introduction

This repo started as a copy of https://github.com/kokke/tiny-regex-c
but I wanted to make several changes that would be incompatible with
the original design goals, so I created my own version of it.
Note that kokke's tiny-regex-c repo is [public domain](https://unlicense.org/).

See [kkoke's repo](https://github.com/kokke/tiny-regex-c) for more information about
the original project.

### What I changed.

I deviated from the original goal of zero malloc/free.
I'm not writing for the embedded space anymore.

* `re_compile()` now mallocs a a stucture (two, actually).
  Added API for `re_free()`.
* The code is now thread-safe.
* Match now returns true for a match, false (0) for non-match.
* Now uses `E()` error handler to report internal errors rather than just counting it as a non-match.
* General code cleanup.

I did not change anything in the guts of the implementation.
The same expression syntax is accepted.
I am implicitly leveraging the deep testing done by kokke without
doing it myself.
This is not best practice.

## Regular Expression Syntax
 
This is a small but useful subset of full regular expressions.
 
```
* '.'        Dot, matches any character
* '^'        Start anchor, matches beginning of string
* '$'        End anchor, matches end of string
* '*'        Asterisk, match zero or more (greedy)
* '+'        Plus, match one or more (greedy)
* '?'        Question, match zero or one (non-greedy)
* '[abc]'    Character class, match if one of {'a', 'b', 'c'}
* '[^abc]'   Inverted class, match if NOT one of {'a', 'b', 'c'} -- NOTE: feature is currently broken!
* '[a-zA-Z]' Character ranges, the character set of the ranges { a-z | A-Z }
* '\s'       Whitespace, \t \f \r \n \v and spaces
* '\S'       Non-whitespace
* '\w'       Alphanumeric, [a-zA-Z0-9_]
* '\W'       Non-alphanumeric
* '\d'       Digits, [0-9]
* '\D'       Non-digits
```

## Usage

```c
#include "re.h"

  re_t *compiled_1;
  compiled_1 = re_compile("^ab[Cc]", 30);

  if (re_match(compiled_1, "abc")) printf("Success\n");
  if (re_match(compiled_1, "abC")) printf("Success\n");

  re_free(compiled_1);
```

## License

I want there to be NO barriers to using this code, so I am releasing it to the public domain.  But "public domain" does not have an internationally agreed upon definition, so I use CC0:

This work is dedicated to the public domain under CC0 1.0 Universal:
http://creativecommons.org/publicdomain/zero/1.0/

To the extent possible under law, Steven Ford has waived all copyright
and related or neighboring rights to this work. In other words, you can 
use this code for any purpose without any restrictions.
This work is published from: United States.
Project home: https://github.com/fordsfords/re
