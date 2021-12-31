/*
 * table.h - generic hash table
 *
 * No algorithm should use these functions directly, because
 *  programming with void* is too error-prone.  Instead,
 *  each module should make "wrapper" functions that take
 *  well-typed arguments and call the TAB_ functions.
 */
#pragma once
#ifdef __cplusplus
extern "C" {
#endif

typedef struct TAB_table_ *TAB_table;

/* Make a new table mapping "keys" to "values". */
TAB_table TAB_empty(void);

/* Enter the mapping "key"->"value" into table "t", 
 *    shadowing but not destroying any previous binding for "key". */
void TAB_insert(TAB_table t, void *key, void *value);

/* Look up the most recent binding for "key" in table "t" */
void *TAB_search(TAB_table t, void *key);

/* Pop the most recent binding and return its key.
 * This may expose another binding for the same key, if there was one. */
void *TAB_pop(TAB_table t);


/* Call "show" on every "key"->"value" pair in the table,
 *  including shadowed bindings, in order from the most 
 *  recent binding of any key to the oldest binding in the table */
void TAB_foreach(TAB_table t, void (*show)(void *key, void *value));


#ifdef __cplusplus
}
#endif