#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct List
{
    char name[30];
    struct Item *firstitem;
    struct List *prevList;
    struct List *nextList;
} List;
typedef struct Item
{
    char name[30];
    struct Item *previtem;
    struct Item *nextitem;

} Item;

// prototypes
List *readFile(List *);
void Menu(List *);
List *exampleDisplay(List *);
void PrintBoard(List *);
void clearBoard(List **);

void editmenu(List **, int option);
void editname(List **, int option);
void addlist(List **, int option);
void deleteList(List**, int option);

void saveBoard(List *);
void clearInputBuffer();
void clearscreen();
void checkifthere(List** board,int option, void(*function)(List** board, int option) );
// keep track of whether to display example or a board loaded from file
// bool boardloaded = false;

void Menu(List *board)
{
    const char *menuOptions[6] = {"Display Board", "Load Board from File", "Edit List",
                                  "Edit Board", "Save Board to a File", "Quit"};
    printf("Welcome to your Kanaban Board!\n");

    bool boardloaded = false;
    int choice = -1;
    board = exampleDisplay(board); // loading in board:
    while (choice != 6)
    {
        printf("Menu:\n");
        for (int i = 0; i < 6; i++)
        {
            printf(" %d. %s\n", i + 1, menuOptions[i]);
        }
        bool validChoice = false;

        while (!validChoice)
        {
            printf("Enter your choice:");
            if (scanf("%d", &choice) != 1)
            {
                printf("Invalid input\n");
                while (getchar() != '\n')
                    ; // Clear input buffer so invalid input doesn't get processed in the next iteration
                continue;
            }
            if (choice < 1 || choice > 6)
            {
                printf("Invalid choice\n");
                continue; // skips the rest of the code in the current loop and goes to the next iteration
            }
            validChoice = true;
        }
        switch (choice)
        {
        case 1:
            // Display Board
            PrintBoard(board);
            break;
        case 2:
            board = readFile(board);
            // printf("boardname=%s ", board->name);
            boardloaded = true;
            break;
        case 3:
            // Edit List
            editmenu(&board, choice);

            break;
        case 4:
            // Edit Board
            editmenu(&board, choice);
            break;
        case 5:
            saveBoard(board);
            break;
        case 6:
            // Quit
            printf("Goodbye!\n");
            break;
        default:
            printf("Invalid choice\n");
        }
    }
}

int main(void)
{
    // set starting pointer
    List *board = NULL;
    Menu(board);
}
List *readFile(List *board)
{
    // Function body for readFile
    // clears the screen
#ifdef windows
    system("clear");
#else
    system("cls");
#endif

    printf("Enter file name:");
    char filename[20];
    scanf("%s", filename);

    FILE *file;
    if ((file = fopen(filename, "r")) == NULL)
    {
        printf("File could not be opened\n");
    }
    else
    {
        printf("File opened\n");
        if (board != NULL)
        {
            puts("Gets cleared");
            clearBoard(&board);
        }

        char rem;

        // initialize pointers to track and set pointers of lists and items
        List *current_list = NULL;
        Item *current_item = NULL;

        while (!feof(file))
        {
            char word[30];
            fgets(word, sizeof(char[30]), file);
            bool isList = false;

            // get rid of enter character
            int len = (int)strlen(word) - 1;
            rem = word[len];
            word[len] = '\0';

            // check whether current line is a list name
            int last_char_index = len - 1;
            if (word[last_char_index] == ':')
            {
                isList = true;
                word[last_char_index] = '\0';
            }

            // printf("currentItem = %s\n", word);

            if (isList)
            {
                List *newlist = malloc(sizeof(List));
                strcpy(newlist->name, word);

                // reset item pointer
                current_item = NULL;

                // if this is the first list, set starting pointer and new list pointers
                if (board == NULL)
                {
                    board = newlist;
                    current_list = newlist;
                }
                else
                {
                    newlist->prevList = current_list;
                    current_list->nextList = newlist;
                    current_list = newlist;
                }

                // the current line is an item of a list
            }
            else
            {
                // Error checking: if we are on an item, current_list should not be null
                if (current_list == NULL)
                {
                    printf("\nError, checking an item not part of a list\n");
                }

                Item *newitem = malloc(sizeof(Item));
                strcpy(newitem->name, word);

                // The first item of a list
                if (current_item == NULL)
                {
                    current_list->firstitem = newitem;
                    current_item = newitem;
                }
                else
                {
                    newitem->previtem = current_item;
                    current_item->nextitem = newitem;
                    current_item = newitem;
                }
            }
        }

        // The last item has the last character cut off, so add back the saved char
        char rem2[2] = {rem, '\0'};
        strcat(current_item->name, rem2);
        // printf("last_item = %s", current_item->name);
    }
    return board;
}

List *exampleDisplay(List *board)
{
    clearscreen();
    // List 1
    List *List1 = malloc(sizeof(List));
    strcpy(List1->name, "Narchiz");
    Item *Item1_1 = malloc(sizeof(Item));
    strcpy(Item1_1->name, "Airpods");
    Item *Item1_2 = malloc(sizeof(Item));
    strcpy(Item1_2->name, "Mifit");
    // List 2
    List *List2 = malloc(sizeof(List));
    strcpy(List2->name, "Davane");
    Item *Item2_1 = malloc(sizeof(Item));
    strcpy(Item2_1->name, "IpadAir");
    Item2_1->nextitem = NULL;
    Item *Item2_2 = malloc(sizeof(Item));
    strcpy(Item2_2->name, "Clippers");
    // list 3
    List *List3 = malloc(sizeof(List));
    strcpy(List3->name, "Tipra");
    Item *Item3_1 = malloc(sizeof(Item));
    strcpy(Item3_1->name, "Ring Three");
    // list 4
    List *List4 = malloc(sizeof(List));
    strcpy(List4->name, "Darren");
    Item *Item4_1 = malloc(sizeof(Item));
    strcpy(Item4_1->name, "Two Phones");
    //printf("%d", strlen(Item4_1->name));

    //linking items
    List1->firstitem = Item1_1;
    Item1_1->nextitem = Item1_2;
    Item1_2->nextitem = NULL;

    Item2_2->nextitem = NULL;
    List2->firstitem = Item2_1;
    Item2_1->nextitem = Item2_2;
    List3->firstitem = Item3_1;
    Item3_1->nextitem = NULL;

    Item4_1->nextitem = NULL;
    List4->firstitem = Item4_1;

    //linking lists
    List1->nextList = NULL;
    List2->nextList = List1;
    List3->nextList = List2;
    List4->nextList = List3;

    List1->prevList = List2;
    List2->prevList = List3;
    List3->prevList = List4;
    List4->prevList = NULL;

    board = List4;
    // PrintBoard(board);
    return board;
}
void PrintBoard(List *board)
{
    clearscreen();
    List *current = board;
    //uses nested loop to go through all the lists and items and prints them out
    while (current != NULL)
    {
        Item *currentitem = current->firstitem;
        printf("%s \n", current->name);
        while (currentitem != NULL)
        {
            printf("\t%s\n", currentitem->name);
            currentitem = currentitem->nextitem;
        }
        current = current->nextList;
    }
}
void clearBoard(List **board)
{
    // This function clears the entire board by deallocating all the memory

    List *currentList = *board;
    // A while loop is used to iterate over each list of the board

    while (currentList != NULL)
    {
        Item *currentItem = currentList->firstitem;
        while (currentItem != NULL)
        {
            // Within each list, a nested while loop is used to iterate over each item of the list
            Item *temp = currentItem;
            currentItem = currentItem->nextitem;
            free(temp);
        }
        // Once all the items of a list are deallocated, the list itself is also deallocated
        List *tempList = currentList;
        currentList = currentList->nextList;
        free(tempList);
    }
    // The loop continues until all lists of the board are deallocated

    *board = NULL;
    // Finally, the head pointer is set to NULL to indicate an empty board
}
void saveBoard(List *board)
{
    // printf("Save func: first list is %s\n", board->name);
    if (board == NULL)
    {
        printf("There is no board to save to a file.\n");
    }
    char filename[50];
    printf("Please provide a name for the file you'd like the board saved to. Please end the file name with .txt):\n");
    int invalid_file = 1;
    while (invalid_file)
    {
        scanf("%s", filename);
        int len = (int)strlen(filename);
        if (strcmp(filename + len - 4, ".txt") == 0)
        {
            invalid_file = 0;
        }
        else
        {
            printf("That is an invalid file name. Please provide a file name ending in .txt:");
        }
    }

    FILE *boardfile = fopen(filename, "w");
    if (boardfile == NULL)
    {
        printf("Error creating this file\n");
    }

    List *current_list = board;
    Item *current_item = board->firstitem;

    while (current_list != NULL)
    {

        // add semi-colon back to the list name
        strcat(current_list->name, ":");

        fprintf(boardfile, "%s\n", current_list->name);

        while (current_item != NULL)
        {
            printf("Current Item: %s\n", current_item->name);

            // For consistency, only the last item does not have an enter sign at the end
            if (current_item->nextitem == NULL && current_list->nextList == NULL)
            {
                fprintf(boardfile, "%s", current_item->name);
            }
            else
            {
                fprintf(boardfile, "%s\n", current_item->name);
            }

            current_item = current_item->nextitem;
        }
        current_list = current_list->nextList;
        if (current_list != NULL)
        {
            current_item = current_list->firstitem;
        }
    }
    fclose(boardfile);
}

void editmenu(List **board, int option)
{
    clearscreen();
    
    const char *editboardoptions[6] = {"Options:", "1. Edit the name of a list", "2. Add a new list",
                                       "3. Delete a list", "4. Return to main menu", "Enter your option:"};
    
    const char *edititemoptions[6] = {"Options:", "1. Edit the name of a item", "2. Add a new item",
                                      "3. Delete a item", "4. Return to main menu", "Enter your option:"};

    bool boardloaded = false;
    int choice = -1;
    bool status = false;
    char listname[30];
    while (status != true)
    {
        puts("");
        choice = -1;
        while (choice > 4 || choice < 1)
        {
            switch (option) //depending on option, will print out different choices
            {
                case 3:
                    for (int i = 0; i < 6; i++)
                    {
                    printf("\n%s", edititemoptions[i]);
                    }
                    break;
                case 4:
                    for (int i = 0; i < 6; i++)
                    {
                    printf("\n%s", editboardoptions[i]);
                    }
                    break;
                default:
                break;
            } 
            scanf("%d", &choice);
            clearInputBuffer();
        }
        switch (choice)
            {
                case 1:
                    // code for editing the name of a list
                    switch (option)
                    {
                        case 3:
                            checkifthere(board, option, editname);
                            break;
                        case 4:
                            editname(board, option);
                            break;
                        default:
                            break;
                    }
                    break;
                case 2:
                     // code for adding a new list
                    switch (option)
                    {
                        case 3:
                            checkifthere(board, option, addlist);
                            break;
                        case 4:
                            addlist(board, option);
                            break;
                        default:
                            break;
                    }
                    break;
                case 3:
                    // code for deleting a list
                    switch (option)
                    {
                        case 3:
                            checkifthere(board, option, deleteList);
                            break;
                        case 4:
                            deleteList(board, option);
                            break;
                        default:
                            break;
                    }
                    break;
                case 4:
                    clearscreen();
                    status = true;
                    //status to true so while loop is broken
                    break;
                default:
                    puts("Invalid");
                    break;
            }
    }   
    //bool validChoice = false;
        
}

void editname(List **board, int option)
{
    printf("Enter name of the item to edit: ");
    char name[30];
    fgets(name, 30, stdin);
    int len = (int)strlen(name) - 1;
    name[len] = '\0';
    //printf("%d is the len \n", strlen(name));
    List *find = *board;
    //printf("%s name %s board\n", name, *board);
    
    if (option == 4)
    {
        while (find != NULL)
        {
            //if found then changes name of list
            if (strcmp(name, find->name) == 0)
            {
                printf("Enter new name:");
                char newname[30];
                scanf("%s", newname);
                strcpy(find->name, newname);
                return;
            }
            else
            {
                //if not then it goes to next board/list
                find = find->nextList;
            }
        }
        printf("item not found\n");
    }
    else if (option == 3)
    {

        char newname[50];
        List *currentList = *board;
        while (currentList != NULL)
        {
            Item *currentItem = currentList->firstitem;
            while (currentItem != NULL)
            {
                //if name is found then it asks for new name
                if (strcmp(currentItem->name, name) == 0)
                {
                    printf("Enter new name for item '%s' in list '%s': ", currentItem->name, currentList->name);
                    fgets(newname, 50, stdin);
                    int len = strlen(newname) - 1;
                    newname[len] = '\0';
                    strcpy(currentItem->name, newname);
                    return;
                }
                //if not, it goes to the next item
                currentItem = currentItem->nextitem;
            }
            currentList = currentList->nextList;
        }

        printf("Item not found\n");
    }
}
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void clearscreen()
{
#ifdef windows
    system("clear");
#else
    system("cls");
#endif
}

void addlist(List **board, int option)
{
    char name[30];
    // Prompt the user to enter the name of the new list or item based on the specified option
    if (option == 4)
    {
        printf("Enter the name of the new List:");
    }
    else if (option == 3)
    {
        printf("Enter the name of the new item:");
    }
    
    fgets(name, 30, stdin);
    int len = strlen(name) - 1;
    name[len] = '\0';

    if (option == 4)
    {
        // Allocate memory for a new List data type
        List* tempptr = malloc(sizeof(List));
        // Copy the entered name to the name field of the new List data type
        // Set the previous list of the current board to the new List data type

        strcpy(tempptr->name, name);
        // Set the next and previous lists of the new List data type

        (*board)->prevList = tempptr;

        tempptr->nextList = *board;
        tempptr->prevList = NULL;
        // Set the board pointer to point to the new List data type
        *board = tempptr;
    }
    else if(option == 3)
    {
        // Allocate memory for a new Item data type
        Item* temppr = malloc(sizeof(Item));
        strcpy(temppr->name, name);
        // If the firstitem field of the current board is empty, set the new Item data type to be the firstitem

        if ((*board)->firstitem == NULL) 
        {
            (*board)->firstitem = temppr;
            temppr->previtem = NULL;
            temppr->nextitem = NULL;
        } else 
        {
            // If the firstitem field of the current board is not empty, 
            //set the new Item data type to be the firstitem and update the pointers 
            //of the old firstitem

            Item* temppr2 = (*board)->firstitem;

            temppr2->previtem = temppr;
            temppr2->nextitem = NULL;

            temppr->nextitem = (*board)->firstitem;
            temppr->previtem = NULL;

            (*board)->firstitem = temppr;
        }
    }
    clearscreen();
}

void deleteList(List** board, int option) 
{
    List* curr_list = *board; //for deleting list
    Item *curr_item = (*board)->firstitem; //for deleting item

    char name[30];
    int len;
    
   switch (option)
{
    // case for deleting a list
    case 4:
        printf("Please enter the List you would like to delete:");
        fgets(name, 30, stdin);
        len = strlen(name) - 1;
        name[len] = '\0';

        // find the list with the given name
        while (curr_list != NULL && strcmp(curr_list->name, name) != 0) {
            curr_list = curr_list->nextList;
        }
        // if the list was found, delete it
        if (curr_list != NULL) 
        {
            if (curr_list->prevList == NULL) 
            {
                // the list to be deleted is the first list
                *board = curr_list->nextList;
            } else {
                // the list to be deleted is not the first list
                curr_list->prevList->nextList = curr_list->nextList;
            }
            if (curr_list->nextList != NULL) 
            {
                // the list to be deleted is the last list
                curr_list->nextList->prevList = curr_list->prevList;
            }
            free(curr_list); // free the memory allocated for the list
            break; // exit the switch statement
        }
        else
        {
            printf("No list found");
        }
        break;

    // case for deleting an item
    case 3:
        printf("Please enter the item you would like to delete:");
        name[30];
        fgets(name, 30, stdin);
        len = strlen(name) - 1;
        name[len] = '\0';

        // find the item with the given name
        while (curr_item != NULL && strcmp(curr_item->name, name) != 0) 
        {
            curr_item = curr_item->nextitem;
        }
        // if the item was found, delete it
        if (curr_item != NULL) 
        {
            if (curr_item->previtem == NULL) 
            {
                // the item to be deleted is the first item
                (*board)->firstitem = curr_item->nextitem;
            } else {
                // the item to be deleted is not the first item
                curr_item->previtem->nextitem = curr_item->nextitem;
            }
            if (curr_item->nextitem != NULL) 
            {
                // the item to be deleted is the last item
                curr_item->nextitem->previtem = curr_item->previtem;
            }
            free(curr_item); // free the memory allocated for the item
            break; // exit the switch statement
        }
        else
        {
            printf("No item found");
        }
        break;

    // default case for an invalid option
    default:
        puts("Invalid");
}
        

}

void checkifthere(List** board, int option, void(*function)(List** board, int option) )
{
    printf("Enter name of the List where the item is located:");
    char listname[30];
    fgets(listname, 30, stdin);
    int len = (int)strlen(listname) - 1;
    // Set the '\n' of the listname array to '\0' to terminate the string
    listname[len] = '\0';
    // Initialize a pointer to a List data type to point to the board
    List* check = *board;

    // Loop through the board to find the specified list
    while (check != NULL )
    {
        // If the name of the current list matches the entered list name, call function  with specified option
        if ( strcmp(check->name, listname) == 0)
        {
            function(&check, option);
            break;
        }
        else
        {
            // If the name of the current list does not match the entered list name, move to the next list
            check = check->nextList;
        }
    }
    
    // If the specified list is not found, print a message to the console
    if (check == NULL)
    {
        printf("List not found");
    }
}

