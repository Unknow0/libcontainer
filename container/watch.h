/*******************************************************************************
 * This file is part of mserver.
 *
 * mserver is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * mserver is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along
 * with mserver; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 ******************************************************************************/
#ifndef _WATCH_H
#define _WATCH_h

#include <sys/inotify.h>
#include <container/hashmap.h>

typedef struct
	{
	int fd;
	hashmap_t *descriptors;
	pthread_t thread;
	
	// call for each event
	void (*event)(struct inotify_event *e);
	} watch_t;

watch_t *watch_create();
void watch_destroy(watch_t*);

int watch(watch_t *w, const char *dir);

#endif
