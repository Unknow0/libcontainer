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
#include <logger.h>

#include <sys/inotify.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#include "utils/hashmap.h"
#include "utils/watch.h"

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_SIZE (1024*EVENT_SIZE)

typedef struct 
	{
	int wd;
	char *path;
	} watch_desc;

static logger_t *log;

static void *watch_loop(void *NA);

size_t hash_desc(void *e)
	{
	return (size_t)((watch_desc*)e)->wd;
	}
void watch_desc_destroy(void *e)
	{
	watch_desc *w=(watch_desc *)e;
	free(w->path);
	free(w);
	}

void watch_destroy(watch_t *w)
	{
	pthread_cancel(w->thread);
	hashmap_destroy(w->descriptors);
	close(w->fd);
	}

watch_t *watch_create(void *payload)
	{
	log=get_logger("mserver.watch");
	watch_t *w=malloc(sizeof(watch_t));
	if(!w)
		return NULL;
	w->fd=inotify_init();
	if(w->fd<0)
		goto error;
	w->descriptors=hashmap_create(10, .75, &hash_desc, &watch_desc_destroy);
	if(!w->descriptors)
		goto error;
	w->payload=payload;
	pthread_create(&w->thread, NULL, &watch_loop, w);
	return w;
error:
	free(w);
	return NULL;
	}

int watch(watch_t *w, const char *dir)
	{
	watch_desc *desc;
	int wd;
	if(!w)
		return 1;

	wd=inotify_add_watch(w->fd, dir, IN_CLOSE_WRITE | IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MOVED_FROM | IN_MOVED_TO);
	if(wd<0)
		return wd;
	desc=malloc(sizeof(watch_desc));
	desc->wd=wd;
	desc->path=strdup(dir);
	hashmap_add(w->descriptors, desc);
	return 0;
	}

static void *watch_loop(void *watcher)
	{
	watch_t *w=(watch_t*)watcher;
	int i;
	int len;
	unsigned char buf[BUF_SIZE];

	while(1)
		{
		i=0;
		len=read(w->fd, buf, BUF_SIZE);
		if(len<0)
			{
			if(errno == EINTR)
				continue;
			// TODO log
			}
		
		while(i<len)
			{
			struct inotify_event *e=(struct inotify_event *)&buf[i];
			i+=EVENT_SIZE+e->len;
			
			if(w->event)
				{
				watch_desc *desc=hashmap_get(w->descriptors, e->wd);
				if(desc)
					w->event(e, desc->path, w->payload);
				}
			}
		}
	}
