#include <stdio.h>
#include <sodium.h>
#include <stdlib.h>
#include <unistd.h> // getopt
#include <getopt.h> // getopt -- on some systems

const char* lower_case = "abcdefghijklmnopqrstuvwxyz";
const char* upper_case = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* special_chars = "!@#$%^&*()_+";
const char* numbers = "0123456789";
const char* wordlist_filename = "wordlist.txt";

const size_t WORDLIST_LENGTH = 2048;
const size_t LETTERS_LENGTH = 26;
const size_t SPECIALS_LENGTH = 12;
const size_t NUMS_LENGTH = 10;

int generate_mnemonic_password(int len) {
  FILE* file = fopen(wordlist_filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Error opening wordlist.txt, is it perhaps missing?\n");
    return 1;
  }

  // Print the password word by word to avoid allocating a large unwieldy array
  printf("Generated Password: \n");
  // Generate next word
  for (int i = 0; i < len; i++) {
    int what_to_generate = randombytes_uniform(WORDLIST_LENGTH);
    // Skip lines of file until we get to the randomly generated line number
    for (int j = 0; j < what_to_generate; j ++) {
        fscanf(file, "%*[^\n]\n");
    }
    char word[20];
    fscanf(file, "%19s", word);
    printf("%s ", word);
    rewind(file);
  }
  // Newline for the terminal
  printf("\n");
  return 0;
}

int generate_password(int len, int caps, int nums, int special, int mnemonic) {
  if (mnemonic == 1) {
    return generate_mnemonic_password(len);
  }

  char* password = (char*)malloc((len + 1) * sizeof(char));

  // null terminate the string
  password[len] = '\0';

  for (int i = 0; i < len; i++) {
    // default case if no extra flags are enabled 
    if (!caps && !nums && !special) {
      password[i] = lower_case[randombytes_uniform(LETTERS_LENGTH)];
      continue;
    }

    int possible_rolls = caps + nums + special + 1;
    char* possible_characters = malloc(sizeof(char) * possible_rolls);

    int caps_rolled = 0, nums_rolled = 0, special_rolled = 0;
    for (int j = 0; j < possible_rolls; j++) {
      if (!caps_rolled && caps) {
        possible_characters[j] = upper_case[randombytes_uniform(LETTERS_LENGTH)];
        caps_rolled++;
      } else if (!special_rolled && special) {
        possible_characters[j] = special_chars[randombytes_uniform(SPECIALS_LENGTH)];
        special_rolled++;
      } else if (!nums_rolled && nums) {
        possible_characters[j] = numbers[randombytes_uniform(NUMS_LENGTH)];
        nums_rolled++;
      } else {
        possible_characters[j] = lower_case[randombytes_uniform(LETTERS_LENGTH)];
      }
    }
    password[i] = possible_characters[randombytes_uniform(possible_rolls)];
    free(possible_characters);
  }

  printf("Generated Password: \n");
  printf("%s\n", password);
  free(password);
  return 0;
}

int main(int argc, char *argv[]) {
  if (sodium_init() < 0) {
    fprintf(stderr, "sodium_init() failed\n");
    return 1;
  }

  int len = 0, caps = 0, nums = 0, special = 0, mnemonic = 0;

  extern char *optarg;
  int err_flag = 0;
  char c;

  while ((c = getopt(argc, argv, "cnsml:")) != -1) {
    switch (c) {
      // Length
      // either characters or words for mnemonic based passwords
      case 'l':
        len = atoi(optarg);
        break;
      // caps
      case 'c':
        caps = 1;
        break;
      // numbers
      case 'n':
        nums = 1;
        break;
      // special characters
      case 's':
        special = 1;
        break;
      // mnemonic based
      case 'm':
        mnemonic = 1;
        break;
      case '?':
        err_flag++;
        break;
    }
  }
  if (err_flag == 0 && len <= 0) {
    printf("Usage: %s [-l nlength] [-cmns]\n", argv[0]); 
    printf("-l nlength: the length of the password to generate\n");
    printf("-c: generate capital letters in password\n");
    printf("-n: generate numbers in password\n");
    printf("-s: generate special characters in password\n");
    printf("-m: generate a mnemonic based password (Using a BIP-39 wordlist)\n");
    return 0;
  }
  if (err_flag > 0 || len <= 0) {
    fprintf(stderr, "Invalid arguments. Usage: %s [-l nlength] [-cnsm]\n", argv[0]); 
    exit(2);
  }

  generate_password(len, caps, nums, special, mnemonic);

  return 0;
}
