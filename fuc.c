#include "lexan.h"


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

  Token *first_token = analyze(file);
  
  if (first_token == NULL) {
    printf("Error\n");
    return 1;
  }

  fclose(file);

  if (first_token != NULL) {
    Token *current_token = first_token;
    do {
      printf("%s\n", (char*) current_token->value);
    } while ((current_token = (Token*)current_token->next_token) != NULL);
  }

  return 0;
}

