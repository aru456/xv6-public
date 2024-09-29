#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define MAX_LINE 1024

// Custom implementation of strcat in xv6
char*
strcat(char *dest, const char *src)
{
  char *d = dest;
  while(*d) d++;
  while((*d++ = *src++) != 0);
  return dest;
}

// Custom implementation of strncat in xv6
char*
strncat(char *dest, const char *src, int n)
{
  char *d = dest;
  while(*d) d++;
  while(n-- && (*d++ = *src++) != 0);
  if(n == 0) *d = '\0';
  return dest;
}

// Custom implementation of strncmp in xv6
int
strncmp(const char *p, const char *q, int n)
{
  while(n > 0 && *p && *p == *q)
    n--, p++, q++;
  if(n == 0)
    return 0;
  return (uchar)*p - (uchar)*q;
}

// Check if a character is a valid word boundary
int
is_word_boundary(char c)
{
  return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\0' ||
          c == '.' || c == ',' || c == '!' || c == '?' || c == ';' || c == ':' ||
          c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' ||
          c == '-' || c == '"' || c == '\'');
}

// Function to replace a variable with its value in a line of text
void
replace_var(char *line, const char *var, const char *val)
{
  char buffer[MAX_LINE] = {0};  // Initialize buffer with zero
  char *pos = line;
  char *var_pos;
  int var_len = strlen(var);

  // Search for occurrences of the variable in the line
  while((var_pos = strchr(pos, *var)) != 0){
    // Check if the rest of the variable matches
    if(strncmp(var_pos, var, var_len) == 0 &&
       is_word_boundary(var_pos[var_len]) &&
       (var_pos == line || is_word_boundary(var_pos[-1]))){
      // Copy the part of the line before the variable to the buffer
      strncat(buffer, pos, var_pos - pos);
      // Add the replacement value
      strcat(buffer, val);
      // Move the position to after the found variable
      pos = var_pos + var_len;
    } else {
      // Continue searching
      strncat(buffer, pos, 1);
      pos++;
    }
  }

  // Add the remaining part of the line after the last replacement
  strcat(buffer, pos);
  // Copy the modified content back into the original line
  strcpy(line, buffer);
}

int
main(int argc, char *argv[])
{
  int i;
  if(argc < 3){
    printf(2, "Usage: %s <input_file> -D<var1>=<val1> ...\n", argv[0]);
    exit();
  }

  // Open the input file for reading
  int fd = open(argv[1], O_RDONLY);
  if(fd < 0){
    printf(2, "Error: Could not open file %s\n", argv[1]);
    exit();
  }

  // Open the output file for writing
  int output_fd = open("output.txt", O_CREATE | O_WRONLY);
  if(output_fd < 0){
    printf(2, "Error: Could not create output file\n");
    close(fd);
    exit();
  }

  // Read the file line by line
  char line[MAX_LINE];
  int n;
  while((n = read(fd, line, sizeof(line) - 1)) > 0){
    line[n] = '\0';
    // For each line, check each -D argument
    for(i = 2; i < argc; i++){
      if(strncmp(argv[i], "-D", 2) == 0){
        // Split the variable and value
        char *equal_sign = strchr(argv[i], '=');
        if(equal_sign){
          *equal_sign = '\0';  // Split the string
          char *var = argv[i] + 2;  // Skip "-D"
          char *val = equal_sign + 1;

          // Replace occurrences of the variable in the line
          replace_var(line, var, val);

          // Restore the '=' in the original string
          *equal_sign = '=';
        }
      }
    }

    // Output the modified line to the console
    printf(1, "%s", line);
    // Write the modified line to the output file
    write(output_fd, line, strlen(line));
  }

  close(fd);
  close(output_fd);
  exit();
}
