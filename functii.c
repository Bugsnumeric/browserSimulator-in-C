#include "functii.h"

TPage initPage() {
    TPage newPage = (TPage)malloc(sizeof(Page));

    if (newPage == NULL) {
        return NULL;
    }

    newPage->id = 0;
    strcpy(newPage->url, "https://acs.pub.ro/");
    newPage->description = strdup("Computer Science");

    return newPage;
}

TTab initTab() {
    TTab tab = (TTab)malloc(sizeof(Tab));

    if (!tab) {
        return NULL;
    }

    tab->id = 0;
    tab->currentPage = initPage();
    tab->backwardStack = NULL;
    tab->forwardStack = NULL;

    return tab;
}

TBrowser initBrowser() {
    TBrowser browser = (TBrowser)malloc(sizeof(Browser));

    if (!browser) {
        return NULL;
    }

    TTabCel* santinela = (TTabCel*)malloc(sizeof(TTabCel));
    if (!santinela) {
        free(browser);
        return NULL;
    }

    //asta este `SANTINELA`
    santinela->tab = NULL;
    santinela->prev = santinela;
    santinela->next = santinela;

    browser->tabsList = santinela;
    browser->current = NULL;

    return browser;
}

void Push(TStack* vf, TPage x) {
    if (!x) {
        return;
    }

    TStack aux = (TStack)malloc(sizeof(TCelulaStiva));

    if (!aux) {
        return;
    }

    TPage newPage = (TPage)malloc(sizeof(Page));

    if (!newPage) {
        free(aux);
        return;
    }

    newPage->id = x->id;
    strcpy(newPage->url, x->url);
    newPage->description = strdup(x->description);

    aux->page = newPage;
    aux->next = *vf;
    *vf = aux;

    return;
}

TPage Pop(TStack *vf) {
    if (!vf || !(*vf)) {
        return NULL;
    }

    TStack top = *vf;
    TPage page = top->page;
    *vf = top->next;
    free(top);

    return page;
}

void newTab(TBrowser browser, int *incrementTab) {
    if (!browser) {
        return;
    }

    //verific daca am doar santinela in lista
    if (browser->tabsList->next == browser->tabsList) {
        TTabCel* aloc = (TTabCel*)malloc(sizeof(TTabCel));

        if (!aloc) {
            return;
        }

        TTab newTab = initTab();
        newTab->id = ++(*incrementTab);
        aloc->tab = newTab;
        aloc->next = browser->tabsList;
        aloc->prev = browser->tabsList;

        browser->tabsList->next = aloc;
        browser->tabsList->prev = aloc;
        browser->current = newTab;
        return;
    }

    TTab newTab = initTab();
    newTab->id = ++(*incrementTab);

    TTabCel *newCell = (TTabCel *)malloc(sizeof(TTabCel));

    if (!newCell) {
        return;
    }

    newCell->tab = newTab;
    // introduc in lista noul tab creat si il leg de santinela
    newCell->prev = browser->tabsList->prev;
    newCell->next = browser->tabsList;
    //refac legaturile
    browser->tabsList->prev->next = newCell;
    browser->tabsList->prev = newCell;
    browser->current = newCell->tab;
}

void print_function(TBrowser browser, FILE* output) {
    if (!browser || !browser->current || !browser->tabsList) {
        return;
    }

    fprintf(output, "%d ", browser->current->id);
    TTabCel* p = browser->tabsList->next;

    while (p->tab != browser->current) {
        p = p->next;
    }

    p = p->next;

    do {
        if (p == browser->tabsList) {
            p = p->next;
            continue;
        }

        fprintf(output, "%d ", p->tab->id);
        p = p->next;
    } while (p->tab != browser->current);

    fprintf(output, "\n%s\n", browser->current->currentPage->description);
}

void close_function(TBrowser browser, FILE* output) {
    if (!browser || !browser->tabsList || !browser->current) {
        return;
    }

    TTabCel* currentCell = browser->tabsList;

    // Gaseste celula cu tabul curent
    while (currentCell->tab != browser->current) {
        currentCell = currentCell->next;
    }

    if (currentCell->tab->id == 0) {
        fprintf(output, "%s\n", "403 Forbidden");
        return;
    }

    //se leaga componentele dupa ce am scos tabul
    currentCell->prev->next = currentCell->next;
    currentCell->next->prev = currentCell->prev;
    browser->current = currentCell->prev->tab;

    freeTab(currentCell->tab);
    free(currentCell);
}

void open_function(TBrowser browser, int id, FILE* output) {
    if (!browser || !browser->tabsList) {
        return;
    }

    TTabCel* p = browser->tabsList->next;

    do {
        if (p->tab->id == id) {
            browser->current = p->tab;
            return;
        }

        p = p->next;
    } while (p != browser->tabsList);

    fprintf(output, "%s\n", "403 Forbidden");
}

void next_function(TBrowser browser) {
    if (!browser || !browser->tabsList || !browser->current) {
        return;
    }

    TTabCel* p = browser->tabsList->next;

    while (p != browser->tabsList && p->tab != browser->current) {
        p = p->next;
    }

    if (p->next == browser->tabsList) {
        p = p->next->next;
        browser->current = p->tab;
    } else {
        browser->current = p->next->tab;
    }
}

void prev_function(TBrowser browser) {
    if (!browser || !browser->tabsList || !browser->current) {
        return;
    }

    TTabCel* p = browser->tabsList->next;

    while (p != browser->tabsList && p->tab != browser->current) {
        p = p->next;
    }

    if (p->prev == browser->tabsList) {
        p = p->prev->prev;
        browser->current = p->tab;
    } else {
        browser->current = p->prev->tab;
    }
}

void page_function(TBrowser browser, Page* pages, int nrPages, int pageID, FILE* output) {
    if (!browser || !browser->current) {
        return;
    }

    // caut pagina cu pageID in vector
    TPage target = NULL;
    for (int i = 0; i < nrPages; i++) {
        if (pages[i].id == pageID) {
            target = &pages[i];
            break;
        }
    }

    if (!target) {
        fprintf(output, "%s\n", "403 Forbidden");
        return;
    }

    // pagina curenta se introduce in stiva backward
    Push(&browser->current->backwardStack, browser->current->currentPage);

    // se sterge stiva forward
    while (browser->current->forwardStack != NULL) {
        TStack temp = browser->current->forwardStack;
        browser->current->forwardStack = browser->current->forwardStack->next;
        free(temp);
    }

    // se introduce ca pagina curenta
    TPage newPage = (TPage)malloc(sizeof(Page));

    if (!newPage) {
        return;
    }

    newPage->id = target->id;
    strcpy(newPage->url, target->url);
    newPage->description = strdup(target->description);

    //free la vechiul current_page
    freePage(browser->current->currentPage);
    browser->current->currentPage = newPage;
}

void printHistory_function(TBrowser browser, int id, FILE* output) {
    if (!browser || !browser->tabsList) {
        return;
    }

    TTabCel* p = browser->tabsList->next;

    do {
        if (p->tab->id == id) {
            // inversam ordinea introducerii, detalii README
            TStack reversedStack = NULL;
            TStack iterator = p->tab->forwardStack;

            while (iterator) {
                Push(&reversedStack, iterator->page);
                iterator = iterator->next;
            }

            while (reversedStack) {
                fprintf(output, "%s\n", reversedStack->page->url);
                reversedStack = reversedStack->next;
            }

            fprintf(output, "%s\n", p->tab->currentPage->url);

            // nu este necesar sa inversam stiva backward pentru afisare
            iterator = p->tab->backwardStack;
            while (iterator) {
                fprintf(output, "%s\n", iterator->page->url);
                iterator = iterator->next;
            }
            return;
        }

        p = p->next;
    } while (p != browser->tabsList);

    fprintf(output, "%s\n", "403 Forbidden");
}

void backward_function(TBrowser browser, FILE* output) {
    if (!browser || !browser->current) {
        return;
    }

    TTab tab = browser->current;

    if (!tab->backwardStack) {
        fprintf(output, "%s\n", "403 Forbidden");
        return;
    }

    Push(&tab->forwardStack, tab->currentPage);
    TPage prevPage = Pop(&tab->backwardStack);

    if (!prevPage) {
        return;
    }

    freePage(tab->currentPage);
    tab->currentPage = prevPage;
}

void forward_function(TBrowser browser, FILE* output) {
    if (!browser || !browser->current) {
        return;
    }

    TTab tab = browser->current;

    if (!tab->forwardStack) {
        fprintf(output, "%s\n", "403 Forbidden");
        return;
    }

    Push(&tab->backwardStack, tab->currentPage);
    TPage nextPage = Pop(&tab->forwardStack);

    if (!nextPage) {
        return;
    }

    freePage(tab->currentPage);
    tab->currentPage = nextPage;
}


void freePage(TPage page) {
    if (!page) {
        return;
    }

    free(page->description);
    free(page);
}

void freeStack(TStack* stack) {
    if (!stack) {
        return;
    }

    TStack aux;

    while (*stack) {
        aux = *stack;
        *stack = (*stack)->next;

        if (aux->page) {
            freePage(aux->page);
        }
        free(aux);
    }
}

void freeTab(TTab tab) {
    if (!tab) {
        return;
    }

    freeStack(&tab->backwardStack);
    freeStack(&tab->forwardStack);
    freePage(tab->currentPage);
    free(tab);
}

void freeTabsList(TTabCel* tabsList) {
    if (!tabsList) {
        return;
    }

    TTabCel* p = tabsList;

    do {
        TTabCel* temp = p;
        p = p->next;
        freeTab(temp->tab);
        free(temp);
    } while (p != tabsList);
}

void freeBrowser(TBrowser browser) {
    if (!browser) {
        return;
    }

    freeTabsList(browser->tabsList);
    free(browser);
}
