/* $Id: main.c.m4,v 1.7 2005/11/07 19:39:53 luis Exp $
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Thu Nov 17 11:05:04 CET 2016
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

#define IN_SAMPLE_C

/* Standard include files */

/* local include files */

#include "logger.h"
#include "avl_c/avl.h"
#include "lists/lists.h"

/* constants */

/* local types */

/* function prototypes */

/* variables */

static struct logg_chann_ops sample_ops = {0};

/* functions */

LOGG_INIT_FUNC(sample, name)
{
    AVL_ITERATOR it;
    it = logg_register_channops(name, &sample_ops);
    DEBUG("logg_register_channops(\"%s\", &sample_ops) => %#p \"%s\"",
            name, it, sample_ops.co_name);
    return it;
} /* LOGG_INIT_FUNC */

LOGG_FINI_FUNC(sample, name)
{
    int res;
    res = logg_unregister_channops(&sample_ops);
    DEBUG("logg_unregister_channops(&sample_ops) => %d", name, res);
    return res;
} /* LOGG_FINI_FUNC */

void _init(void)
{
    DEBUG("initialization");
}

void _fini(void)
{
    DEBUG("finalization");
}

/* static functions */

/* test code */

/* $Id: main.c.m4,v 1.7 2005/11/07 19:39:53 luis Exp $ */
