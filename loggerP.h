/* $Id: header.h.m4,v 1.7 2005/11/07 19:39:53 luis Exp $
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Tue Oct 25 19:46:01 EEST 2016
 * Project: Services
 *
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

/* Do not include anything BEFORE the line below, as it would not be
 * protected against double inclusion from other files */
#ifndef LOGGERP_H
#define LOGGERP_H

#include "logger.h"

/* constants */

/* types */
struct logger_ctx_s {
    AVL_TREE        lctx_chann_classes;
    AVL_TREE        lctx_channels;
    
};

struct logger_chann_class_s {
    int               (*lc_open)();
    int               (*lc_close)();
    int               (*lc_rotate)();
    ssize_t           (*lc_write)(const char *str);
    ssize_t           (*lc_flush)();
};

typedef struct logg_chan_s {
    LOGG_CHAN_CLASS_P  *lcc_class;
    char               *lcc_name;
    AVL_TREE           *lcc_by_func;
    AVL_TREE           *lcc_by_line;
    LLIST_T             lcc_all;
} LOGG_CHANN_T, *LOGG_CHANN_P;

typedef struct logger_s {
    int                 lg_flags; /* flags for message output */
    int                 lg_crit; /* -1 used as wilcard */
    char               *lg_file; /* NULL used as wilcard */
    int                 lg_line_begin; /* -1 used as wilcard */
    int                 lg_line_end; /* -1 used as wilcard */
    char               *lg_func; /* NULL used as wilcard */
    LOGG_CHANN_P        lg_chann; /* pointer to device entry point */
} LOGGER_T, *LOGGER_P;

/* prototypes */

ssize_t loggv(
        int             crit,
        const char     *file,
        const int       begin,
        const int       end,
        const char     *func,
        const char     *fmt,
        va_list         args);

ssize_t logg(
        int             crit,
        const char     *file,
        const int       line,
        const char     *func,
        const char     *fmt,
        ...);

void logger_init(
        LOGGER_P        logger,
        int             flags,
        int             crit,
        char           *file,
        int             line_start,
        int             line_end,
        char           *func,
        LOGG_CHANN_P    chan);

LOGGER_P logger_new(
        int             flags,
        int             crit,
        char           *file,
        int             line_start,
        int             line_end,
        char           *func,
        LOGG_CHANN_P    chan);

#endif /* LOGGERP_H */
/* Do not include anything AFTER the line above, as it would not be
 * protected against double inclusion from other files.  */
/* $Id: header.h.m4,v 1.7 2005/11/07 19:39:53 luis Exp $ */
