
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

int solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
    // Help variable to store chars from the top of the stack
    char temp;
    // Cycle until stack is empty
    while (!Stack_IsEmpty(stack))
    {
        // Store top element to temp variable
        Stack_Top(stack, &temp);
        // Pop the top element of the stack
        Stack_Pop(stack);
        // Found the left bracket and stop
        if (temp == '(')
            break;
        else
        {
            // Take top stack element and put it at the end of the output
            postfixExpression[*postfixExpressionLength] = temp;
            // Increase output length
            (*postfixExpressionLength)++;
        }
    }
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
    // If stack is empty store operator on the top
    if (Stack_IsEmpty(stack))
        Stack_Push(stack, c);
    else
    {
        // Variable to store an element on the top and detect operator
        char temp;
        // Store the element from the top
        Stack_Top(stack, &temp);
        // If character is left bracket then store it on the top
        if (temp == '(')
            Stack_Push(stack, c);
        // If character is operator with higher priority then operator
        // on the top of the stack then store it on the top
        else if ( (c == '*' || c == '/') && (temp == '+' || temp == '-') )
            Stack_Push(stack, c);
        else
        {
            // Store operator from the top on the stack to output
            postfixExpression[*postfixExpressionLength] = temp;
            // Increase output length
            (*postfixExpressionLength)++;
            // Delete the operator from the stack
            Stack_Pop(stack);
            // Call doOperation in recursive way
            doOperation(stack, c, postfixExpression, postfixExpressionLength);
        }
    }
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression Znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns Znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
    // Allocation of memory for stack for operators
    Stack *stack = (Stack *) malloc(sizeof(Stack) * STACK_SIZE);
    // If allocation went wrong return NULL
    if (stack == NULL)
        return NULL;
    // Allocate memory for output expression
    char *postfixExpression = (char *) malloc(sizeof(char) * MAX_LEN);
    // If allocation went wrong return NULL
    if (postfixExpression == NULL)
    {
        // Free space used for the stack
        free(stack);
        return NULL;
    }

    // Variable which stores current length of output string
    unsigned postfixExpressionLength = 0;
    // Initialization of stack
    Stack_Init(stack);

    // Go through all the characters one by one
    for (int i = 0; infixExpression[i] != '\0'; i++)
    {
        // If the character is '(' store it on the top of the stack
        if (infixExpression[i] == '(')
        {
            Stack_Push(stack, infixExpression[i]);
        }
        // If the character is ')' call function untilLeftPar
        else if (infixExpression[i] == ')')
        {
            untilLeftPar(stack, postfixExpression, &postfixExpressionLength);
        }
        // If character is operator call function doOperation
        else if ((infixExpression[i] == '+') ||
                 (infixExpression[i] == '-') ||
                 (infixExpression[i] == '*') ||
                 (infixExpression[i] == '/'))
        {
            doOperation(stack, infixExpression[i], postfixExpression, &postfixExpressionLength);
        }
        // If character is a number or letter add it to the output string and increase the length
        else if ((infixExpression[i] >= 'a' && infixExpression[i] <= 'z') ||
                 (infixExpression[i] >= 'A' && infixExpression[i] <= 'Z') ||
                 (infixExpression[i] >= '0' && infixExpression[i] <= '9'))
        {
            postfixExpression[postfixExpressionLength] = infixExpression[i];
            postfixExpressionLength++;
        }
        // If the character is '=' we are at the end
        else if (infixExpression[i] == '=')
        {
            // Add all the elements from the stack to the output string
            while (!Stack_IsEmpty(stack))
            {
                // Helpful variable to store an element from the top
                char temp;
                Stack_Top(stack, &temp);
                Stack_Pop(stack);
                // Add the element to the output
                postfixExpression[postfixExpressionLength] = temp;
                postfixExpressionLength++;
            }
            // When stack is empty add '=' and increase the length
            postfixExpression[postfixExpressionLength] = '=';
            postfixExpressionLength++;
        }
    }
    // Free the space used for the stack
    free(stack);
    // Add '\0' at the end of the output string
    postfixExpression[postfixExpressionLength] = '\0';
    // Return the output string
    return postfixExpression;
}

/* Konec c204.c */
