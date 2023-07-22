#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
void main(int argc, char *argv[])
{
 if (argc < 2)
 {
 fprintf(2, "Error!Usage: %s <num> \n", argv[0]);
 exit(1);
 }
 sleep(atoi(argv[1]));
 exit(0);
}