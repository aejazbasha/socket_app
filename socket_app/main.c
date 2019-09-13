#include <stdio.h>
#include <stdlib.h>

struct data
{
  int rollno;
  int result;
}d;

int upload(struct data*);

int main(int argc, char const *argv[])
{

  int num = 3;
  int sq_num = num*num;

  printf("enter your rollno\n");
  scanf("%d", &d.rollno);

  d.result = sq_num;

  printf("In main rollno:%d result:%d\n", d.rollno, d.result);
  upload(&d);

  return 0;
}
