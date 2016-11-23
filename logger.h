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
#ifndef LOGGER_H
#define LOGGER_H

#include <sys/types.h>
#include <stdarg.h>

#include "avl_c/avl.h"

/* constants */

/* the five less signifiant bits are there to allow for extension of
 * logger levels, so more granularity is allowed than the proposed
 * standard. */
#define __LOGG_FREE_BITS         5
#define LOGG_VER               ((7 << __LOGG_FREE_BITS) | (1 << (__LOGG_FREE_BITS-1)))
#define LOGG_DEB               ((6 << __LOGG_FREE_BITS) | (1 << (__LOGG_FREE_BITS-1)))
#define LOGG_INF               ((5 << __LOGG_FREE_BITS) | (1 << (__LOGG_FREE_BITS-1)))
#define LOGG_WAR               ((4 << __LOGG_FREE_BITS) | (1 << (__LOGG_FREE_BITS-1)))
#define LOGG_ERR               ((3 << __LOGG_FREE_BITS) | (1 << (__LOGG_FREE_BITS-1)))
#define LOGG_CRT               ((2 << __LOGG_FREE_BITS) | (1 << (__LOGG_FREE_BITS-1)))
#define LOGG_EMG               ((1 << __LOGG_FREE_BITS) | (1 << (__LOGG_FREE_BITS-1)))
#define LOGG_FAT               ((0 << __LOGG_FREE_BITS) | (1 << (__LOGG_FREE_BITS-1)))

#define LOGG(lvl, fmt, args...) logg(lvl, __FILE__, __LINE__, __func__, (fmt), ##args)
#define DEBUG(fmt, args...)     logg(LOGG_DEB, __FILE__, __LINE__, __func__, (fmt), ##args)
#define INFO(fmt, args...)      logg(LOGG_INF, __FILE__, __LINE__, __func__, (fmt), ##args)
#define WARNING(fmt, args...)   logg(LOGG_WAR, __FILE__, __LINE__, __func__, (fmt), ##args)
#define ERROR(fmt, args...)     logg(LOGG_ERR, __FILE__, __LINE__, __func__, (fmt), ##args)
#define EMERG(fmt, args...)     logg(LOGG_EMG, __FILE__, __LINE__, __func__, (fmt), ##args)
#define CRITICAL(fmt, args...)  logg(LOGG_CRT, __FILE__, __LINE__, __func__, (fmt), ##args)
#define FATAL(fmt, args...)     logg(LOGG_FAT, __FILE__, __LINE__, __func__, (fmt), ##args)

#define LOGG_TIMESTAMP         (1 << 0) /* do we output timestamp */
#define LOGG_CRIT              (1 << 1)
#define LOGG_FILENAME          (1 << 2) /* do we output filename */
#define LOGG_LINENUM           (1 << 3) /* '' line number */
#define LOGG_FUNCNAME          (1 << 4) /* '' function name */
#define LOGG_COLOR             (1 << 5) /* '' ansi color sequences(if channel admits it) */

#define LOGG_ALL               (LOGG_TIMESTAMP | LOGG_CRIT |\
                                LOGG_FILENAME | LOGG_LINENUM |\
                                LOGG_FUNCNAME)

/* error codes */
#define LOGG_ERR_SUCCESS                             0
#define LOGG_ERR_CHANNOP_ALREADY_REGISTERED         -1

/* types */

typedef struct logg_chann_ops      *LOGG_CHANN_OPS;
typedef struct logg_chann          *LOGG_CHANN;
typedef struct logg_log            *LOGG_LOG;

struct logg_chann_ops {
    const char         *co_name;
    int               (*co_init)(LOGG_CHANN_OPS *);
    LOGG_CHANN        (*co_open)(const char *name, va_list p);
    int               (*co_close)(LOGG_CHANN chann);
    int               (*co_rotate)(LOGG_CHANN chann);
    ssize_t           (*co_write)(LOGG_CHANN chann, const char *str);
    ssize_t           (*co_flush)(LOGG_CHANN chann);
    size_t              co_inst_size;
    AVL_TREE            co_channs;
    int                 co_refcnt;
    pthread_mutex_t     co_lock;
};

struct logg_chann {
    ssize_t             ch_refcnt;
    char               *ch_name;
    LOGG_CHANN_OPS      ch_channops;
    pthread_mutex_t     ch_lock;
};

/* prototypes */

int  logg_init();
void logg_end();

LOGG_CHANN_OPS  logg_channop_lookup(
        char *name);

LOGG_CHANN      logg_chann_open(
        LOGG_CHANN_OPS  channops,
        char           *name,
                        ...);

LOGG_CHANN      logg_chann_vopen(
        LOGG_CHANN_OPS channops,
        char           *name,
        va_list         args);

int             logg_chann_close(
        LOGG_CHANN       chann);

LOGG_LOG logg_add_log(
        int             prio,
        const char     *file,
        const char     *func,
        const int       line_start,
        const int       line_end,
        const int       flags,
        LOGG_CHANN      chan);

int logg_del_log(
        LOGG_LOG        log);

ssize_t loggv(
        int             crit,
        const char     *file,
        const int       line,
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

/* support for modules register/unregister */

#define LOGG_INIT_FUNC(modname, parname) AVL_ITERATOR modname##_init(const char *parname)
#define LOGG_FINI_FUNC(modname, parname) int modname##_fini(const char *parname)

AVL_ITERATOR logg_register_channops(
        const char     *name,
        LOGG_CHANN_OPS  channops);

int logg_unregister_channops(
        LOGG_CHANN_OPS   channops);

#endif /* LOGGER_H */
/* Do not include anything AFTER the line above, as it would not be
 * protected against double inclusion from other files.  */
/* $Id: header.h.m4,v 1.7 2005/11/07 19:39:53 luis Exp $ */
