/*
 * ============================================
 *       Filename:  AmirMohammad_argp-sample.c
 *       Description: Simple calculator
 *                    batch adder/multiplier command-line calculator
 *                    using GNU argp library
 *       Github: https://github.com/DarkC343
 *       Created:  12/21/2019 07:08:56 PM
 *
 *       Examples:  * add 3 numbers in verbose mode
 *                  => ./out 10 20 30 --add -v
 *                  * add and multiply 3 numbers in verose mode
 *                    and print result to the file called "result.txt"
 *                  => ./out 10 20 30 -amv --output=results.txt
 * ============================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <argp.h>
#include <string.h>

const char *argp_program_varsion = "simple-calc 1.0";
const char *argp_program_bug_address = "bugs@nowhere.com";
static char doc[] = "Simple calculator -- batch adder/multiplier command-line calculator\vNote: Be aware of long long integral overflow! (from -(2^63) to (2^63)-1)\nMore info at nowhere.com";

static char args_doc[] = "[NUMBERS...]";

static struct argp_option options[] = {
    { 0, 0, 0, 0, "Main options" },
    { "add", 'a', 0, 0, "Add numbers" },
    { "multiply", 'm', 0, 0, "Multiply numbers" },
    { 0, 0, 0, 0, "Other options" },
    { "verbose", 'v', 0, 0, "Verbose output" },
    { "quiet", 'q', 0, 0, "Non-verbose output" },
    { "silent", 's', 0, OPTION_ALIAS },
    { "output", 'o', "FILENAME", 0, "Print calculation(s) and result(s) to a file" },
    { 0 }
};

struct arguments
{
    char **numbers;
    long long add, multiply;
    int silent, verbose;
    char *filename;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;
    switch(key)
    {
        case 'a':
            arguments->add = 1;
            break;
        case 'm':
            arguments->multiply = 1;
            break;
        case 'q': case 's':
            arguments->silent = 1;
            break;
        case 'v':
            arguments->verbose = 1;
        case 'o':
            arguments->filename = arg;
            break;
        case ARGP_KEY_ARG:
            arguments->numbers = &state->argv[state->next];
            state->next = state->argc;
            break;
        case ARGP_KEY_NO_ARGS:
            argp_usage(state);
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char **argv)
{
    struct arguments arguments;
    arguments.add = 0;
    arguments.multiply = 0;
    arguments.filename = "";
    arguments.silent = 0;
    arguments.verbose = 0;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    long long sum = 0;
    long long multiplication = 1;
    char verbose_sum[120] = "", verbose_multipcation[120] = "";
    for ( int i = -1; arguments.numbers[i]; i += 1 ) {
        if(arguments.verbose)
        {
            if(i == -1)
            {
                strcat(verbose_sum, arguments.numbers[i]);
                strcat(verbose_multipcation, arguments.numbers[i]);
            }
            else
            {
                strcat(verbose_sum, " + ");
                strcat(verbose_multipcation, " * ");
                strcat(verbose_sum, arguments.numbers[i]);
                strcat(verbose_multipcation, arguments.numbers[i]);
            }
        }
        if(arguments.add) sum += strtoll(arguments.numbers[i], NULL, 10);
        if(arguments.multiply) multiplication *= strtoll(arguments.numbers[i], NULL, 10);
    }

    if(arguments.verbose)
    {
        if(arguments.add) printf("%s = %lld\n", verbose_sum, sum);
        if(arguments.multiply) printf("%s = %lld\n", verbose_multipcation, multiplication);
    } 
    else
    { 
        if(arguments.add) printf("Sum is %lld\n", sum);
        if(arguments.multiply) printf("Multiplication is %lld\n", multiplication);
    }

    if(arguments.filename)
    {
        FILE *f = fopen(arguments.filename, "w");
        if(!f)
        {
            perror(NULL);
            exit(EXIT_FAILURE);
        }
        if(arguments.add)
            if(arguments.verbose) fprintf(f, "%s = %lld\n", verbose_sum, sum);
            else fprintf(f, "Sum is %lld\n", sum); 
        if(arguments.multiply)
            if(arguments.verbose) fprintf(f, "%s = %lld\n", verbose_multipcation, multiplication);
            else fprintf(f, "Multiplication is %lld\n", multiplication);
        fclose(f);
    }


    return 0;
}

