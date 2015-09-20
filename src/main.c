#include <stdio.h>
#include <alpm.h>
#include <unistd.h>
#include <getopt.h>

#include "base.h"
#include "list.h"
#include "gui.h"
#include "wrap.h"

#define VERSION "0.03"

char *AUTHORS[] = {"Marius Messerschmidt <marius.messerschmidt@googlemail.com>"};
char *DESIGNER[] = {"Tom Messerschmidt <tom.messerschmidt@googlemail.com>"};

struct option args[] = {
    {"listall", no_argument, NULL, 'l'},
    {"help"    , no_argument, NULL, 'h'},
    {"version"    , no_argument, NULL, 'v'},
    {"install"    , required_argument, NULL, 'i'},
    {"remove"    , required_argument, NULL, 'r'},
    {"update"    , required_argument, NULL, 'u'},
    {"upgrade"    , no_argument, NULL, 'U'},
    {"gui"    , no_argument, NULL, 'g'}
};


void show_version(void)
{
	puts("- JetSpace Logistics package management frontend -");
	puts("Copyright (C) 2015 Marius Messerschmidt (JetSpace)");
	puts("--------------------------------------------------\n");

	printf(" -	Version %s\n", VERSION);
	puts("\n--------------------------------------------------\n");
	puts("--------------------Written by:-------------------\n");
	for(int x = 0; x < sizeof(AUTHORS) / sizeof(char *); x++)
		puts(AUTHORS[x]);

	puts("\n--------------------Design by:--------------------\n");
	for(int x = 0; x < sizeof(DESIGNER) / sizeof(char *); x++)
		puts(DESIGNER[x]);

}

void show_help(void)
{
	puts("- JetSpace Logistics package management frontend -");
	puts("Copyright (C) 2015 Marius Messerschmidt (JetSpace)");
	puts("--------------------------------------------------\n");

	puts("Available Options:");
	puts(" :: System functions");
	puts("	-l | --listall          : Print all available packages to stdout");
	puts("	-h | --help             : Show this help output");
	puts("	-v | --version          : Print version information");
	puts(" :: Pacman wraper functions");
	puts("	-i | --install [pkg]    : Install [pkg]");
	puts("	-r | --remove  [pkg]    : Remove  [pkg]");
	puts("	-u | --update           : Update package data base");
	puts("	-U | --upgrade          : Update all packages");
	puts(" :: GUI functions");
	puts("	-g | --gui              : Show GUI package manager");
}

int main(int argc, char **argv)
{
    int c =0;
    while(( c = getopt_long(argc, argv, ":lhvi:r:uUg", args, NULL)) != -1)
    {
        switch(c)
        {
	    	case 'h':
				show_help();
				return 0;
	    	break;
	    	case 'v':
				show_version();
				return 0;
	    	break;
            case 'l':
                list_all_packages_to_terminal();
            break;
            case 'i':
               	install_package(optarg);
            break;
            case 'r':
               	remove_package(optarg);
            break;
            case 'u':
               	update_package_db();
            break;
            case 'U':
               	system_upgrade();
            break;
            case 'g':
                init_gui(argc, argv);
            break;
			default:
				show_help();
				return 1;
			break;
        }
    }
    return 0;
}
