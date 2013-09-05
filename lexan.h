#ifndef LEXICAL_ANALYZER_H
#define LEXICAl_ANALYZER_H

#include <stdio.h>

#define MAX_TOKEN_LENGTH 101

/*
This type defines the basic token produced by the
Lexical Analyzer 
*/
typedef struct Token {
  int type;
  void *value;
  struct Token *next_token;
} Token;

Token *analyze(FILE *file);



#endif
