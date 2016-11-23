/* $Id: main.c.m4,v 1.7 2005/11/07 19:39:53 luis Exp $
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Tue Oct 25 20:14:24 EEST 2016
 * Project: Services
 *
 * Disclaimer:
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#define IN_LOGGER_C

/* Standard include files */

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <dlfcn.h>

/* local include files */

#include "logger.h"
#include "avl_c/avl.h"
#include "lists/lists.h"

/* constants */

#ifndef MAX_PATH_ELEMENTS
#define MAX_PATH_ELEMENTS   32
#endif
#ifndef MAX_PATH_LENGTH
#define MAX_PATH_LENGTH     1024
#endif
#ifndef MAX_DLNAME_LENGTH
#define MAX_DLNAME_LENGTH   1024
#endif
#ifndef DEFAULT_LOGGER_PATH
#define DEFAULT_LOGGER_PATH "/lib/logger:/usr/lib/logger:/usr/local/lib/logger"
#endif
#ifndef MAX_LOGGV_BUFFER_SIZE
#define MAX_LOGGV_BUFFER_SIZE       65536
#endif
#ifndef MAX_LOGGV_LINES
#define MAX_LOGGV_LINES             1024
#endif
#ifndef LOGG_PATH_VARIABLE
#define LOGG_PATH_VARIABLE          "LOGG_PATH"
#endif

/* local types */

typedef struct logg_registry_key {
    char           *lr_file;
    char           *lr_func;
    int             lr_line;
} *LOGG_REGISTRY_KEY;

typedef struct logg_registry {
    LOGG_REGISTRY_KEY
                    lr_key;

    LLIST_T         lr_chanlist;

    u_int64_t       lr_count_selected;
    u_int64_t       lr_count_total;
    struct timespec lr_last_time;
} *LOGG_REGISTRY;

typedef struct logg_message {
    LOGG_REGISTRY   lm_registry;
    const char    **lm_message;
} *LOGG_MESSAGE;


/* function prototypes */

static int   logg_registry_comparator(const void *_a, const void *_b);
static void *logg_registry_constructor(const void *_a);
static void  logg_registry_destructor(const void *_a);
static int   logg_registry_print(const void *_a, FILE *o);
static void *logg_thread_monitor(void *_p);

/* variables */

static struct logg_global {
    AVL_TREE        gl_chann_ops;
    AVL_TREE        gl_log_registry;
    pthread_mutex_t gl_mutex;
} logg_global_ctx;

static char *logg_crits[] = {
    "FATAL",
    "CRITI",
    "EMERG",
    "ERROR",
    "WARNI",
    "INFOR",
    "DEBUG"
};
static int logg_crits_n = sizeof logg_crits / sizeof logg_crits[0];

/* functions */

int logg_init()
{
    logg_global_ctx.gl_chann_ops = new_avl_tree(
            (AVL_FCOMP) strcmp,
            (AVL_FCONS) strdup,
            (AVL_FDEST) free,
            (AVL_FPRNT) fputs);
    assert(logg_global_ctx.gl_chann_ops != NULL);

    logg_global_ctx.gl_log_registry = new_avl_tree(
            logg_registry_comparator,
            logg_registry_constructor,
            logg_registry_destructor,
            logg_registry_print);
    assert(logg_global_ctx.gl_log_registry != NULL);
            
    return 0;
} /* logg_init */

LOGG_CHANN_OPS logg_channop_lookup(char *name)
{
    LOGG_CHANN_OPS res;
    DEBUG("looking for channops \"%s\"\n", name);
    AVL_ITERATOR it = avl_tree_atkey(logg_global_ctx.gl_chann_ops,
            name,
            MT_EQ);
    if (!it) { /* doesn't exist */
        static char *path[MAX_PATH_ELEMENTS] = {0};
        int i;
        DEBUG("not registered, looking for \"%s\" module", name);
        if (!path[0]) {
            char *path_env = getenv(LOGG_PATH_VARIABLE);
            int i;
            if (!path_env || !*path_env) path_env = DEFAULT_LOGGER_PATH;
            static char path_buffer[MAX_PATH_LENGTH];
            char *strtok_ctx;

            DEBUG("initializing search path with \"%s\"\n", path_env);
            strncpy(path_buffer, path_env, sizeof path_buffer);
            for (   i = 0, path[i] = strtok_r(path_buffer, ":", &strtok_ctx);
                    path[i] && (i < MAX_PATH_ELEMENTS);
                    path[++i] = strtok_r(NULL, ":", &strtok_ctx))
                DEBUG("adding directory \"%s\" to path list", path[i]);
        } /* if */
        for (i = 0; path[i]; i++) {
            char dlname[MAX_DLNAME_LENGTH];
            void *dldesc;
            AVL_ITERATOR (*mod_init)(const char *);

            snprintf(dlname, sizeof dlname, "%s/logg_%s.so", path[i], name);
            DEBUG("trying [%s]\n", dlname);
            if (!(dldesc = dlopen(dlname, RTLD_LAZY | RTLD_LOCAL))) {
                INFO("dlopen: %s: %s\n", dlname, dlerror());
                continue;
            }
            DEBUG("found [%s], trying init function", dlname);
            snprintf(dlname, sizeof dlname, "%s_init", name);
            mod_init = dlsym(dldesc, dlname);
            if (!mod_init) {
                DEBUG("no %s function found, continue", dlname);
                continue;
            }
            it = mod_init(name);
            if (!it) {
                INFO("%s(\"%s\") => module not registered, continue", dlname, name);
                continue;
            } /* if */

            /* if we reach here, exit the loop, we registered a valid
             * module */
            break;
        } /* for */
        if (!path[i]) { /* if list exausted, signal an error. */
            ERROR("%s not found, give up\n", name);
            return NULL;
        } /* if */
    } /* if */

    /* now, we have a registered module */
    res = avl_iterator_data(it);

    DEBUG("registration of module \"%s\" ==> %#p\n", name, res);
    return res;
} /* logg_channop_lookup */

LOGG_CHANN logg_chann_vopen(LOGG_CHANN_OPS channops, char *name, va_list p)
{
    pthread_mutex_lock(&channops->co_lock);
    AVL_ITERATOR it = avl_tree_atkey(
            channops->co_channs,
            name,
            MT_EQ);
    if (it) {
        LOGG_CHANN res = avl_iterator_data(it);
        pthread_mutex_unlock(&channops->co_lock);
        pthread_mutex_lock(&res->ch_lock);
        res->ch_refcnt++;
        pthread_mutex_unlock(&res->ch_lock);
        return res;
    } else {
        LOGG_CHANN res = malloc(channops->co_inst_size);
    } /* if */


    return NULL;
} /* logg_chann_vlookup */

LOGG_CHANN logg_chann_open(LOGG_CHANN_OPS channops, char *name, ...)
{
    va_list p;
    LOGG_CHANN res;

    va_start(p, name);
    res = logg_chann_vopen(channops, name, p);
    va_end(p);
    return res;
} /* logg_chann_lookup */

/* channops supporting functions */

AVL_ITERATOR logg_register_channops(
        const char     *name,
        LOGG_CHANN_OPS  channops)
{
    AVL_ITERATOR it = avl_tree_atkey(
            logg_global_ctx.gl_chann_ops,
            name,
            MT_EQ);
    if (it) {
        DEBUG("key \"%s\" already registered", name);
        return NULL;
    }
    it = avl_tree_put(
            logg_global_ctx.gl_chann_ops,
            name,
            channops);
    channops->co_name = avl_iterator_key(it);
    return it;
} /* logg_register_channops */

int logg_unregister_channops(
        LOGG_CHANN_OPS  channops)
{
    DEBUG("searching for channops \"%s\"", channops->co_name);
    AVL_ITERATOR it = avl_tree_atkey(
            logg_global_ctx.gl_chann_ops,
            channops->co_name,
            MT_EQ);
    if (!it) {
        INFO("channops \"%s\" not registered", channops->co_name);
        return -1;
    }
    avl_iterator_del(it, logg_global_ctx.gl_chann_ops);
    DEBUG("deleting channops \"%s\"", channops->co_name);
    return 0;
} /* logg_register_channops */

/* static functions */

static int logg_registry_comparator(const void *_a, const void *_b)
{
    int res;
    const struct logg_registry_key *a = _a, *b = _b;

    res = strcmp(a->lr_file, b->lr_file); if (res) return res;
    res = strcmp(a->lr_func, b->lr_func); if (res) return res;
    return a->lr_line - b->lr_line;
} /* log_registry_comparator */

static void *logg_registry_constructor(const void *_a)
{
    LOGG_REGISTRY_KEY res = malloc(sizeof *res);
    const struct logg_registry_key *a = _a;
    assert(res != NULL);
    *res = *a;
    return res;
} /* logg_registry_constructor */

static void logg_registry_destructor(const void *_a)
{
    free((void *)_a);
} /* logg_registry_destructor */

static int logg_registry_print(const void *_k, FILE *o)
{
    const struct logg_registry_key *k = _k;
    return fprintf(o,
            "%s:%s:%d",
            k->lr_file,
            k->lr_func,
            k->lr_line);
} /* logg_registry_print */

/* test code */

ssize_t loggv(
        int             crit,
        const char     *file,
        const int       line,
        const char     *func,
        const char     *fmt,
        va_list         args)
{
    char buffer[MAX_LOGGV_BUFFER_SIZE];
    char *lines[MAX_LOGGV_LINES];
    char *strtok_ctx;
    int num_lines = 0, i;
    size_t res = 0;
    int crit_ix = crit >> 5;

    vsnprintf(buffer, sizeof buffer, fmt, args);
    for (   num_lines = 0, lines[num_lines] = strtok_r(buffer, "\n", &strtok_ctx);
            lines[num_lines] && (num_lines < MAX_LOGGV_LINES);
            lines[++num_lines] = strtok_r(NULL, "\n", &strtok_ctx))
        continue;
    for ( i = 0; lines[i]; i++) {
        res += printf("<%d-%s>:%s:%d:%s: %s\n",
                crit,
                crit_ix < logg_crits_n
                    ? logg_crits[crit_ix]
                    : "VERBO",
                file, line, func,
                lines[i]);
    } /* for */
    return res;
} /* loggv */

ssize_t logg(
        int             crit,
        const char     *file,
        const int       line,
        const char     *func,
        const char     *fmt,
        ...)
{
    int res;
    va_list args;
    va_start(args, fmt);
    res = loggv(crit, file, line, func, fmt, args);
    va_end(args);
    return res;
}

/* $Id: main.c.m4,v 1.7 2005/11/07 19:39:53 luis Exp $ */
