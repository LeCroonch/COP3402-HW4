/* $Id: symtab.c,v 1.4 2023/11/06 09:36:24 leavens Exp $ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "symtab.h"
#include "utilities.h"

// For a data structure, we use an array (for now), of associations,
// although a hash table would be better...

// associations in the symbol table
typedef struct assoc_s {
    const char *name;
    id_attrs attrs;
} assoc_t;

// Requires: name != NULL && attrs != NULL;
// Return a fresh association from the given name to the given attributes
static assoc_t *assoc_create(const char *name, id_attrs attrs)
{
    assert(name != NULL);
    assoc_t *ret = (assoc_t *)malloc(sizeof(assoc_t));
    if (ret == NULL) {
	bail_with_error("Cannot allocate space for an assoc_t in symtab!");
    }
    ret->name = name;
    ret->attrs = attrs;
    return ret;
}

// size is also the index of the next element to allocate
static int size;
// The data structure is such that the first size entries contain actual data
static assoc_t *entries[MAX_SYMTAB_SIZE];

// The symbol table's invariant
void symtab_okay()
{
    assert(0 <= size);
    assert(size < MAX_SYMTAB_SIZE);
    for (int i = 0; i < size; i++) {
	assert(entries[i] != NULL);
	assert(entries[i]->name != NULL);
    }
}

// initialize the symbol table
void symtab_initialize()
{
    size = 0; // no data yet
    symtab_okay();
}

// Return the number of mappings in this symbol table
unsigned int symtab_size() { return size; }

// Is this symbol table empty? (I.e., does it have not mappings?)
bool symtab_empty() { return size == 0; }

// Is this symbol table full? (I.e., can it not hold more mappings?)
// (i.e., is symtab_size() equal to MAX_SYMTAB_SIZE-1)?
bool symtab_full() { return size >= MAX_SYMTAB_SIZE; }

// Is the given name associated with some attributes?
bool symtab_declared(const char *name)
{
    id_attrs *v = symtab_lookup(name);
    return v != NULL;
}    

// Requires: !symtab_full
// Requires: !symtab_declared(attrs.name)
// Remember the given attributes (i.e., an association from attrs.name
// to the other parts of attrs)
void symtab_insert(const char *name, id_attrs attrs)
{
    if (symtab_full()) {
	bail_with_error("The symtab is full!");
    }
    entries[size++] = assoc_create(name, attrs);
}

// if name == NULL or if name has no associations, return -1
// if name has an association in the table, return its index
static int find_index(const char *name)
{
    if (name == NULL) {
	return -1;
    }
    for (int i = 0; i < size; i++) {
	if (strcmp(entries[i]->name, name) == 0) {
	    return i;
	}
    }
    return -1;
}


// Return (a pointer to) the attributes of the given name
// or NULL if there is no association for that name.
id_attrs *symtab_lookup(const char *name)
{
    int i = find_index(name);
    if (0 <= i) {
	return &(entries[i]->attrs);
    } else {
	return NULL;
    }
}

// iteration helpers
// iterations use an external key which is a name

// Start an iteration by returning the first name in the symbol table,
// return NULL if symtab_empty()
const char *symtab_first_name()
{
    if (symtab_empty()) {
	return NULL;
    }
    assert(0 < size);
    return entries[0]->name;
}

// Are there more names defined in the symbol table after the given one?
// This returns false if name is NULL, if name is not defined,
// or if there are no more names following name in the symbol table
bool symtab_more_after(const char *name)
{
    int i = find_index(name);
    return (0 <= i && i < size);
}

// Requires: symtab_more_after(name);
// Return the next name defined in the symbol table after the given one,
// but return NULL if there are no more names
const char *symtab_next_name(const char *name)
{
    int i = find_index(name);
    if (i < 0 || i >= size) {
	return NULL;
    } else {
	return entries[i+1]->name;
    }
}
