#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct HashMap HashMap;
int enlarge_called = 0;

struct HashMap
{
    Pair **buckets;
    long size;     // cantidad de datos/pairs en la tabla
    long capacity; // capacidad de la tabla
    long current;  // indice del ultimo dato accedido
};

Pair *createPair(char *key, void *value)
{
    Pair *new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash(char *key, long capacity)
{
    unsigned long hash = 0;
    char *ptr;
    for (ptr = key; *ptr != '\0'; ptr++)
    {
        hash += hash * 32 + tolower(*ptr);
    }
    return hash % capacity;
}

int is_equal(void *key1, void *key2)
{
    if (key1 == NULL || key2 == NULL)
        return 0;
    if (strcmp((char *)key1, (char *)key2) == 0)
        return 1;
    return 0;
}

void insertMap(HashMap *map, char *key, void *value)
{
    long pos = hash(key, map->capacity);
    Pair *nuevo = createPair(key, value);
    if (map->buckets[pos] == NULL)
    {
        map->buckets[pos] = nuevo;
        map->size++;
    }
    else
    {
        while (map->buckets[pos] != NULL)
        {
            if (is_equal(map->buckets[pos]->key, key))
            {
                map->buckets[pos]->value = value;
                free(nuevo);
                return;
            }
            pos++;
            pos = pos % map->capacity;
        }
        map->buckets[pos] = nuevo;
        map->size++;
    }
}

void enlarge(HashMap *map)
{
    long old_capacity = map->capacity;
    Pair **old_buckets = map->buckets;

    long new_capacity = old_capacity * 2;
    Pair **new_buckets = calloc(new_capacity, sizeof(Pair *));
    if (new_buckets == NULL)
        return;

    map->capacity = new_capacity;
    map->buckets = new_buckets;
    map->size = 0;
    map->current = 0;

    for (long i = 0; i < old_capacity; i++)
    {
        if (old_buckets[i] != NULL)
        {
            insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
        }
    }
    free(old_buckets);
}

HashMap *createMap(long capacity)
{
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    if (map == NULL)
        return NULL;
    map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
    if (map->buckets == NULL)
    {
        free(map);
        return NULL;
    }
    map->capacity = capacity;
    map->size = 0;
    map->current = -1;
    return map;
}

void eraseMap(HashMap *map, char *key)
{
    long pos = hash(key, map->capacity);
    if (map->buckets[pos] == NULL)
        return;
    while (map->buckets[pos] != NULL)
    {
        if (is_equal(map->buckets[pos]->key, key))
        {
            free(map->buckets[pos]->key);
            free(map->buckets[pos]->value);
            free(map->buckets[pos]);
            map->buckets[pos] = NULL;
            map->size--;

            long siguiente = (pos + 1) % map->capacity;
            while (map->buckets[siguiente] != NULL)
            {
                Pair *temp = map->buckets[siguiente];
                map->buckets[siguiente] = NULL;
                map->size--;

                insertMap(map, temp->key, temp->value);
                free(temp);

                siguiente = (siguiente + 1) % map->capacity;
            }
        }
        pos++;
        pos = pos % map->capacity;
    }
}

Pair *searchMap(HashMap *map, char *key)
{

    return NULL;
}

Pair *firstMap(HashMap *map)
{

    return NULL;
}

Pair *nextMap(HashMap *map)
{

    return NULL;
}
