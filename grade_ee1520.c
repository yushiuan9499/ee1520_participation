
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct grade
{
  char id[16];
  char uid[32];
  char name[128];
  double hw[6];
  double midterm;
  double participation_01;
  double participation_02;
  double finalproject;
  double score;
  char lgrade[4];
};

struct grade mGA[256];

int
main(int argc, char *argv[])
{
  if (argc != 2)
    {
      fprintf(stderr, "need one and only one argument\n");
      exit(-1);
    }
  
  FILE *ip = fopen(argv[1], "r");
  int rc, i, j, num;
  bzero(mGA, 256 * sizeof(struct grade));

  if (ip == NULL) return -1;

  printf("[");
  for (i = 0; i < 256; i++)
    {
      char buf[2048];
      bzero(buf, 2048);

      char b0[256];
      bzero(b0, 256);
      char b1[256];
      bzero(b1, 256);
      char b2[256];
      bzero(b2, 256);
      char b3[256];
      bzero(b3, 256);
      char b4[256];
      bzero(b4, 256);
      char b5[256];
      bzero(b5, 256);
      char b6[256];
      bzero(b6, 256);
      char b7[256];
      bzero(b7, 256);
      char b8[256];
      bzero(b8, 256);
      char b9[256];
      bzero(b9, 256);

      struct grade *g = &(mGA[i]);
      rc = fscanf(ip, "%[^\n]", buf);
      if (rc == EOF) break;

      rc = sscanf(buf,
                  "%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]",
		  b0, b1, b2, b3, b4);

      printf("{");
      printf("\"name\":\"%s\",", b0);
      printf("\"id\":\"%s\",", b1);
      printf("\"vname\":\"%s\",", b2);
      printf("\"major\":\"%s\",", b3);
      printf("\"status\":\"%s\"", b4);
      printf("},");
      fgetc(ip);
    }
  fclose(ip);
  printf("]");
  return 0;
}
