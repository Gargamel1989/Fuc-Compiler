#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 101

/*
This type defines the basic token produced by the
Lexical Analyzer 
*/
typedef struct {
  int type;
  void *value;
  struct Token *next_token;
} Token;

static int line_number = 1;

char get_next_char(FILE *file) {
  char c = getc(file);
  if (c == '\n') {
    line_number++;
  }
  return c;
}

int main(int argc, char *argv[]) {
  
  // We only allow 1 file to be compiled for now
  if (argc != 2) {
    printf("Usage: %s filename\n", argv[0]);
    return 1;
  }

  char *filename = argv[1];
  FILE *file;

  // Check if the file is readable
  if ((file = fopen(filename, "r")) == NULL) {
    printf("Error accessing '%s'\n", argv[1]);
    return 1;
  }
  
  /*
    Do Lexical Analysis on the source file, producing
    a linked list of tokens
  */
  // The first token of the linked list
  Token *first_token = NULL;
  // The token parsed before the current token
  Token *prev_token;
  // This booleans remembers across a loop if a new token
  // should be started
  int char_type;

  char c;
  char token_value[MAX_TOKEN_LENGTH] = {};
  int token_value_index = 0;
  do {
    c = get_next_char(file);

    // Handle comments
    if (c == '/') {
      // This should be the start of a comment, otherwise it
      // is an illegal character
      c = get_next_char(file);
      if (c == '/') {
	// Inline comment
	while (c != '\n' && c != EOF) {
	  c = get_next_char(file);
	}
      } else if (c == '*') {
	// Multiline comment
	int started = 0;
	while (1) {
	  c = get_next_char(file);
	  if (c == EOF) {
	    printf("Line %d: Premature end of file (Multiline comment not ended)\n", line_number);
	    return 1;
	  }
	  if (c == '*') {
	    started = 1;
	  } else if (c == '/' && started) {
	    // Comment has ended, make sure a new token is
	    // generated
	    c = '\n';
	    break;
	  } else {
	    started = 0;
	  }
	}
      } else {
	// Error
	printf("Line %d: Illegal character '/'\n", line_number);
	return 1;
      }
    }
    
    // Check what type of character this is
    //  0: normal character
    //  1: token seperator that is a token
    //  2: token seperator that can be omitted
    if (c == ' ' || c == '\n' || c == '\r' || c == EOF) {
      char_type = 2;
    } else {
      char_type =0;
    }

    if ((char_type == 1 || char_type == 2) && token_value_index > 0) {
      Token *current_token = (Token*) malloc(sizeof(Token));
      // FIXME: Find token type
      current_token->type = 0;
      if (first_token == NULL) {
	first_token = current_token;
      } else {
	prev_token->next_token = (void*) current_token;
      }
      current_token->next_token = NULL;
      int value_length = (int) strlen(token_value);
      current_token->value = (char*) malloc(value_length);
      memcpy(current_token->value, token_value, value_length);
      prev_token = current_token;


      // Reset variables
      int i;
      for (i=0; i<MAX_TOKEN_LENGTH; i++)
	token_value[i] = 0;
      token_value_index = 0;
    } 
    
    if (char_type != 2) {
      if (token_value_index > MAX_TOKEN_LENGTH - 2) {
	printf("Line %d: Max token length exceeded (Token: %s)\n", line_number, token_value);
	return 1;
      }
      token_value[token_value_index++] = c;
    }

  } while (c != EOF);

  fclose(file);
  
  if (first_token != NULL) {
    Token *current_token = first_token;
    do {
      printf("%s\n", (char*) current_token->value);
    } while ((current_token = (Token*)current_token->next_token) != NULL);
  }

  return 0;
}

