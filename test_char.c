/* test_re.c - Tests for the re regular expression engine.
 * See https://github.com/fordsfords/re for documentation. */

/* This work is dedicated to the public domain under CC0 1.0 Universal:
 * http://creativecommons.org/publicdomain/zero/1.0/ */

#include "re.h"
#include <stdio.h>
#include <string.h>

int main() {
  char txt[] = "a.";
  int i;
  for (i = 1; i <= 255; i++) {
    txt[1] = i;

    /*
     * WARNING: The following code leaks memory! The re_t structure returned
     * by re_compile() is not saved anywhere, and the malloced memory is never
     * freed until the program exits. This is usually terrible practice, but
     * for the purposes of this test program, it produces compact source code
     * that corresponds nicely to the Perl version of this program.
     */

    printf("%d=", i);
    if (re_match(re_compile("a."), txt, NULL, NULL)) {
      printf(".");
    }
    if (re_match(re_compile("a\\s"), txt, NULL, NULL)) {
      printf("s");
    }
    if (re_match(re_compile("a\\S"), txt, NULL, NULL)) {
      printf("S");
    }
    if (re_match(re_compile("a\\w"), txt, NULL, NULL)) {
      printf("w");
    }
    if (re_match(re_compile("a\\W"), txt, NULL, NULL)) {
      printf("W");
    }
    if (re_match(re_compile("a\\d"), txt, NULL, NULL)) {
      printf("d");
    }
    if (re_match(re_compile("a\\D"), txt, NULL, NULL)) {
      printf("D");
    }
    if (re_match(re_compile("a$"), txt, NULL, NULL)) {
      printf("$");
    }
    if (re_match(re_compile("a[\\s]"), txt, NULL, NULL)) {
      printf("[s]");
    }
    if (re_match(re_compile("a[\\S]"), txt, NULL, NULL)) {
      printf("[S]");
    }
    if (re_match(re_compile("a[\\w]"), txt, NULL, NULL)) {
      printf("[w]");
    }
    if (re_match(re_compile("a[\\W]"), txt, NULL, NULL)) {
      printf("[W]");
    }
    if (re_match(re_compile("a[\\d]"), txt, NULL, NULL)) {
      printf("[d]");
    }
    if (re_match(re_compile("a[\\D]"), txt, NULL, NULL)) {
      printf("[D]");
    }
    if (re_match(re_compile("a[a-z]"), txt, NULL, NULL)) {
      printf("[a-z]");
    }
    if (re_match(re_compile("a[A-Z]"), txt, NULL, NULL)) {
      printf("[A-Z]");
    }
    if (re_match(re_compile("a[0-9]"), txt, NULL, NULL)) {
      printf("[0-9]");
    }
    printf("\n");
  }
  return 0;
}
