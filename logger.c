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

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "logger.h"
#include "avl_c/avl.h"
#include "lists/lists.h"

/* Standard include files */

/* constants */

/* types */

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

/* prototypes */
static int   logg_registry_comparator(const void *_a, const void *_b);
static const void *logg_registry_constructor(const void *_a);
static void  logg_registry_destructor(const void *_a);
static int   logg_registry_print(const void *_a, FILE *o);

/* variables */

static struct logg_global {
    AVL_TREE        gl_chann_ops;
    AVL_TREE        gl_log_registry;
} logg_global_ctx;

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



/* static functions */

static int logg_registry_comparator(const void *_a, const void *_b)
{
    int res;
    const struct logg_registry_key *a = _a, *b = _b;

    res = strcmp(a->lr_file, b->lr_file); if (res) return res;
    res = strcmp(a->lr_func, b->lr_func); if (res) return res;
    return a->lr_line - b->lr_line;
} /* log_registry_comparator */

static const void *logg_registry_constructor(const void *_a)
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

/* $Id: main.c.m4,v 1.7 2005/11/07 19:39:53 luis Exp $ */
