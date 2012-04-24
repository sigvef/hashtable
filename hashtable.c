#include "hashtable.h"

#include <stdlib.h>
#include <string.h>

unsigned int get_hash(char *string) {
	unsigned int hash_value = 0;

	while (*string != '\0') {
		hash_value = *string + (hash_value << 5) - hash_value;
		string++;
	}

	return hash_value;
}

unsigned int get_hash_table_position(hash_table_t *hash_table, char *string) {
    int hash_value = get_hash(hash_table, string);

    int position = hash_value % (1 << hash_table->exponent);
    if (position < hash_table->next_split) {
        position = hash_value % (1 << (hash_table->exponent + 1));
    }
    
    return position;
}

hash_table_t *get_hash_table(int size) {
    int i;

    hash_table_t *hash_table = (hash_table_t *) malloc(sizeof(hash_table_t));
    if (hash_table == NULL) {
        return NULL;
    }
    
    hash_table->elements = (hash_table_list_t *) malloc(sizeof(hash_table_list_t) * size);
    if (hash_table->elements == NULL) {
        free(hash_table);
        return NULL;
    }

    for (i = 0; i < size; i++) {
        hash_table->elements[i] = NULL;
    }

    hash_table->size = size;

    for (i = size; i > 1; i >> 1) {
        hash_table->exponent++;
    }

    hash_table->next_split = 0;
    hash_table->n_elements = 0;

    return hash_table;
}

int has_hash_table_element(hash_table_t *hash_table, char *string) {
    int position = get_hash_table_position(hash_table, string);
    hash_table_list_t *list = hash_table->elements[position];

    while (list != NULL) {
        if (strcmp(string, list->string) == 0) {
            return 1;
        }
        list = list->next;
    }

    return 0; 
}

hash_table_list_t *get_hash_table_element(hash_table_t *hash_table, char *string) {
    int position = get_hash_table_position(hash_table, string);
    hash_table_list_t *list = hash_table->elements[position];

    while (list != NULL) {
        if (strcmp(string, list->string) == 0) {
            return list;
        }
        list = list->next;
    }

    return NULL;
}

hash_table_list_t *add_hash_table_element(hash_table_t *hash_table, char *string) {
    if (has_hash_table_element(hash_table, string)) {
        return get_hash_table_element(hash_table, string);
    }

    int position = get_hash_table_position(hash_table, string);
    ht_table_list_t *new_element = malloc(ht_table_list_t);
    if (new_element == NULL) {
        return NULL;
    }

    int length = strlen(string) + 1;
    new_element->string = (char *) malloc(sizeof(char) * length);
    if (new_element->string == NULL) {
        free(new_element);
        return NULL;
    }
    memcpy(new_element->string, string, length);

    new_element->next = NULL;

    ht_table_list_t **list_element = &hash_table->elements[position];
    while (*list_element != NULL) {
        *list_element = *list_element->next;
    }

    *list_element = new_element;
    hash_table->n_elements++;

    if (((float) hash_table->n_elements/hash_table->size) > HASH_TABLE_GROW_SIZE) {
        //TODO: grow the table
    }

    return new_element;
}
