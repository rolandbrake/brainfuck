#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define TAPE_SIZE 30000
#define MAX_STACK 1024

typedef struct
{
    const char *code;
    int pos;
} parser;

// loop stack
typedef struct
{
    int start[MAX_STACK]; // start position of loops
    int top;              // loop top pointer
} stack;

/**
 * @brief Runs the given Brainfuck source code.
 *
 * @param source A null-terminated string containing the Brainfuck code to run.
 *
 * @details
 * This function preprocesses the code to expand repeated commands and
 * normalize loop brackets. It then runs the parsed code on a tape of
 * size `TAPE_SIZE` and prints the results to stdout.
 *
 * @note
 * This implementation does not handle EOF correctly. It will loop
 * indefinitely if the program attempts to read from the tape when it is
 * empty.
 */
void run(const char *source)
{
    unsigned char tape[TAPE_SIZE] = {0};
    unsigned char *ptr = tape;

    // Preprocess: Expand code
    char *code = malloc(strlen(source) * 10); // Allocate enough space
    int length = 0;

    for (int i = 0; source[i];)
    {
        char ch = source[i];

        // Skip comments
        if (ch == '#')
        {
            while (source[i] && source[i] != '\n')
                i++;
            continue;
        }


        // multi-line comments
        if (ch == '\'') // If we hit a single quote
        {
            i++; // Move past the opening quote
            // Skip everything until we find the closing quote or end of source
            while (source[i] && source[i] != '\'')
            {
                i++;
            }
            if (source[i] == '\'')
            {
                i++; // Move past the closing quote
            }
            continue; // Skip this comment and continue to next character
        }

        // Repeat syntax
        if (ch == '>' || ch == '<' || ch == '+' || ch == '-')
        {
            code[length++] = ch;
            i++;

            if (isdigit(source[i]))
            {
                int count = 0;
                while (isdigit(source[i]))
                {
                    count = count * 10 + (source[i] - '0');
                    i++;
                }
                for (int j = 1; j < count; j++)
                    code[length++] = ch;
            }
        }
        // Loop brackets normalization
        else if (ch == '(' || ch == '{')
        {
            code[length++] = '[';
            i++;
        }
        else if (ch == ')' || ch == '}')
        {
            code[length++] = ']';
            i++;
        }
        // Valid command or loop
        else if (strchr("?[]*.", ch))
        {
            code[length++] = ch;
            i++;
        }
        // Ignore everything else
        else
            i++;
    }

    code[length] = '\0';

    // Run parsed code
    const char *pc = code;
    stack stack = {.top = 0};

    while (*pc)
    {
        switch (*pc)
        {
        case '>':
            ptr++;
            if (ptr >= tape + TAPE_SIZE)
                ptr = tape;
            break;
        case '<':
            if (ptr <= tape)
                ptr = tape + TAPE_SIZE;
            else
                ptr--;
            break;
        case '+':
            (*ptr)++;
            break;
        case '-':
            (*ptr)--;
            break;
        case '.':
            putchar(*ptr);
            break;
        case '*':
            *ptr = getchar();
            break;

        // generate a random value between 0-255 and put it in the current cell of the tape
        case '?':
            *ptr = rand() % 256;
            break;
        case '[':
            if (*ptr == 0)
            {
                int loop = 1;
                while (loop > 0)
                {
                    pc++;
                    if (*pc == '[')
                        loop++;
                    else if (*pc == ']')
                        loop--;
                    if (*pc == '\0')
                    {
                        fprintf(stderr, "Error: Unmatched [\n");
                        free(code);
                        return;
                    }
                }
            }
            else
            {
                if (stack.top >= MAX_STACK)
                {
                    fprintf(stderr, "Error: Loop stack overflow\n");
                    free(code);
                    return;
                }
                stack.start[stack.top++] = pc - code;
            }
            break;
        case ']':
            if (stack.top <= 0)
            {
                fprintf(stderr, "Error: Unmatched ]\n");
                free(code);
                return;
            }
            if (*ptr != 0)
                pc = code + stack.start[stack.top - 1];
            else
                stack.top--;
            break;
        }
        pc++;
    }

    free(code);
}

/**
 * @brief Main entry point of the program.
 *
 * Reads the contents of the specified file and interprets it as a
 * Brainfuck program.
 *
 * @param[in] argc Number of command line arguments.
 * @param[in] argv Command line arguments. The first argument should be the
 *            name of the file containing the Brainfuck program.
 *
 * @return 0 on success, 1 if no file was specified, 2 if the file could
 *         not be opened.
 */
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <BF_FILE>\n", argv[0]);
        return 1;
    }

    srand((unsigned int)time(NULL)); // Seed the RNG once

    FILE *f = fopen(argv[1], "r");
    if (!f)
    {
        perror("File open");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *src = malloc(size + 1);
    fread(src, 1, size, f);
    src[size] = '\0';
    fclose(f);

    run(src);

    free(src);
    return 0;
}
