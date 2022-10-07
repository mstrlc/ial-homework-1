/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
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
** uvedeno něco jiného.
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
void DLL_Error()
{
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
void DLL_Init( DLList *list )
{
	// Initialize the list to empty
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list )
{
	// Go through all elements in the list in order, starting with firstElement
	while (list->firstElement != NULL)
	{
		// Store deleted element so we can access next one after its deletion
		DLLElementPtr toRemove = list->firstElement;
		list->firstElement = toRemove->nextElement;
		free(toRemove);
	}
	
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data )
{
	// Allocate needed space for the new element
	DLLElementPtr newElement = (DLLElementPtr)malloc(sizeof(struct DLLElement));
	
	if (newElement == NULL)
	{
		DLL_Error();
		return;
	}

	newElement->data = data;
	newElement->nextElement = list->firstElement;
	newElement->previousElement = NULL;

	if (list->firstElement != NULL)
	{
		list->firstElement->previousElement = newElement;
	}
	else
	{
		list->lastElement = newElement;
	}
	
	list->firstElement = newElement;
}

/**
 * V případě, že není dostatek pa * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
měti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data )
{
	// Allocate needed space for the new element
	DLLElementPtr newElement = (DLLElementPtr)malloc(sizeof(struct DLLElement));
	
	if (newElement == NULL)
	{
		DLL_Error();
		return;
	}

	newElement->data = data;
	newElement->nextElement = NULL;
	newElement->previousElement = list->lastElement;

	if (list->lastElement != NULL)
	{
		list->lastElement->nextElement = newElement;
	}
	else
	{
		list->firstElement = newElement;
	}

	list->lastElement = newElement;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list )
{
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list )
{
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr )
{
	// If first element is not NULL, the list is not empty
	if (list->firstElement != NULL)
	{
		*dataPtr = list->firstElement->data;
	}
	else
	{
		DLL_Error();
		return;
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr )
{
	// If last element is not NULL, the list is not empty
	if (list->lastElement != NULL)
	{
		*dataPtr = list->lastElement->data;
	}
	else
	{
		DLL_Error();
		return;
	}
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list )
{
	DLLElementPtr element;

	if (list->firstElement != NULL)
	{
		element = list->firstElement;
		
		if (list->activeElement == list->firstElement)
		{
			list->activeElement = NULL;
		}
		if(list->firstElement == list->lastElement)
		{
			list->firstElement = NULL;
			list->lastElement = NULL;
		}
		else
		{
			list->firstElement = list->firstElement->nextElement;
			list->firstElement->previousElement = NULL;
		}
		free(element);
	}
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list )
{
	DLLElementPtr toRemove;

	if (list->lastElement != NULL)
	{
		toRemove = list->lastElement;
		
		if (list->activeElement == list->lastElement)
		{
			list->activeElement = NULL;
		}

		if(list->firstElement == list->lastElement)
		{
			list->firstElement = NULL;
			list->lastElement = NULL;
		}
		else
		{
			list->lastElement = toRemove->previousElement;
			list->lastElement->nextElement = NULL;
		}

		free(toRemove);
	}
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list )
{
	if (list->activeElement != NULL && list->activeElement != list->lastElement)
	{
		if (list->activeElement->nextElement != NULL)
		{
			DLLElementPtr toRemove = list->activeElement->nextElement;
			list->activeElement->nextElement = toRemove->nextElement;
			
			if (toRemove == list->lastElement)
			{
				list->lastElement = toRemove->previousElement;
			}
			else
			{
				toRemove->previousElement->nextElement = toRemove->nextElement;
			}
			free(toRemove);
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
void DLL_DeleteBefore( DLList *list ) 
{
	if (list->activeElement != NULL)
	{
		if (list->activeElement->previousElement != NULL)
		{
			DLLElementPtr toRemove = list->activeElement->previousElement;
			list->activeElement->previousElement = toRemove->previousElement;
			
			if (toRemove == list->firstElement)
			{
				list->firstElement = list->activeElement;
			}
			else
			{
				toRemove->previousElement->nextElement = list->activeElement;
			}
			free(toRemove);
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
void DLL_InsertAfter( DLList *list, int data )
{
	if (list->activeElement != NULL)
	{
		DLLElementPtr newElement = (DLLElementPtr)malloc(sizeof(struct DLLElement));
		
		if (newElement == NULL)
		{
			DLL_Error();
			return;
		}

		newElement->data = data;
		newElement->nextElement = list->activeElement->nextElement;
		newElement->previousElement = list->activeElement;
		list->activeElement->nextElement = newElement;

		if (list->activeElement == list->lastElement)
		{
			list->lastElement = newElement;
		}
		else
		{
			newElement->nextElement->previousElement = newElement;
		}
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
void DLL_InsertBefore( DLList *list, int data )
{
	if (list->activeElement != NULL)
	{
		DLLElementPtr newElement = (DLLElementPtr)malloc(sizeof(struct DLLElement));
		
		if (newElement == NULL)
		{
			DLL_Error();
			return;
		}

		newElement->data = data;
		newElement->nextElement = list->activeElement;
		newElement->previousElement = list->activeElement->previousElement;

		list->activeElement->previousElement = newElement;

		if (list->activeElement == list->firstElement)
		{
			list->firstElement = newElement;
		}
		else
		{
			newElement->previousElement->nextElement = newElement;
		}
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr )
{
	if (list->activeElement != NULL)
	{
		*dataPtr = list->activeElement->data;
	}
	else
	{
		DLL_Error();
		return;
	}
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data )
{
	if (list->activeElement != NULL)
	{
		list->activeElement->data = data;
	}
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list )
{
	if (list->activeElement != NULL)
	{
		list->activeElement = list->activeElement->nextElement;
	}
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list )
{
	if (list->activeElement != NULL)
	{
		list->activeElement = list->activeElement->previousElement;
	}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list )
{
	return (list->activeElement != NULL); 
}

/* Konec c206.c */
