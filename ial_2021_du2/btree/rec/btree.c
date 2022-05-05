/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  // set the pointer to the root to NULL
  *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  // check if pointer to the root is not NULL
  // if yes then tree is empty, return false
  if (tree == NULL)
  {
    return false;
  }
  // if tree is not empty
  else
  {
    // if node's key is equal to the key
    // set the pointer to the value
    // and return true
    if (tree->key == key)
    {
      *value = tree->value;
      return true;
    }
    // otherwise call this function for the right or left
    // subtree according to the node's key
    else
    {
      if (key < tree->key)
      {
        return (bst_search(tree->left, key, value));
      }
      else
      {
        return (bst_search(tree->right, key, value));
      }
    }
  }
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  // it tree is empty
  if (*tree == NULL)
  {
    // create the new node and set up all the data as leaf
    *tree = (bst_node_t *) malloc(sizeof(bst_node_t));
    if (*tree != NULL)
    {
      (*tree)->key = key;
      (*tree)->value = value;
      (*tree)->left = NULL;
      (*tree)->right = NULL;
    }
  }
  // tree is not empty
  else
  {
    // if the node with the same key already exists
    // update its value
    if ((*tree)->key == key)
    {
      (*tree)->value = value;
    }
    // call this function for the left or right subtree according to the node's key
    else
    {
      if ((*tree)->key < key)
      {
        bst_insert(&((*tree)->right), key, value);
      }
      else 
      {
        bst_insert(&((*tree)->left), key, value);
      }
    }
  }   
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  // check if tree is not empty
  if (*tree != NULL)
  {
    // if node has right subtree then call this function for the right subtree
    if ((*tree)->right != NULL)
    {
      bst_replace_by_rightmost(target, &((*tree)->right));
    }
    // pointer to the tree is NULL
    else
    {
      // write the data of the most right node to the target node
      target->key = (*tree)->key;
      target->value = (*tree)->value;
      // save its left subtree
      bst_node_t *tmp = *tree;
      *tree = tmp->left;
      free(tmp);
    }  
  }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  // check if tree is not empty
  if ((*tree) != NULL)
  {
    // if node's key is equal to the key
    if ((*tree)->key == key)
    {
      // if it has both subtrees
      // replace it with the most right node
      if ((*tree)->left != NULL && (*tree)->right != NULL)
      {
        bst_replace_by_rightmost((*tree), &((*tree)->left));
      }
      // if it has only left subtree
      else if ((*tree)->left != NULL && (*tree)->right == NULL)
      {
        // store left subtree, free the node and connect the left subtree back
        bst_node_t *tmp = (*tree)->left;
        free(*tree);
        *tree = tmp;
      }
      // if it has only right subtree
      else if ((*tree)->left == NULL && (*tree)->right != NULL)
      {
        // store right subtree, free the node and connect the right subtree back
        bst_node_t *tmp = (*tree)->right;
        free(*tree);
        *tree = tmp;
      }
      // if it has no subtrees
      else
      {
        // free the node
        free(*tree);
        *tree = NULL;
      }
    }
    // if node's key is different
    // call this function for the left or right subtree
    // depending on the key of the node
    else
    {
      if (key < (*tree)->key)
      {
        bst_delete(&((*tree)->left), key);
      }
      else
      {
        bst_delete(&((*tree)->right), key);
      }
    }
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  // if tree is not empty
  if (*tree != NULL)
  {
    // call this function for both subtrees of the node
    bst_dispose(&(*tree)->left);
    bst_dispose(&(*tree)->right);
    // free the node
    free(*tree);
    *tree = NULL;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  // check if tree is not empty
  if (tree != NULL)
  {
    // print the node, call this function for both subtrees
    bst_print_node(tree);
    bst_preorder(tree->left);
    bst_preorder(tree->right);
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  // check if tree is not empty
  if (tree != NULL)
  {
    // call this function for the left subtree
    // print the node
    // call this function for the right subtree
    bst_preorder(tree->left);
    bst_print_node(tree);
    bst_preorder(tree->right);
  }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  // check if tree is not empty
  if (tree != NULL)
  {
    // call this function for both subtrees and print the node
    bst_preorder(tree->left);
    bst_preorder(tree->right);
    bst_print_node(tree);
  }
}
