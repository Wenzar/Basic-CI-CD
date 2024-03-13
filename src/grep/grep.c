#include "Grep.h"

int main(int argc, char **argv) {
  if (argc > 1) {
    char *pattern = NULL;
    FlagGrep flagsValue = {0};

    for (int InputArgs =
             FlagsIdentificationGrep(argc, argv, &flagsValue, &pattern);
         InputArgs < argc; InputArgs++) {
      ReadFile(argv[InputArgs], flagsValue, pattern);
    }

    if (pattern != NULL) {
      free(pattern);
    }
  }
  return 0;
}

/// @brief Парсинг флагов поехали
/// @param argc
/// @param argv
/// @param flagsValue структуру FlagGrep
/// @param pattern
/// @return Индекс первого элемента argv, не являющегося опцией
int FlagsIdentificationGrep(int argc, char **argv, FlagGrep *flagsValue,
                            char **pattern) {
  for (int currentFlag = 0; currentFlag != -1;
       currentFlag = getopt(argc, argv, "ivclnhsoe:f:")) {
    switch (currentFlag) {
      case 'i':
        flagsValue->flag_i = 1;
        break;
      case 'v':
        flagsValue->flag_v = 1;
        break;
      case 'c':
        flagsValue->flag_c = 1;
        break;
      case 'l':
        flagsValue->flag_l = 1;
        break;
      case 'n':
        flagsValue->flag_n = 1;
        break;
      case 'h':
        flagsValue->flag_h = 1;
        break;
      case 's':
        flagsValue->flag_s = 1;
        break;
      case 'o':
        flagsValue->flag_o = 1;
        break;
      case 'e':
        flagsValueE(flagsValue, pattern, optarg);
        break;
      case 'f':
        flagsValueF(flagsValue, pattern, optarg);
        break;
    }
  }
  if ((flagsValue->flag_e || flagsValue->flag_f) == 0) {
    BufferForPattern(pattern, argv[optind]);
    ++optind;
  }
  SwitchFlagOToZero(flagsValue, argc, optind);
  return optind;
}

/// @brief Считываем содержимое файла
/// @param filePath
/// @param flagsValue
/// @param pattern
void ReadFile(char *filePath, FlagGrep flagsValue, char *pattern) {
  FILE *file = fopen(filePath, "r");

  if (file != NULL) {
    for (char symbol = '0'; (symbol = getc(file)) != EOF;) {
      ++flagsValue.numberLine;
      int length = 0;
      char *text = calloc(256, 1);

      if (text == NULL) {
        printf("Can't allocate memory\n");
        exit(0);
      }

      for (text[length] = '\0'; symbol != EOF && symbol != '\n';
           symbol = getc(file)) {
        text[length] = symbol;
        text[++length] = '\0';

        if (length % 255 == 0) {
          text = realloc(text, length + 256);

          if (text == NULL) {
            printf("Can't allocate memory\n");
            exit(0);
          }
        }
      }

      TextPrintFlags(text, &flagsValue, pattern, filePath);
      free(text);
    }
    flagsValueLC(flagsValue, filePath);
    fclose(file);
  } else if (flagsValue.flag_s == 0) {
    fprintf(stderr, ERROR, filePath);
  }
}

/// @brief Флаги дополняющие вывод текстовых совпадений
/// @param text
/// @param flagsValue
/// @param pattern
/// @param filePath
void TextPrintFlags(char *text, FlagGrep *flagsValue, char *pattern,
                    char *filePath) {
  regex_t regex;

  if (regcomp(&regex, pattern, flagsValue->flag_i ? REG_ICASE : REG_EXTENDED) ==
      0) {
    if (regexec(&regex, text, 0, NULL, 0) == flagsValue->flag_v) {
      flagsValue->countMatches++;

      if ((flagsValue->flag_c || flagsValue->flag_l) == 0) {
        if (flagsValue->countFiles > 1 && !(flagsValue->flag_h)) {
          printf("%s:", filePath);
        }

        if (flagsValue->flag_n) {
          printf("%i:", flagsValue->numberLine);
        }

        if (!flagsValue->flag_o) {
          printf("%s\n", text);
        } else {
          flagsValueO(regex, text, *flagsValue, filePath);
        }
      }
    }
    regfree(&regex);
  }
}

/// @brief Реализация флага F
/// @param flagsValue
/// @param pattern
/// @param optarg
void flagsValueF(FlagGrep *flagsValue, char **pattern, char *optarg) {
  FILE *file = fopen(optarg, "r");
  if (file != NULL) {
    for (char symbol = '0'; (symbol = getc(file)) != EOF;) {
      int length = 0;
      char *line = calloc(256, 1);

      if (line == NULL) {
        printf("Can't allocate memory\n");
        exit(0);
      }

      for (line[length] = '\0'; symbol != EOF && symbol != '\n';
           symbol = getc(file)) {
        line[length] = symbol;
        line[length += 1] = '\0';
        if (length % 255 == 0) line = (char *)realloc(line, length + 256);

        if (line == NULL) {
          printf("Can't allocate memory\n");
          exit(0);
        }
      }

      if (!(flagsValue->flag_e || flagsValue->flag_f)) {
        flagsValue->flag_f = BufferForPattern(pattern, line);
      } else {
        AddPatternToBuffer(pattern, line);
      }

      free(line);
    }
    fclose(file);
  } else if (flagsValue->flag_s == 0) {
    fprintf(stderr, ERROR, optarg);
  }
}
/// @brief Реализация флага E
/// @param flagsValue
/// @param pattern
/// @param optarg
void flagsValueE(FlagGrep *flagsValue, char **pattern, char *optarg) {
  if (!(flagsValue->flag_e || flagsValue->flag_f)) {
    flagsValue->flag_e = BufferForPattern(pattern, optarg);
  } else {
    AddPatternToBuffer(pattern, optarg);
  }
}

/// @brief Обработка флага С и L
/// @param flagsValue
/// @param filePath
void flagsValueLC(FlagGrep flagsValue, char *filePath) {
  if (flagsValue.flag_c) {
    if (flagsValue.flag_l) {
      if (flagsValue.countFiles > 1 && flagsValue.countMatches != 0) {
        printf("%s\n", filePath);
      }
    } else {
      if (flagsValue.countFiles > 1 && !flagsValue.flag_h) {
        printf("%s:", filePath);
      }
      printf("%i\n", flagsValue.countMatches);
    }
  }
  if (flagsValue.flag_l && flagsValue.countMatches) {
    printf("%s\n", filePath);
  }
}

/// @brief Обработка флага O
/// @param regex
/// @param line
/// @param flagsValue
/// @param filePath
void flagsValueO(regex_t regex, char *line, FlagGrep flagsValue,
                 char *filePath) {
  int countMatchesInLine = 0;

  while (regexec(&regex, line, 0, NULL, 0) == flagsValue.flag_v) {
    int start = 0;
    int end = strlen(line);
    char *buff = calloc(strlen(line) + 1, 1);

    if (countMatchesInLine > 0 && flagsValue.countFiles > 1 &&
        !flagsValue.flag_h) {
      printf("%s:", filePath);
    }

    if (buff == NULL) {
      printf("Can't allocate memory\n");
      exit(0);
    }
    strcpy(buff, line);

    while (regexec(&regex, buff, 0, NULL, 0) == 0) {
      end--;
      buff[strlen(buff) - 1] = 0;
    }
    buff[strlen(buff)] = line[strlen(buff)];

    while (regexec(&regex, buff, 0, NULL, 0) == 0 && strlen(buff) > 0) {
      int j = 0;

      while (buff[j] != 0) {
        buff[j] = buff[j + 1];
        j++;
      }
      start++;
    }
    start--;
    int buffSize = strlen(buff);

    while (buffSize != 0) {
      buff[buffSize] = buff[buffSize - 1];
      buffSize--;
    }
    buff[0] = line[start];
    printf("%s\n", buff);
    free(buff);
    buffSize = start + 1;

    while (line[buffSize] != 0) {
      line[buffSize - start - 1] = line[buffSize];
      buffSize++;
    }
    line[buffSize - start - 1] = 0;
    ++countMatchesInLine;
  }
}

/// @brief Если с флагом -о вводятся флаги -l,-v,-c флаг -о сделаем не активным
/// @param flagsValue
/// @param argc
/// @param optind
void SwitchFlagOToZero(FlagGrep *flagsValue, int argc, int optind) {
  if (flagsValue->flag_o &&
      (flagsValue->flag_l || flagsValue->flag_v || flagsValue->flag_c))
    flagsValue->flag_o = 0;
  flagsValue->countFiles = argc - optind;
}

/// @brief Создаем буффер для вводимых паттернов
/// @param pattern
/// @param optarg
/// @return Получилось ли выделить память
int BufferForPattern(char **pattern, char *optarg) {
  *pattern = calloc(strlen(optarg) + 1, 1);

  if (pattern == NULL) {
    printf("Can't allocate memory\n");
    exit(0);
  }
  strcpy(*pattern, optarg);
  return pattern ? 1 : 0;
}

/// @brief Добавляем паттерны к уже имеющимся
/// @param pattern
/// @param optarg
void AddPatternToBuffer(char **pattern, char *optarg) {
  *pattern = realloc(*pattern, strlen(*pattern) + strlen(optarg) + 2);

  if (pattern == NULL) {
    printf("Can't allocate memory\n");
    exit(0);
  }

  if (*pattern) {
    strcat(strcat(*pattern, "|"), optarg);
  }
}
