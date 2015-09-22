#include <stdio.h>
#include <alpm.h>
#include <unistd.h>
#include <getopt.h>

#include <glib/gi18n.h>
#include <locale.h>

#include "base.h"
#include "list.h"
#include "gui.h"
#include "wrap.h"

#define VERSION "0.07"

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
    {"gui"    , no_argument, NULL, 'g'},
    {"gui-updates"    , no_argument, NULL, 'x'}
};


void show_version(void)
{
	puts(_("- JetSpace Logistics package management frontend -"));
	puts(_("Copyright (C) 2015 Marius Messerschmidt (JetSpace)"));
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
	puts(_("- JetSpace Logistics package management frontend -"));
	puts("Copyright (C) 2015 Marius Messerschmidt (JetSpace)");
	puts("--------------------------------------------------\n");

	puts(_("Available Options:"));
	puts(_(" :: System functions"));
	puts(_("	-l | --listall          : Print all available packages to stdout"));
	puts(_("	-h | --help             : Show this help output"));
	puts(_("	-v | --version          : Print version information"));
	puts(_(" :: Pacman wraper functions"));
	puts(_("	-i | --install [pkg]    : Install [pkg]"));
	puts(_("	-r | --remove  [pkg]    : Remove  [pkg]"));
	puts(_("	-u | --update           : Update package data base"));
	puts(_("	-U | --upgrade          : Update all packages"));
	puts(_(" :: GUI functions"));
	puts(_("	-g | --gui              : Show GUI package manager"));
  puts(_("	-x | --gui-updates      : Show GUI package manager (show updates)"));
}

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "" );
    textdomain("logistics");
    int c =0;
    while(( c = getopt_long(argc, argv, ":lhvi:r:uUgx", args, NULL)) != -1)
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
                gtk_init(&argc, &argv);
                init_gui(argc, argv, 0);
            break;
            case 'x':
                gtk_init(&argc, &argv);
                init_gui(argc, argv, 1);
            break;
			default:
				show_help();
				return 1;
			break;
        }
    }
    return 0;
}
