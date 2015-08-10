#include <stdio.h>
#include <alpm.h>

static alpm_handle_t *handle;

int main(void)
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
            printf("PKG: %s\n", alpm_pkg_get_name(i->data));
        }

    }

    return 0;
}
