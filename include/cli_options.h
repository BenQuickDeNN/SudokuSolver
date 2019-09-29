#include <getopt.h>

static struct option long_options[] =
{
    /* distinguish them by their indices. */
    /*  option_name, argument, flag, short_option_name*/
    {"help",    no_argument,        0,  'h'},
    {"version", no_argument,        0,  'v'},
    {"file",    required_argument,  0,  'f'},
    {0,         0,                  0,   0}
};