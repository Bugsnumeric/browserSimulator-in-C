#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct page {
    int id;
    char url[50];
    char *description;
} Page, *TPage;

typedef struct celst {
    TPage page;
    struct celst *next;
} TCelulaStiva, *TStack;

typedef struct tab {
    int id;
    TPage currentPage;
    TStack backwardStack;
    TStack forwardStack;
} Tab, *TTab;

typedef struct TTabCel{
    TTab tab;
    struct TTabCel *prev;
    struct TTabCel *next;
} TTabCel;

typedef struct brwoser {
    TTab current;
    TTabCel *tabsList;
} Browser, *TBrowser;

TPage initPage();
TTab initTab();
TBrowser initBrowser();
void Push(TStack* vf, TPage x);
TPage Pop(TStack* vf);
void print_function(TBrowser browser, FILE* output);
void newTab(TBrowser browser, int *incrementTab);
void close_function(TBrowser browser, FILE* output);
void open_function(TBrowser browser, int id, FILE* output);
void next_function(TBrowser browser);
void prev_function(TBrowser browser);
void page_function(TBrowser browser, Page* pages, int nrPages, int pageID, FILE* output);
void printHistory_function(TBrowser browser, int id, FILE* output);
void backward_function(TBrowser browser, FILE* output);
void forward_function(TBrowser browser, FILE* output);
void freePage(TPage page);
void freeTab(TTab tab);
void freeStack(TStack *stack);
void freeTabsList(TTabCel* tabsList);
void freeBrowser(TBrowser browser);
