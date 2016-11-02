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

/* prototypes */

/* variables */

static struct log_global {
    AVL_TREE        glb_chann_ops;
} log_global_ctx;

/* functions */

static int chann_ops_comparator(LOGG_CHANN_OPS a, LOGG_CHANN_OPS b)
{
    return strcmp(a->co_name, b->co_name);
} /* chann_ops_comparator */

int logg_init()
{
    log_global_ctx.glb_chann_ops = new_avl_tree(
            (AVL_FCOMP)chann_ops_comparator, NULL, NULL, NULL);
    assert(log_global_ctx.glb_chann_ops != NULL);
    return 0;
} /* logg_init */

/* $Id: main.c.m4,v 1.7 2005/11/07 19:39:53 luis Exp $ */
