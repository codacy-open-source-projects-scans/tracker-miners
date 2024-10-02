/*
 * Copyright (C) 2010, Nokia <ivan.frade@nokia.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef __MINERS_MOCK_H__
#define __MINERS_MOCK_H__

#include <glib.h>

G_BEGIN_DECLS

#define MOCK_MINER_1 "org.freedesktop.Tracker3.Miner.Mock1"
#define MOCK_MINER_2 "org.freedesktop.Tracker3.Miner.Mock2"

/*
 * Assumptions:
 *
 *  There are this two miners, 
 *  Initial state: Mock1 is running, Mock2 is paused
 *
 */
void    miners_mock_init (void);

G_END_DECLS


#endif
