libutils
============

simple c utils library

### Install
	autoreconf -fis
	./configure
	make
	make install

#### Container

arraylist, linkedlist and chunkedlist store copy of elements. So they should all have the same size.

Hashmap use pointer on element.

#### String

use a string cache to reuse static string (they should not be modified)

#### Watch

a watch on file & directory

#### Bus

a call asynchrone callback bus
