#ifndef LOGISTICS_BASE_H
#define LOGISTICS_BASE_H

//HANDLE FOR ALPM
static alpm_handle_t *handle;

/*
  All DB's
*/
const char *dbs[] = {
    "testing",
    "core",
    "extra",
    "community-testing",
    "community",
    "multilib-testing",
    "multilib",
};

/*
  Init ALPM
*/
void init_alpm_sync(void)
{
  handle = alpm_initialize("/","/var/lib/pacman",NULL);
  alpm_list_t *sync_dbs = alpm_get_syncdbs(handle);
}

#endif
