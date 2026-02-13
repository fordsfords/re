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
  int idx, len;

  compiled_1 = re_compile("^abc", 30);  E(compiled_1 == NULL);
  compiled_2 = re_compile("abc$", 30);  E(compiled_2 == NULL);
  compiled_3 = re_compile("abc", 30);  E(compiled_3 == NULL);

  E(!re_match(compiled_1, "abc", NULL, NULL));
  idx = -1;  len = -2;
  E(!re_match(compiled_1, "abcxyz", &idx, &len));  E(idx != 0);  E(len != 3);
  idx = -1;  len = -2;
  E(re_match(compiled_1, "aabc", &idx, &len));  E(idx != -1);  E(len != -2);
  E(re_match(compiled_1, "ab", NULL, NULL));
  E(re_match(compiled_1, "abC", NULL, NULL));

  idx = -1;  len = -2;
  E(!re_match(compiled_2, "abc", &idx, &len));  E(idx != 0);  E(len != 3);
  idx = -1;  len = -2;
  E(!re_match(compiled_2, "xyzabc", NULL, &len));  E(idx != -1);  E(len != 3);
  E(re_match(compiled_2, "abcc", NULL, NULL));
  E(re_match(compiled_2, "bc", NULL, NULL));
  E(re_match(compiled_2, "Abc", NULL, NULL));

  E(!re_match(compiled_3, "abc", NULL, NULL));
  idx = -1;  len = -2;
  E(!re_match(compiled_3, "xyzabc", &idx, NULL));  E(idx != 3);  E(len != -2);
  E(!re_match(compiled_3, "abcc", NULL, NULL));
  E(!re_match(compiled_3, "xyzabc123", NULL, NULL));
  E(re_match(compiled_3, "bc", NULL, NULL));
  E(re_match(compiled_3, "Abc", NULL, NULL));

  re_free(compiled_1);
  re_free(compiled_2);
  re_free(compiled_3);

  printf("All pass.\n");
  return 0;
}  /* main */
