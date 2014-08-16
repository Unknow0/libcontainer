/*******************************************************************************
 * This file is part of libcontainer.
 *
 * libcontainer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * libcontainer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along
 * with libcontainer; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 ******************************************************************************/

#include "container/chunked_string.h"

#include <string.h>
#include <stdio.h>

struct e
	{
	int i;
	char *v;
	};

int main(int argc, char** argv)
	{
	char s[16];
	int i;
	chunked_string_t *l=chunked_string_create(35);

	
	for(i=0; i<200; i++)
		{
		sprintf((char *)&s, "i am: %d ", i);
		chunked_string_add(l, s);
		}


	struct str_chunk *c=l->head;
	while(c!=NULL)
		{
		printf("%.35s", c->data);
		c=c->next;
		}
	printf("\n");
	
	return 0;
	}
