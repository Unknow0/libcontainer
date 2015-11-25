/*******************************************************************************
 * This file is part of libutils.
 *
 * libutils is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * libutils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along
 * with libutils; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 ******************************************************************************/
#ifndef _UTILS_BUS_H
#define _UTILS_BUS_H

#include <pthread.h>

typedef struct bus_event_t
	{
	void (*callback)(void*);
	void *arg;
	} bus_event_t;

typedef struct bus_t
	{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	pthread_t loop;

	size_t event_off;
	size_t event_count;
	size_t event_size;
	bus_event_t event[0];
	} bus_t;


bus_t *bus_create(size_t max_event);

int bus_add(bus_t *bus, void (*callback)(void*), void *arg);

void bus_destroy(bus_t *bus);

#endif
