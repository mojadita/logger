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

/* Standard include files */

/* constants */

/* types */

typedef struct logg_registry {
    u_int64_t       lr_count_selected;
    u_int64_t       lr_count_unselected;
    struct timespec lr_last_time;

    int             lr_crit;
    char           *lr_file;
    char           *lr_func;
    int             lr_line;
} *LOGG_REGISTRY;

/* prototypes */
static int chann_ops_comparator(const void *_a, const void *_b);
static int logg_registry_comparator(const void *_a, const void *_b);

/* variables */

static struct logg_global {
    AVL_TREE        gl_chann_ops;
    AVL_TREE        gl_log_registry;
} logg_global_ctx;

/* functions */

int logg_init()
{
    log_global_ctx.gl_chann_ops = new_avl_tree(
            chann_ops_comparator, NULL, NULL, NULL);
    assert(log_global_ctx.gl_chann_ops != NULL);
    return 0;
} /* logg_init */

static int chann_ops_comparator(const void *_a, const void *_b)
{
    const struct logg_chann_ops *a = _a, *b = _b;
    return strcmp(a->co_name, b->co_name);
} /* chann_ops_comparator */

static int logg_registry_comparator(const void *_a, const void *_b)
{
    const struct logg_registry *a = _a, *b = _b;
    int res;
    res = strcmp(a->lr_file, b->lr_file); if (res) return res;
    res = strcmp(a->lr_func, b->lr_func); if (res) return res;
    return a->lr_line - b->lr_line;
} /* log_registry_comparator */
/* $Id: main.c.m4,v 1.7 2005/11/07 19:39:53 luis Exp $ */
