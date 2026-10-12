#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define EXTRA_SIZE 256
#define BLOCK_SIZE 128
#define BUF_SIZE 256

struct header {
  uint64_t size;
  struct header *next;
};

void handle_error(char *msg) {
  write(STDERR_FILENO, msg, strlen(msg));
  write(STDERR_FILENO, "/n", 1);
  _exit(1);
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data : *(void **)data);
  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

void *increase_heap_size(int size) {
  void *start = sbrk(size);
  if (start == (void *)-1) {
    handle_error("sbrk");
  }
  return start;
}

void initialize_block(struct header *block, struct header *next, int value) {
  block->size = BLOCK_SIZE;
  block->next = next;
  memset((char *)block + sizeof(struct header), value, BLOCK_SIZE - sizeof(struct header));
}

void print_data(struct header *block) {
  char *data = (char *)block + sizeof(struct header);
  for (size_t i = 0; i < BLOCK_SIZE - sizeof(struct header); i++) {
    char out[2] = {'0' + data[i], '\n'};
    write(STDOUT_FILENO, out, 2);
  }
}

int main() {
  struct header *first_block_pointer = increase_heap_size(EXTRA_SIZE);
  struct header *second_block_pointer = (struct header *)((char *)first_block_pointer + BLOCK_SIZE);

  initialize_block(first_block_pointer, NULL, 0);
  initialize_block(second_block_pointer, first_block_pointer, 1);

  print_out("first block: %p\n", &first_block_pointer, sizeof(first_block_pointer));

  print_out("second block: %p\n", &second_block_pointer, sizeof(second_block_pointer));

  print_out("first block size: %lu\n", &first_block_pointer->size,
            sizeof(first_block_pointer->size));

  print_out("first block next: %p\n", &first_block_pointer->next,
            sizeof(first_block_pointer->next));

  print_out("second block size: %lu\n", &second_block_pointer->size,
            sizeof(second_block_pointer->size));

  print_out("second block next: %p\n", &second_block_pointer->next,
            sizeof(second_block_pointer->next));

  print_data(first_block_pointer);
  print_data(second_block_pointer);

  return 0;
}
