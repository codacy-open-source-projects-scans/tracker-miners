/*
 * Copyright (C) 2020, Red Hat Ltd
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 *
 * Author: Carlos Garnacho <carlosg@gnome.org>
 */

#ifndef __TRACKER_ERROR_REPORT_H__
#define __TRACKER_ERROR_REPORT_H__

#include <gio/gio.h>

#define TRACKER_TYPE_ERROR_REPORT (tracker_error_report_get_type ())
G_DECLARE_FINAL_TYPE (TrackerErrorReport,
                      tracker_error_report,
                      TRACKER, ERROR_REPORT,
                      GObject)

TrackerErrorReport * tracker_error_report_new (GError **error);

void tracker_error_report_save (TrackerErrorReport *errors,
                                GFile              *file,
                                const gchar        *error_message,
                                const gchar        *sparql);

void tracker_error_report_delete (TrackerErrorReport *errors,
                                  GFile              *file);

#endif /* __TRACKER_ERROR_REPORT_H__ */
