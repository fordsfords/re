/* test_re.c - Tests for the re regular expression engine.
 * See https://github.com/fordsfords/re for documentation. */

/* This work is dedicated to the public domain under CC0 1.0 Universal:
 * http://creativecommons.org/publicdomain/zero/1.0/ */

#include "re.h"
#include <stdio.h>
#include <string.h>

static int tests_run = 0;
static int tests_passed = 0;

static void test_match(const char *desc, const char *pattern, const char *text,
                       int expect_idx, int expect_len) {
  tests_run++;
  re_t *re = re_compile(pattern);
  int idx = -1, len = -1;
  int matched = re_match(re, text, &idx, &len);
  re_free(re);

  if (matched && idx == expect_idx && len == expect_len) {
    tests_passed++;
  } else {
    printf("  FAIL: %s  pattern=\"%s\" text=\"%s\"\n", desc, pattern, text);
    printf("        expected match at idx=%d len=%d, got matched=%d idx=%d len=%d\n",
           expect_idx, expect_len, matched, idx, len);
  }
}

static void test_nomatch(const char *desc, const char *pattern, const char *text) {
  tests_run++;
  re_t *re = re_compile(pattern);
  int idx = -1, len = -1;
  int matched = re_match(re, text, &idx, &len);
  re_free(re);

  if (!matched) {
    tests_passed++;
  } else {
    printf("  FAIL: %s  pattern=\"%s\" text=\"%s\"\n", desc, pattern, text);
    printf("        expected no match, got idx=%d len=%d\n", idx, len);
  }
}

int main() {
  printf("Literals:\n");
  test_match("literal in middle", "hello", "say hello world", 4, 5);
  test_match("literal at start", "say", "say hello", 0, 3);
  test_match("literal at end", "world", "hello world", 6, 5);
  test_nomatch("literal no match", "xyz", "hello world");

  printf("Anchors:\n");
  test_match("^ match", "^hello", "hello world", 0, 5);
  test_nomatch("^ no match", "^hello", "say hello");
  test_match("$ match", "world$", "hello world", 6, 5);
  test_nomatch("$ no match", "world$", "world hello");
  test_match("^$ empty string", "^$", "", 0, 0);
  test_match("^$ exact match", "^hello$", "hello", 0, 5);
  test_nomatch("^$ no match", "^hello$", "say hello");

  printf("Dot:\n");
  test_match("dot basic", "h.llo", "hello", 0, 5);
  test_match("dot any char", "h.llo", "hxllo", 0, 5);
  test_nomatch("dot needs char", "h.llo", "hllo");
  test_nomatch("dot excludes newline", "h.llo", "h\nllo");
  test_match("dot matches cr", "h.llo", "h\rllo", 0, 5);
  test_match("dot-star stops at newline", "a.*b", "a foo b\nxb", 0, 7);
  test_nomatch("dot-star-dollar disrupted by newline", "a.*$", "a foo b\nxb");
  test_match("dot-star-dollar works without newline", "a.*$", "a foo b", 0, 7);

  printf("Dollar before trailing newline:\n");
  test_match("$ before trailing newline", "foo$", "foo\n", 0, 3);
  test_match("$ at true end", "foo$", "foo", 0, 3);
  test_nomatch("$ not before non-trailing newline", "foo$", "foo\nbar");
  test_match(".*$ before trailing newline", "a.*$", "a foo b\n", 0, 7);
  test_match("^$ on lone newline", "^$", "\n", 0, 0);

  printf("Star (greedy):\n");
  test_match("star zero", "ab*c", "ac", 0, 2);
  test_match("star one", "ab*c", "abc", 0, 3);
  test_match("star many", "ab*c", "abbbc", 0, 5);
  test_match("star greedy", "a.*b", "aXXbYYb", 0, 7);

  printf("Plus (greedy):\n");
  test_match("plus one", "ab+c", "abc", 0, 3);
  test_match("plus many", "ab+c", "abbbc", 0, 5);
  test_nomatch("plus zero", "ab+c", "ac");

  printf("Question (greedy):\n");
  test_match("question with", "ab?c", "abc", 0, 3);
  test_match("question without", "ab?c", "ac", 0, 2);
  test_match("question greedy takes one", "ab?", "ab", 0, 2);
  test_match("question greedy only one", "ab?", "abb", 0, 2);

  printf("Character classes:\n");
  test_match("class basic", "[abc]", "xbz", 1, 1);
  test_nomatch("class no match", "[abc]", "xyz");
  test_match("class range", "[a-z]", "5m9", 1, 1);
  test_nomatch("class range no match", "[a-z]", "589");
  test_match("class multi-range", "[a-zA-Z]", "5Hello", 1, 1);
  test_match("class literal dash start", "[-abc]", "x-z", 1, 1);
  test_match("class literal dash end", "[abc-]", "x-z", 1, 1);

  printf("Inverted character classes:\n");
  test_match("inv class basic", "[^abc]", "xbz", 0, 1);
  test_match("inv class skip", "[^abc]+", "abcxyz", 3, 3);
  test_nomatch("inv class no match", "[^a-z]", "abc");
  test_match("inv class digit", "[^a-z]", "a1b", 1, 1);

  printf("Escape sequences:\n");
  test_match("\\d digits", "\\d+", "abc123def", 3, 3);
  test_match("\\D non-digits", "\\D+", "123abc456", 3, 3);
  test_match("\\w word chars", "\\w+", "  hello  ", 2, 5);
  test_match("\\W non-word", "\\W+", "hello world", 5, 1);
  test_match("\\s whitespace", "\\s+", "hello world", 5, 1);
  test_match("\\S non-whitespace", "\\S+", "  hello  ", 2, 5);

  printf("Escaped metacharacters:\n");
  test_match("escaped dot", "\\.", "a.b", 1, 1);
  test_nomatch("escaped dot literal", "\\.", "abc");
  test_match("escaped star", "\\*", "a*b", 1, 1);
  test_match("escaped backslash", "\\\\", "a\\b", 1, 1);

  printf("Combined patterns:\n");
  test_match("two classes", "[abc][xyz]", "axbycz", 0, 2);
  test_match("class plus range", "[abc][0-9]+", "a42!", 0, 3);
  test_match("complex", "[Hh]ello [Ww]orld\\s*[!]?", "ahem.. 'hello world !' ..", 8, 13);

  printf("Zero-length matches:\n");
  test_match("star on empty", "a*", "", 0, 0);
  test_match("star zero-len in text", "x*", "abc", 0, 0);

  printf("NULL output parameters:\n");
  {
    tests_run++;
    re_t *re = re_compile("hello");
    int matched = re_match(re, "say hello", NULL, NULL);
    re_free(re);
    if (matched) {
      tests_passed++;
    } else {
      printf("  FAIL: NULL output params\n");
    }
  }

  printf("\n%d/%d tests passed.\n", tests_passed, tests_run);
  return (tests_passed == tests_run) ? 0 : 1;
}
