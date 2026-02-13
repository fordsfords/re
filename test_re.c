/* test_re.c
 */


#include "re.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char **argv) {
  re_t *compiled_1;
  re_t *compiled_2;
  re_t *compiled_3;

  compiled_1 = re_compile("^abc", 30);  E(compiled_1 == NULL);
  compiled_2 = re_compile("abc$", 30);  E(compiled_2 == NULL);
  compiled_3 = re_compile("abc", 30);  E(compiled_3 == NULL);

  E(!re_match(compiled_1, "abc"));
  E(!re_match(compiled_1, "abcxyz"));
  E(re_match(compiled_1, "aabc"));
  E(re_match(compiled_1, "ab"));
  E(re_match(compiled_1, "abC"));

  E(!re_match(compiled_2, "abc"));
  E(!re_match(compiled_2, "xyzabc"));
  E(re_match(compiled_2, "abcc"));
  E(re_match(compiled_2, "bc"));
  E(re_match(compiled_2, "Abc"));

  E(!re_match(compiled_3, "abc"));
  E(!re_match(compiled_3, "xyzabc"));
  E(!re_match(compiled_3, "abcc"));
  E(!re_match(compiled_3, "xyzabc123"));
  E(re_match(compiled_3, "bc"));
  E(re_match(compiled_3, "Abc"));

  re_free(compiled_1);
  re_free(compiled_2);
  re_free(compiled_3);

  printf("All pass.\n");
  return 0;
}  /* main */
