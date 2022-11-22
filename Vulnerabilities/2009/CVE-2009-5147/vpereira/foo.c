#include <stdlib.h>

char * foo() {
  return getenv("LOGNAME");
}
