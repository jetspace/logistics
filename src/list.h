#ifndef LOGISTICS_LIST_H
#define LOGISTICS_LIST_H
#include "base.h"

void list_all_packages_to_terminal(void)
{
    init_alpm_sync();

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
#endif
