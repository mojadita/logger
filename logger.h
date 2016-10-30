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
#include "avl_c/avl.h"
#include "lists/lists.h"

/* constants */
/* the five less signifiant bits are there to allow for extension of
 * logger levels, so more granularity is allowed than the proposed
 * standard. */
#define LOGG_DEB               (7 << 5)
#define LOGG_INF               (6 << 5)
#define LOGG_WAR               (5 << 5)
#define LOGG_ERR               (4 << 5)
#define LOGG_EMG               (3 << 5)
#define LOGG_CRT               (2 << 5)
#define LOGG_FAT               (1 << 5)

#define DEBUG(fmt, args...)    log(LOGG_DEB, __FILE__, __LINE__, __func__, (fmt), ##args)
#define INFO(fmt, args...)     log(LOGG_INF, __FILE__, __LINE__, __func__, (fmt), ##args)
#define WARNING(fmt, args...)  log(LOGG_WAR, __FILE__, __LINE__, __func__, (fmt), ##args)
#define ERROR(fmt, args...)    log(LOGG_ERR, __FILE__, __LINE__, __func__, (fmt), ##args)
#define EMERG(fmt, args...)    log(LOGG_EMG, __FILE__, __LINE__, __func__, (fmt), ##args)
#define CRITICAL(fmt, args...) log(LOGG_CRT, __FILE__, __LINE__, __func__, (fmt), ##args)
#define FATAL(fmt, args...)    log(LOGG_FAT, __FILE__, __LINE__, __func__, (fmt), ##args)

#define LOGG_TIMESTAMP         (1 << 0) /* do we output timestamp */
#define LOGG_FILENAME          (1 << 1) /* do we output filename */
#define LOGG_LINENUM           (1 << 2) /* '' line number */
#define LOGG_FUNCNAME          (1 << 3) /* '' function name */
#define LOGG_COLOR             (1 << 4) /* '' ansi color sequences(if channel admits it) */

#define LOGG_ALL               (LOGG_TIMESTAMP | LOGG_FILENAME |\
                                LOGG_LINENUM   | LOGG_FUNCNAME)

/* types */

typedef struct logger_chann_class_s *LOGGER_CHAN_CLASS;
typedef struct logger_chann_s       *LOGGER_CHANN;
typedef struct logger_s             *LOGGER;

/* prototypes */

int logg_init();
void logg_end();

ssize_t loggv(
        int             crit,
        const char     *file,
        const int       line_start,
        const int       line_end,
        const char     *func,
        const char     *fmt,
        va_list         args);

ssize_t logg(
        int             crit,
        const char     *file,
        const int       line_start,
        const int       line_end,
        const char     *func,
        const char     *fmt,
        ...);

LOGGER_CTX
logger_new(
        char          **envp);

LOGGER_CHANN
logger_chan_new(LOGGER_CTX ctx, char *typ, ...);

LOGGER logger_new(
        LOGGER_CTX      ctx,
        LOGGER_CHANN    chn, 
        int             flags,
        int             crit,
        char           *file,
        int             line_start,
        int             line_end,
        char           *func,
        LOGG_CHANN_P    chan);

#endif /* LOGGER_H */
/* Do not include anything AFTER the line above, as it would not be
 * protected against double inclusion from other files.  */
/* $Id: header.h.m4,v 1.7 2005/11/07 19:39:53 luis Exp $ */
