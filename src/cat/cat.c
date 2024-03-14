#include "Cat.h"

int main(int argc, char *argv[]) {
  const char *table[256];
  FlagsCat flagsValue = {0};

  if (FlagsIdentificationCat(argc, argv, &flagsValue)) {
    printf("Try 'cat --help' for more information.\n");
    return 0;
  }

  AsciiTableGenerate(table);
  TableModify(table, &flagsValue);
  WorkWithFile(argc, argv, table, &flagsValue);

  return 0;
}

/// @brief Определяем какие фалаги были введены
/// @param argc Кол-во аргументов командной строки
/// @param argv Аргументы командной строки
/// @return 0 - если все введенные флаги валидны
int FlagsIdentificationCat(int argc, char *argv[], FlagsCat *flagsValue) {
  struct option longopts[] = {{"number-nonblank", 0, 0, 'b'},
                              {"number", 0, 0, 'n'},
                              {"squeeze-blank", 0, 0, 's'},
                              {"show-ends", 0, 0, 'E'},
                              {"show-tabs", 0, 0, 'T'},
                              {"show-nonprinting", 0, 0, 'v'},
                              {0, 0, 0, 0}};
  int invalidInput = 0;
  for (int currentFlag = 0; currentFlag != -1;
       currentFlag = getopt_long(argc, argv, "bveEnstT", longopts, 0)) {
    if (currentFlag == 0 || currentFlag > 256) {
      continue;
    }

    switch (currentFlag) {
      case 'b':
        flagsValue->flag_b = 1;
        break;
      case 'e':
        flagsValue->flag_e = 1;
        flagsValue->flag_v = 1;
        break;
      case 'v':
        flagsValue->flag_v = 1;
        break;
      case 'E':
        flagsValue->flag_e = 1;
        break;
      case 'n':
        flagsValue->flag_n = 1;
        break;
      case 's':
        flagsValue->flag_s = 1;
        break;
      case 't':
        flagsValue->flag_t = 1;
        flagsValue->flag_v = 1;
        break;
      case 'T':
        flagsValue->flag_t = 1;
        break;
      default:
        invalidInput = 1;
        break;
    }
  }
  return invalidInput;
}

/// @brief Преобразование таблицы в зависимости от введенных флагов
/// @param table Таблица сгенерированя в AsciiTableGenerate
void TableModify(const char *table[static 256], FlagsCat *flagsValue) {
  if (flagsValue->flag_e) {
    UseFalgE(table);
  }
  if (flagsValue->flag_t) {
    UseFalgT(table);
  }
  if (flagsValue->flag_v) {
    UseFalgV(table);
  }
}

/// @brief Проверка наличия читаемого файла и его открытие
/// @param argc Кол-во аргументов командной строки
/// @param argv Аргументы командной строки
/// @param table Таблица сгенерированя в AsciiTableGenerate и преобразованная в
/// TableModify
void WorkWithFile(int argc, char *argv[], const char *table[static 256],
                  FlagsCat *flagsValue) {
  for (char **fileName = &argv[1], **end = &argv[argc]; fileName != end;
       ++fileName) {
    if (**fileName == '-') {
      continue;
    }

    FILE *file = fopen(*fileName, "r");

    if (errno) {
      fprintf(stderr, "%s: ", argv[0]);
      perror(*fileName);
      continue;
    }

    PrintFile(file, table, flagsValue);
    fclose(file);
  }
}

/// @brief Вывод на экран содержимого файла с модификациями флагов
/// @param file Читаемый файл
/// @param table Таблица сгенерированя в AsciiTableGenerate и преобразованная в
/// TableModify
void PrintFile(FILE *file, const char *table[static 256],
               FlagsCat *flagsValue) {
  int readSymbol = 0;
  int lastSymbolOfSrting = '\n';
  int secondEmptyStringInRow = 0;
  int lineNumber = 0;

  while (fread(&readSymbol, 1, 1, file) > 0) {
    if (lastSymbolOfSrting == '\n') {
      if (flagsValue->flag_s && readSymbol == '\n') {
        if (secondEmptyStringInRow) {
          continue;
        }
        secondEmptyStringInRow = 1;
      } else {
        secondEmptyStringInRow = 0;
      }
      if (flagsValue->flag_b) {
        if (readSymbol != '\n') {
          printf("%6d\t", ++lineNumber);
        }
      } else if (flagsValue->flag_n) {
        printf("%6d\t", ++lineNumber);
      }
    }

    if (!*table[readSymbol]) {
      printf("%c", '\0');
    }

    printf("%s", table[readSymbol]);
    lastSymbolOfSrting = readSymbol;
  }
}