
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
    // Set all the pointers to NULL
    list->firstElement = NULL;
    list->lastElement = NULL;
    list->activeElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
    // Pointer to store a pointer on next element of the list
    DLLElementPtr temp = list->firstElement;
    // Delete elements until the list is empty
    while (list->firstElement != NULL)
    {
        // Set pointer firstElement to the next element
        list->firstElement = list->firstElement->nextElement;
        // Delete previous element
        free (temp);
        // Set variable to the next element
        temp = list->firstElement;
    }
    list->firstElement = NULL;
    list->lastElement = NULL;
    list->activeElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
    // Allocation of memory for the new element
    DLLElementPtr newFirst = (DLLElementPtr) malloc(sizeof(struct DLLElement));
    // Check if allocation was successful
    if (newFirst == NULL)
        DLL_Error();
    else
    {
        // Set up data of the new element
        newFirst->data = data;
        // Set right pointer of the new element to the old first element
        newFirst->nextElement = list->firstElement;
        // Set left pointer of the new element to NULL
        newFirst->previousElement = NULL;
        // If list is not empty
        if (list->firstElement != NULL)
            // Set left pointer of the old first element to the new element
            list->firstElement->previousElement = newFirst;
        // List is empty
        else
            // Set pointer lastElement to new element
            list->lastElement = newFirst;
        // Set pointer firstElement to the new first element
        list->firstElement = newFirst;
    }
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
    // Allocation of memory for the new element
    DLLElementPtr newLast = (DLLElementPtr) malloc(sizeof(struct DLLElement));
    // Check if allocation was successful
    if (newLast == NULL)
        DLL_Error();
    else
    {
        // Set up data of the new element
        newLast->data = data;
        // Set right pointer of the new element to NULL
        newLast->nextElement = NULL;
        // Set left pointer of the new element to the old last element
        newLast->previousElement = list->lastElement;
        // If list is not empty
        if (list->firstElement != NULL)
            // Set right pointer of the old last element to the new element
            list->lastElement->nextElement = newLast;
            // List is empty
        else
            // Set pointer firstElement to new element
            list->firstElement = newLast;
        // Set pointer lastElement to the new last element
        list->lastElement = newLast;
    }
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
    // Set the pointer activeElement to the first element of the list
    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
    // Set the pointer activeElement to the last element of the list
    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
    // Check if list is not empty
    if (list->firstElement == NULL)
        // List is empty so we call DLL_Error()
        DLL_Error();
    else
        // List is not empty then set the pointer *dataPtr to the data of the first element
        *dataPtr = list->firstElement->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
    // Check if list is not empty
    if (list->firstElement == NULL)
        // List is empty so we call DLL_Error()
        DLL_Error();
    else
        // List is not empty then set the pointer *dataPtr to the data of the last element
        *dataPtr = list->lastElement->data;
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
    // Define helpful variable temp
    DLLElementPtr temp;
    // Check if list is not empty
    if (list->firstElement != NULL) {
        // Set temp to the firstElement
        temp = list->firstElement;
        // If the first element is the active one at the same time then list becomes inactive
        if (list->firstElement == list->activeElement)
            list->activeElement = NULL;
        // If there is only one element in the list then set pointers firstElement and lastElement to NULL
        if (list->firstElement == list->lastElement)
            list->firstElement = list->lastElement = NULL;
        // There is more than one element in the list
        else
        {
            // Pointer firstElement will point on the next one after current first
            list->firstElement = list->firstElement->nextElement;
            // Set pointer on the previous element of the firstElement to NULL
            list->firstElement->previousElement = NULL;
        }
        // Free space
        free(temp);
    }
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
    // Define helpful variable
    DLLElementPtr temp;
    // Check if list is not empty
    if (list->firstElement != NULL)
    {
        // Set temp to the lastElement
        temp = list->lastElement;
        // If the last element is the active one at the same time then list becomes inactive
        if (list->lastElement == list->activeElement)
            list->activeElement = NULL;
        // If there is only one element in the list then set pointers firstElement and lastElement to NULL
        if (list->lastElement == list->firstElement)
            list->lastElement = list->firstElement = NULL;
        // There is more than one element in the list
        else
        {
            // Pointer lastElement will point on the previous one before current last
            list->lastElement = list->lastElement->previousElement;
            // Set pointer on the next element of the firstElement to NULL
            list->lastElement->nextElement = NULL;
        }
        // Free space
        free(temp);
    }
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
    // Check if list is not empty
    if (list->activeElement != NULL)
    {
        // Check if the active element is not the last one in the list
        if (list->activeElement->nextElement != NULL)
        {
            // Define helpful pointer and there the element we want to delete
            DLLElementPtr temp = list->activeElement->nextElement;
            // Set the pointer on the next element of the active element to the one after element we need to delete
            list->activeElement->nextElement = temp->nextElement;
            // Check if the element we want to delete is the last element
            if (temp == list->lastElement)
                // If yes then active element is the last element in the list
                list->lastElement = list->activeElement;
            else
                // If not then pointer on the previous element of the element after deleted one set to active element
                temp->nextElement->previousElement = list->activeElement;
            // Free space
            free(temp);
        }
    }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
    // Check if list is not empty
    if (list->activeElement != NULL)
    {
        // Check if the active element is not the first one in the list
        if (list->activeElement->previousElement != NULL)
        {
            // Define helpful pointer and store there the element we want to delete
            DLLElementPtr temp = list->activeElement->previousElement;
            // Set the pointer on the previous element of the active element to the one before element we need to delete
            list->activeElement->previousElement = temp->previousElement;
            // Check if the element we want to delete is the first element
            if (temp == list->firstElement)
                // Active element becomes first element
                list->firstElement = list->activeElement;
            else
                // Change the pointer on next element of the element before
                // the element we want to delete to the active element
                temp->previousElement->nextElement = list->activeElement;
            // Free the space
            free(temp);
        }
    }
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
    // Check if list is active
    if (list->activeElement != NULL)
    {
        // Allocation of space for the new element
        DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement));
        // Check if allocation was successful
        if (newElement == NULL)
            DLL_Error();
        // Set up data of the new element
        newElement->data = data;
        // Set up the pointer on the next element of the new element to the element after active one
        newElement->nextElement = list->activeElement->nextElement;
        // Set up the pointer on the previous element of the new element to the active element
        newElement->previousElement = list->activeElement;
        // Set up the pointer on the next element of the active element to the new element
        list->activeElement->nextElement = newElement;
        // If active element is the last one in the list
        if (list->activeElement == list->lastElement)
            // Set up the pointer on the last element to the new element
            list->lastElement = newElement;
        else
            // Set up the pointer on the previous element of the element after new element to the new element
            newElement->nextElement->previousElement = newElement;
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
    // Check if list is active
    if (list->activeElement != NULL)
    {
        // Allocation of the memory for the new element
        DLLElementPtr newElement = (DLLElementPtr) malloc(sizeof(struct DLLElement));
        // Check if allocation was successful
        if (newElement == NULL)
            DLL_Error();
        // Set up data of the new element
        newElement->data = data;
        // Set up the pointer on the next element of the new element to the active element
        newElement->nextElement = list->activeElement;
        // Set up the pointer on the previous element of the new element to the element before active element
        newElement->previousElement = list->activeElement->previousElement;
        // Set up the pointer on the previous element of the active element to the new element
        list->activeElement->previousElement = newElement;
        // Check if active element is the first element
        if (list->activeElement == list->firstElement)
            // Set up the pointer on the first element to the new element
            list->firstElement = newElement;
        else
            // Set up the pointer on the next element of the element before before new element to the new element
            newElement->previousElement->nextElement = newElement;
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
    // Check if the list is active
    if (list->activeElement == NULL)
        DLL_Error();
    else
        // Set the pointer dataPtr on the data of the active element
        *dataPtr = list->activeElement->data;
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
    // Check if the list is active
    if (list->activeElement != NULL)
        // Set the data  of the active element to the given value
        list->activeElement->data = data;
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
    // Check if the list is active
    if (list->activeElement != NULL)
        // Set up the pointer active element to the next element
        list->activeElement = list->activeElement->nextElement;
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
    // Check if the list is active
    if (list->activeElement != NULL)
        // Set up the pointer active element to the previous element
        list->activeElement = list->activeElement->previousElement;
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
    // Return 0 if list is inactive
    return (list->activeElement != NULL);
}

/* Konec c206.c */
