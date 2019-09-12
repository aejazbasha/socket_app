#include <stdio.h>
#include <stdlib.h>

int upload(int, int);

int main(int argc, char const *argv[]) {

  int rollno = 103;
  int num = 3;
  int result = num * num;
  upload(rollno, result);

  return 0;
}
