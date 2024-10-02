/*
 * Copyright (C) 2011, Nokia <ivan.frade@nokia.com>
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
 * Author: Carlos Garnacho <carlos@lanedo.com>
 */

#include "config-miners.h"

#include "tracker-extract-info.h"

/**
 * SECTION:tracker-extract-info
 * @title: TrackerExtractInfo
 * @short_description: struct used to pass information to and from
 *                     a Tracker extract module
 * @stability: Stable
 * @include: libtracker-extract/tracker-extract.h
 *
 * The #TrackerExtractInfo structure is used to pass information
 * on the file being extracted to an extractor module and contains
 * objects to hold the SPARQL updates generated by the extractor.
 **/


struct _TrackerExtractInfo
{
	TrackerResource *resource;

	GFile *file;
	gchar *content_id;
	gchar *mimetype;
	gchar *graph;

	gint max_text;

	gint ref_count;
};

G_DEFINE_BOXED_TYPE (TrackerExtractInfo, tracker_extract_info,
                     tracker_extract_info_ref, tracker_extract_info_unref)

/**
 * tracker_extract_info_new:
 * @file: a #GFile
 * @mimetype: mimetype for @file
 * @graph: SPARQL graph used for inserting data
 *
 * Returns a newly created #TrackerExtractInfo
 *
 * Returns: (transfer full): (boxed): A newly allocated #TrackerExtractInfo
 *
 * Since: 0.12
 **/
TrackerExtractInfo *
tracker_extract_info_new (GFile       *file,
                          const gchar *content_id,
                          const gchar *mimetype,
                          const gchar *graph,
                          gint         max_text)
{
	TrackerExtractInfo *info;

	g_return_val_if_fail (G_IS_FILE (file), NULL);
	g_return_val_if_fail (content_id && *content_id, NULL);

	info = g_slice_new0 (TrackerExtractInfo);
	info->file = g_object_ref (file);
	info->content_id = g_strdup (content_id);
	info->mimetype = g_strdup (mimetype);
	info->graph = g_strdup (graph);
	info->max_text = max_text;

	info->resource = NULL;

	info->ref_count = 1;

	return info;
}

/**
 * tracker_extract_info_ref:
 * @info: a #TrackerExtractInfo
 *
 * Increases the reference count of @info
 *
 * Returns: A new reference to @info
 *
 * Since: 0.12
 **/
TrackerExtractInfo *
tracker_extract_info_ref (TrackerExtractInfo *info)
{
	g_return_val_if_fail (info != NULL, NULL);

	g_atomic_int_inc (&info->ref_count);

	return info;
}

/**
 * tracker_extract_info_unref:
 * @info: a #TrackerExtractInfo
 *
 * Decreases the reference count of @info, freeing all its associated resources
 * if it reaches 0.
 *
 * Since: 0.12
 **/
void
tracker_extract_info_unref (TrackerExtractInfo *info)
{
	g_return_if_fail (info != NULL);

	if (g_atomic_int_dec_and_test (&info->ref_count)) {
		g_object_unref (info->file);
		g_free (info->content_id);
		g_free (info->mimetype);
		g_free (info->graph);

		if (info->resource)
			g_object_unref (info->resource);

		g_slice_free (TrackerExtractInfo, info);
	}
}

/**
 * tracker_extract_info_get_file:
 * @info: a #TrackerExtractInfo
 *
 * Returns a #GFile pointing to the file being affected
 * by the metadata extraction represented by @info
 *
 * Returns: (transfer none): The file being inspected
 *
 * Since: 0.12
 **/
GFile *
tracker_extract_info_get_file (TrackerExtractInfo *info)
{
	g_return_val_if_fail (info != NULL, NULL);

	return info->file;
}

gchar *
tracker_extract_info_get_content_id (TrackerExtractInfo *info,
                                     const gchar        *suffix)
{
	g_return_val_if_fail (info != NULL, NULL);

	if (suffix)
		return g_strconcat (info->content_id, "/", suffix, NULL);
	else
		return g_strdup (info->content_id);
}

/**
 * tracker_extract_info_get_mimetype:
 * @info: a #TrackerExtractInfo
 *
 * Returns the mimetype being used for the file
 * metadata extraction.
 *
 * Returns: (transfer none): the mimetype being used
 *          for extraction.
 *
 * Since: 0.12
 **/
const gchar *
tracker_extract_info_get_mimetype (TrackerExtractInfo *info)
{
	g_return_val_if_fail (info != NULL, NULL);

	return info->mimetype;
}

const gchar *
tracker_extract_info_get_graph (TrackerExtractInfo *info)
{
	g_return_val_if_fail (info != NULL, NULL);

	return info->graph;
}

/**
 * tracker_extract_info_get_resource:
 * @info: a #TrackerExtractInfo
 *
 * Returns the #TrackerResource representing metadata about the file
 * associated with this #TrackerExtractInfo, or %NULL if
 * tracker_extract_info_set_metadata() was not yet called.
 *
 * Returns: (transfer none): a #TrackerResource instance
 *
 * Since: 1.10
 */
TrackerResource *
tracker_extract_info_get_resource (TrackerExtractInfo *info)
{
	return info->resource;
}

/**
 * tracker_extract_info_set_resource:
 * @info: a #TrackerExtractInfo
 * @resource: a #TrackerResource
 *
 * Adds the #TrackerResource with results from the extraction to this
 * #TrackerExtractInfo.
 *
 * Information about the file itself should be represented by properties of
 * @resource itself. It's expected this resource will have nfo:FileDataObject
 * as one of its types. This @resource can have related resources attached to
 * it.
 *
 * In most cases, a file contains a single logical resource. Most MP3 files
 * contain one song, for example. In this case you set all properties on the
 * one @resource.
 *
 * In more complex cases, a single physical resource (i.e. a file) contains multiple
 * logical resources: for example, an MBOX file holding multiple emails, or
 * an audio file containing an entire CD. In this case you should treat each
 * logical resource as its own #TrackerResource. Only properties of the file
 * itself should be set on @resource. You then relate each logical
 * #TrackerResource to the main @resource using the nie:isStoredAs property.
 *
 * FIXME: you need a way to delete the logical resources when re-extracting a
 * file -- still need to decide on API for that.
 *
 * Since: 1.10
 **/
void
tracker_extract_info_set_resource (TrackerExtractInfo *info,
                                   TrackerResource    *resource)
{
	g_object_ref (resource);
	info->resource = resource;
}

gint
tracker_extract_info_get_max_text (TrackerExtractInfo *info)
{
	return info->max_text;
}