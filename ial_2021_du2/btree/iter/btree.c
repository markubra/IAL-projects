/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  // helpful bool to detect if item was found
  bool found = false;
  // check if pointer to the root is not NULL
  // if yes then tree is empty, return false
  if (tree == NULL)
  {
    found = false;
  }
  // if tree is not empty
  else
  {
    // go inside until pointer to the node is not NULL
    while (tree != NULL)
    {
      // if node has the key we are looking for
      // return true
      if (tree->key == key)
      {
        found = true;
        return found;
      }
      // if not then compare the key of the node with key in arguments
      // if node's key is bigger go to the left subtree otherwise to the right
      else
      {
        if (tree != NULL)
        {
          if (key < tree->key)
          {
            tree = tree->left;
          }
          else
          {
            tree = tree->right;
          }
        }
        // return false if the pointer in NULL
        else
        {
          found = false;
          return found;
        }
      }
    }
  }
  // return the bool
  return found;
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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  // create helpful variables
  bst_node_t *tmp = (*tree);
  bst_node_t *after_this = NULL;
  // check if tree is empty
  // if yes then insert the first node and return
  if (*tree == NULL)
  {
    bst_node_t *new = (bst_node_t *) malloc(sizeof(bst_node_t));
    // check malloc
    if (new == NULL)
    {
      return;
    }
    // set up the data of the child node
    new->key = key;
    new->value = value;   
    new->left = NULL;
    new->right = NULL;
    *tree = new;
    return;
  }
  // if tree is not empty
  else
  {
    // helpful bool to know which side 
    bool right;
    // go through the until NULL
    while (tmp != NULL)
    {
      // save the node before NULL
      after_this = tmp;
      // if node with this key already exists update its value
      if (tmp->key == key)
      {
        tmp->value = value;
        return;
      }
      // node's key is smaller go to the right subtree
      else if (tmp->key < key)
      {
        tmp = tmp->right;
        right = true;
      }
      // otherwise to the left
      else
      {
        tmp = tmp->left;
        right = false;
      }
    }
    // there is no node with the same key
    // create a new one depending on the key
    bst_node_t *new = (bst_node_t *) malloc(sizeof(bst_node_t));
    // check malloc
    if (new == NULL)
    {
      return;
    }
    // set up the data of the child node
    new->key = key;
    new->value = value;   
    new->left = NULL;
    new->right = NULL;

    if (right)
    {
      after_this->right = new;
    }
    else
    {
      after_this->left = new;
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
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  // check if tree is not empty
  if (*tree != NULL)
  {
    // create helpful pointers
    bst_node_t *current = *tree;
    bst_node_t *previous = NULL;
    // go until the most right subtree
    while (current->right != NULL)
    {
      // save a pointer to the previous node
      previous = current;
      // go to the right
      current = current->right;
    }
    // write data of the most right subtree to the target node
    target->key = current->key;
    target->value = current->value;
    // don't forget about the left subtree of the most right node
    previous->right = current->left;
    // free the node
    free(current);
    current = NULL;
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
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  // check if tree is not empty
  if ((*tree) != NULL)
  {
    // create helpful pointers
    bst_node_t *current = *tree;
    bst_node_t *previous = NULL;
    // go until NULL or if key is found
    while (current != NULL && current->key != key)
    {
      // save the pointer to the previous node
      previous = current;
      // if node's key is bigger then go to the left
      if (current->key > key)
      {
        current = current->left;
      }
      //otherwise to the right
      else
      {
        current = current->right;
      }
    }
    // check if the node with the key we need was found
    if (current != NULL)
    {
      // if it has two subtrees then replace it by the most right node
      if (current->left != NULL && current->right != NULL)
      {
        bst_replace_by_rightmost(current, &(current->left));
      }
      // otherwise
      else
      {
        // create healful pointer
        bst_node_t *tmp = NULL;
        // if it has only left subtree
        if (current->left != NULL && current->right == NULL)
        {
          // store its left subtree
          tmp = current->left;
        }
        // if it has only right subtree
        else if (current->left == NULL && current->right != NULL)
        {
          // store its right subtree
          tmp = current->right;
        }
        // if the node we want to delete is the left subtree
        // of the previous node then set the pointer on the left subtree
        // to the stored subtree
        if (current == previous->left)
        {
          previous->left = tmp;
        }
        // if the node we want to delete is the right subtree
        // of the previous node then set the pointer on the right subtree
        // to the stored subtree
        else
        {
          previous->right = tmp;
        }
        // free the node
        free(current);
        current = NULL;
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
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  // check if tree is empty
  if (*tree != NULL)
  {
    // create stack, initiate it and push the root there
    stack_bst_t stack;
    stack_bst_init(&stack);
    stack_bst_push(&stack, *tree);
    // create helpful pointer for the node we will delete
    bst_node_t *tmp;
    // go until stack is empty
    while(stack_bst_empty(&stack) == false)
    {
      // get the node from the stack
      tmp = stack_bst_pop(&stack);
      // if it is not NULL then push to the stack his subtrees and free the node
      if(tmp != NULL)
      {
        stack_bst_push(&stack, tmp->left);
        stack_bst_push(&stack, tmp->right);
        free(tmp);
      }
    }
    // set the pointer on the tree to NULL
    *tree = NULL;
  }
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  // go until pointer to the node is NULL
  while (tree != NULL)
  {
    // print the node, push it to the stack, go to the left subtree
    bst_print_node(tree);
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  // create and initiate the stack
  stack_bst_t stack;
  stack_bst_init(&stack);
  // fill the stack with the nodes
  bst_leftmost_preorder(tree, &stack);
  // go until stack is empty
  while (!stack_bst_empty(&stack))
  {
    // get the node from the top of the stack
    tree = stack_bst_top(&stack);
    // delete it from the stack
    stack_bst_pop(&stack);
    // fill the stack with nodes of the right subtree
    bst_leftmost_preorder(tree->right, &stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  // go until the pointer on the node is NULL
  while (tree != NULL)
  {
    // push the node to the stack
    stack_bst_push(to_visit, tree);
    // go to the left subtree
    tree = tree->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  // create and initiate the stack
  stack_bst_t stack;
  stack_bst_init(&stack);
  // fill the stack with the nodes
  bst_leftmost_inorder(tree, &stack);
  // go until stack is empty
  while (!stack_bst_empty(&stack))
  {
    // get the node from the top of the stack
    tree = stack_bst_top(&stack);
    // delete it from the stack
    stack_bst_pop(&stack);
    // print the node
    bst_print_node(tree);
    // fill the stack with the nodes of the right subtree
    bst_leftmost_inorder(tree->right, &stack);
  }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
  // go until the pointer on the node is NULL
  while (tree != NULL)
  {
    // push the node to the stack
    stack_bst_push(to_visit, tree);
    // push true to the stack because it was first visit
    stack_bool_push(first_visit, true);
    // go to the left subtree
    tree = tree->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  // create helpful bool to decide if we need to print the node
  bool from_left;
  // create and initiate the stacks
  stack_bst_t stack_bst;
  stack_bool_t stack_bool;
  stack_bst_init(&stack_bst);
  stack_bool_init(&stack_bool);
  // fill the stack with the values
  bst_leftmost_postorder(tree, &stack_bst, &stack_bool);
  // go until stack is empty
  while (!stack_bst_empty(&stack_bst))
  {
    // get the values from the top of the stack
    tree = stack_bst_top(&stack_bst);
    from_left = stack_bool_top(&stack_bool);
    // delete the value from the top of the bool stack
    stack_bool_pop(&stack_bool);
    // if it was our first visit
    // push false to the bool stack because
    // it was our second time visiting this node
    // fill the node stack with the values of right subtree
    if (from_left)
    {
      stack_bool_push(&stack_bool, false);
      bst_leftmost_postorder(tree->right, &stack_bst, &stack_bool);
    }
    // if it was our second time visiting the node
    // pop the node and print it
    else
    {
      stack_bst_pop(&stack_bst);
      bst_print_node(tree);
    }
  }
}
