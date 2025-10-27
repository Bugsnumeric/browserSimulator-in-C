# browserSimulator-in-C
Basic browser simulation implemented in C.
Functions & Commands:
1) Initialize default values for:

Page – sets the default page for a new tab

Tab – default ID = 0

Browser – initializes the sentinel node

2) Push & Pop

Standard stack operations.
Push: adds an element to the top of the stack.
Pop: removes and deallocates the last added element.

3) NEW_TAB

If only the sentinel exists, the first created tab is linked directly to it and becomes the current tab.
Otherwise, a new tab is added at the end of the circular list, updating links so that the sentinel points to it.
The newly created tab becomes the current one.

4) PRINT

Displays the ID of the current tab and then iterates circularly through the tab list using a do-while loop.
Skips the sentinel when encountered and stops when reaching the current tab again.

5) CLOSE

Removes the current tab from the list, reconnecting the previous and next tabs.
The new current tab becomes the one before the closed tab.

6) OPEN

Traverses the tab list (skipping the sentinel) to find a tab with a specific ID.
If found, it becomes the current tab; otherwise, an error is reported.

7) NEXT

Moves to the next tab in the list.
If the next element is the sentinel, it skips it and moves forward.

8) PREV

Similar to NEXT, but navigates to the previous tab instead.

9) PAGE

Searches for a page by name/ID.
If not found → error.
Pushes the current page onto the backward stack, clears the forward stack, and sets the new current page.

10) PRINT_HISTORY

Displays browsing history.
The forward stack is reversed before printing (to show elements from base → top), while the backward stack is already ordered correctly.

11) BACKWARD / FORWARD

Simulate browser navigation.
The current page is pushed to one stack while popping the corresponding page from the opposite stack.
The old page is freed and replaced with the one retrieved from the stack.

12) Free functions

Deallocate all dynamically allocated memory for each structure, iteratively.
