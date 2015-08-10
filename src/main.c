#include <stdio.h>
#include <alpm.h>
#include <unistd.h>
#include <getopt.h>

#include "base.h"
#include "list.h"
#include "gui.h"

struct option args[] = {
    {"listall", no_argument, NULL, 'l'},
    {"gui"    , no_argument, NULL, 'g'}
};



int main(int argc, char **argv)
{
    int c =0;
    while(( c = getopt_long(argc, argv, ":lg", args, NULL)) != -1)
    {
        switch(c)
        {
            case 'l':
                list_all_packages_to_terminal();
            break;
            case 'g':
                init_gui(argc, argv);
            break;
        }
    }
    return 0;
}
