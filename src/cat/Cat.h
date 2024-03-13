#ifndef CAT_H
#define CAT_H

#include <errno.h>
#include <getopt.h>
#include <stdio.h>

typedef struct {
  int flag_b;
  int flag_e;
  int flag_n;
  int flag_s;
  int flag_t;
  int flag_v;
} FlagsCat;

int FlagsIdentificationCat(int argc, char *argv[], FlagsCat *flagsValue);
void AsciiTableGenerate(const char *table[static 256]);
void UseFalgE(const char *table[static 256]);
void UseFalgT(const char *table[static 256]);
void UseFalgV(const char *table[static 256]);

void TableModify(const char *table[static 256], FlagsCat *flagsValue);
void WorkWithFile(int argc, char *argv[], const char *table[static 256],
                  FlagsCat *flagsValue);
void PrintFile(FILE *file, const char *table[static 256], FlagsCat *flagsValue);

#endif