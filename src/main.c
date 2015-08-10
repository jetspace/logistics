#include <stdio.h>
#include <alpm.h>
#include <unistd.h>
#include <getopt.h>

void list_all(void);

struct option args[] = {
    {"listall", no_argument, NULL, 'l'},
};

static alpm_handle_t *handle;

int main(int argc, char **argv)
{
    int c =0;
    while(( c = getopt_long(argc, argv, ":l", args, NULL)) != -1)
    {
        switch(c)
        {
            case 'l':
                list_all();
            break;
        }
    }
    return 0;
}

void list_all(void)
{
    handle = alpm_initialize("/","/var/lib/pacman",NULL);
    alpm_list_t *sync_dbs = alpm_get_syncdbs(handle);
    const char *dbs[] = {
        "testing",
        "core",
        "extra",
        "community-testing",
        "community",
        "multilib-testing",
        "multilib",
    };
    size_t i;
    for(i = 0; i < sizeof(dbs) / sizeof(dbs[0]); i++)
    {
        alpm_db_t *db = alpm_register_syncdb(handle, dbs[i], ALPM_SIG_USE_DEFAULT);

        alpm_list_t *i, *cache = alpm_db_get_pkgcache(db);
        for(i = cache; i; i = alpm_list_next(i))
        {
            printf("PKG: %s (%s)\n", alpm_pkg_get_name(i->data), alpm_pkg_get_version(i->data));
        }

    }
}
