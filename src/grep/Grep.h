#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR "grep: %s: No such file or directory\n"

typedef struct {
  int flag_i;
  int flag_v;
  int flag_c;
  int flag_l;
  int flag_n;
  int flag_h;
  int flag_s;
  int flag_o;
  int flag_e;
  int flag_f;
  int countFiles;
  int numberLine;
  int countMatches;
} FlagGrep;

void ReadFile(char *name, FlagGrep flagsValue, char *tmpl);
int FlagsIdentificationGrep(int argc, char **argv, FlagGrep *flagsValue,
                            char **pattern);
void TextPrintFlags(char *str, FlagGrep *FlagGrep, char *tmpl, char *name);
void flagsValueF(FlagGrep *flagsValue, char **pattern, char *optarg);
void flagsValueE(FlagGrep *flagsValue, char **pattern, char *optarg);
void flagsValueO(regex_t my_regex, char *str, FlagGrep flagsValue, char *path);
void flagsValueLC(FlagGrep flagsValue, char *path);
void SwitchFlagOToZero(FlagGrep *flagsValue, int argc, int optind);
int BufferForPattern(char **str, char *optarg);
void AddPatternToBuffer(char **str, char *optarg);

#endif