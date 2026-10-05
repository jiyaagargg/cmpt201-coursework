
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
$include<string.h>

#define MAX_LEN 5

    void add_to_history(char *input_history[], int *history_count, char *input);
void remove_oldest_record(char *input_history[], int *history_count);
void print_history(char *input_history[], int history_count);
char *get_input();

int main(void) {
  char *input_history[MAX_LEN];
  int history_count = 0;

  while (1) {
    char *input = get_input();

    if (input == NULL) {
      printf("Ended\n");
      break;
    }
    add_to_history(input_history, &history_count, input);

    if (strcmp(input, "print") == 0) {
      print_history(input_history, history_count);
    }
  }
  for (int i = 0; i < history_count; i++) {
    free(input_history[i]);
  }
  return 0;
}

char *get_input() {
  char *buffer = NULL;
  size_t bufsize = 0;

  printf("Enter input: ");
  ssize_t len = getline(&buffer, &bufsize, stdin);

  if (len == -1) {
    free(buffer);
    return NULL;
  }
  if (len > 0 && buffer[len - 1] == '\n') {
    buffer[len - 1] = '\0';
  }

  return buffer;
}

void add_to_history(char *input_history[], int *history_count, char *input) {
  if (*history_count >= MAX_LEN) {
    remove_oldest_record(input_history, history_count);
  }
  input_history[*history_count] = input;
  (*history_count)++;
}

void remove_oldest_record(char *input_history[], int *history_count) {
  if (*history_count > 0) {
    free(input_history[0]);

    for (int i = 1; i < *history_count; i++) {
      input_history[i - 1] = input_history[i];
    }
    (*history_count)--;
  }
}

void print_history(char *input_history[], int history_count) {
  for (int i = 0; i < history_count; i++) {
    printf("%s\n", input_history[i]);
  }
}
