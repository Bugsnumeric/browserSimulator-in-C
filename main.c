#include "functii.h"

int main() {
	FILE *input = fopen("tema1.in", "r");
	FILE *output = fopen("tema1.out", "w");

	int nrPages = 0;
	fscanf(input, "%d", &nrPages);
	fgetc(input);

	Page pages[50];
	TBrowser browser = initBrowser();

	if (browser == NULL) {
		return 1;
	}

	int incrementTab = -1;
	newTab(browser, &incrementTab);
	char description[50];

	for (int i = 0; i < nrPages; i++) {
		fscanf(input, "%d", &pages[i].id);
		fgetc(input);

		fgets(pages[i].url, 50, input);
		pages[i].url[strcspn(pages[i].url, "\n")] = 0;

		fgets(description, 50, input);
		description[strcspn(description, "\n")] = 0;
		pages[i].description = strdup(description);
	}

	int nrCommands = 0;
	fscanf(input, "%d", &nrCommands);
	fgetc(input);

	char line[20];

	for (int i = 0; i < nrCommands; i++) {
		fgets(line, sizeof(line), input);
		line[strcspn(line, "\n")] = 0;

		if (strcmp(line, "NEW_TAB") == 0) {
			newTab(browser, &incrementTab);
		} else if (strcmp(line, "CLOSE") == 0) {
			close_function(browser, output);
		} else if (strncmp(line, "OPEN", 4) == 0) {
			int ID_open;
			sscanf(line, "OPEN %d", &ID_open);
			open_function(browser, ID_open, output);
		} else if (strcmp(line, "NEXT") == 0) {
			next_function(browser);
		} else if (strcmp(line, "PREV") == 0) {
			prev_function(browser);
		} else if (strncmp(line, "PAGE", 4) == 0) {
			int ID_page;
			sscanf(line, "PAGE %d", &ID_page);
			page_function(browser, pages, nrPages, ID_page, output);
		} else if (strcmp(line, "BACKWARD") == 0) {
			backward_function(browser, output);
		} else if (strncmp(line, "FORWARD", 7) == 0) {
			forward_function(browser, output);
		} else if (strcmp(line, "PRINT") == 0) {
			print_function(browser, output);
		} else if (strncmp(line, "PRINT_HISTORY", 13) == 0) {
			int ID_printHistory;
			sscanf(line, "PRINT_HISTORY %d", &ID_printHistory);
			printHistory_function(browser, ID_printHistory, output);
		}
	}

	for (int i = 0; i < nrPages; i++) {
		free(pages[i].description);
	}

	freeBrowser(browser);
	fclose(input);
	fclose(output);
	return 0;
}
