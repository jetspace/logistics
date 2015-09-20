#ifndef _LOGISTICS_PACMAN_WRAP
#define _LOGISTICS_PACMAN_WRAP

#include <glib.h>

void install_package(char *pkgname)
{
	char *cmd = g_strdup_printf("pacman -S %s", pkgname);
	system(cmd);
	g_free(cmd);
}

void remove_package(char *pkgname)
{
	char *cmd = g_strdup_printf("pacman -R %s", pkgname);
	system(cmd);
	g_free(cmd);
}

//defines

#define update_package_db() system("pacman -Sy")
#define system_upgrade() system("pacman -Su")


#endif
