/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  // check if pointer to the table is not NULL
  if (table != NULL)
  {
    // set all the pointers to the cells to NULL
    for (int i = 0; i < HT_SIZE; i++)
    {
      (*table)[i] = NULL;
    }
  }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  // check if pointer to the table is not NULL
  if (table != NULL)
  {
    // find the row according to the key
    ht_item_t *item = (*table)[get_hash(key)];
    // go through all the items in that row
    // and if key of any of them is equal to the key
    // we are looking for then return it
    // otherwise return NULL
    while (item != NULL)
    {
      if (key == item->key)
      {
        return item;
      }
      else
      {
        item = item->next;
      }
    }
  }
  return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  // check if pointer to the table is not NULL
  if (table != NULL)
  {
    // find the row to which we will add a new item
    ht_item_t *item = ht_search(table, key);
    // check if there is already an item with the key we need
    // if yes then update the value
    if (item != NULL)
    {
      item->value = value;
    }
    // if not then create a new item
    else
    {
      ht_item_t *tmp = (ht_item_t *) malloc(sizeof(ht_item_t));
      int connect = get_hash(key);
      // check malloc
      if (tmp != NULL)
      {
        // set up the data, key and connect the item to the table
        tmp->key = key;
        tmp->value = value;
        tmp->next = (*table)[connect];
        (*table)[connect] = tmp;
      }
    }
  }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  // check is pointer to the table is not NULL
  if (table != NULL)
  {
    // find the item in the table
    ht_item_t *tmp = ht_search(table, key);
    // if item exists return its value
    // otherwise return NULL
    if (tmp != NULL)
    {
      return &tmp->value;
    }
  }
  return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  // check if pointer to the table is not NULL
  if (table != NULL)
  {
    // create helpful items to delete an item
    // get hash to find the row we need to look in
    ht_item_t *current = (*table)[get_hash(key)];
    ht_item_t *previous = NULL;
    // helpful bool to differentiate the case when
    // we need to delete the first item in the row
    bool first = true;
    // go through all the items in the row
    while (current != NULL)
    {
      // if the item we need to delete is the first item in the row
      if (current->key == key && first)
      {
        (*table)[get_hash(key)] = NULL;
        free(current);
      }
      // otherwise
      else
      {
        // the item we need to delete is not the first one
        first = false;
        // save the pointer on the previous item
        previous = current;
        current = current->next;
        
        if (current->key == key)
        {
          previous->next = current->next;
          free(current);
          return;
        }
      }
    }
  }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
  if (table != NULL)
  {
    // create helpful items to delete all
    ht_item_t *current;
    ht_item_t *next;
    // go through all the rows
    for (int i = 0; i < HT_SIZE; i++)
    {
      // go through all the items in the row
      current = (*table)[i];
      while (current)
      {
        // go to the next item
        next = current->next;
        free(current);
        // save the pointer to the current item
        current = next;
      }
      // set the pointer to the row to NULL
      (*table)[i] = NULL;
    }
  }
}