/* Abbas Nassar 1210482 Sec.3 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int maxDigits = 0;

/* Structs definitions */
struct district;
typedef struct district *ptrToDistrict;
typedef struct district *districtList;

struct town;
typedef struct town *ptrToTown;
typedef struct town *townList;

struct district
{
    ptrToDistrict prev;
    char districtName[32];
    int districtPopulation;
    ptrToTown townsInDistrict;
    ptrToDistrict next;
};

struct town
{
    ptrToTown prev;
    char townName[32];
    int townPopulation;
    ptrToTown next;
};

void printLoadedInfo();            // Read data from the file as it is.
void readFromFile(districtList L); // Reading the data from the file and storing them in the lists.

districtList createDistrictList();                                                            // Create district List. Returns head of list.
void insertDistrict(districtList L, char insertedDistrictName[]);                             // Insert district node at the end of the list.
int checkDistrict(districtList L, char insertedDistrictName[]);                               // returns 1 if the inserted district name is already in the list.
ptrToDistrict copyDistrictNode(districtList L, char insertedDistrictName[], int districtPop); // Used in the radix sort function to copy a district node and adds it to destination list.
// It returns a pointer to the newly copied and inserted node, so I can insert list of towns to it.
ptrToDistrict searchForDistrict(districtList L, char key[]);                                  // Returns a pointer to the wanted node.
void deleteDistrict(districtList L, char districtName[]);                                     // Deletes a specific district.
void deleteDistrictList(districtList L);                                                      // Delete a whole district list.
void copyDistrictList(districtList dest, districtList src);                                   // Copy a district list.
void printDistrictList(districtList L);                                                       // Prints district list with its population.
void printDistrictTownsWithPopulation(districtList L);                                        // Prints district list with its town and population.

townList createTownList();                                                                            // Create town List. Returns head of list.
void insertTown(ptrToDistrict dist, townList L, char insertedTownName[], int insertedTownPopulation); // insert town node at the end of the list.
void copyTownList(districtList dest, districtList src);                                               // Copy's a town list from list of districts. Used in radix sort function.
int checkTown(townList L, char insertedTownName[]);                                                   // returns 1 if the inserted district name is already in the list.
void sortTowns(districtList L);
void deleteTownFromDistrict(districtList L, char districtName[], char townName[]); // Delete specific town form a certain district.
void deleteTownList(ptrToDistrict dist);                                           // Delete a whole town list.
void printTownList(townList L);                                                    // Print town list.

void sortedTownList(townList L);                                               // Insertion sort used to sort towns based on its population in ascending order.
void changeTownPopulation(districtList dist, char key[], int insertedTownPop); // Change a specific town population.
void calculateDistrictsPopulation(districtList L);                             // Calculate the whole population of palestine, and max, min of districts and towns.
void radixSort(districtList L);                                                // Radix sort used to sort district list.
void addSpaces(districtList L);                                                // Add spaces to a string after the string. Used in radix sort to make all the strings the same size.
void deleteSpaces(districtList L);                                             // Delete all the spaces from a string. Used in radix sort to return the strings to its original sizes.
void cleanStringFromExtraSpaces(char insertedString[]);                        // Clean string from spaces entered after the word.

void saveInFile(districtList L); // Store the data in a file in the wanted format.

int main()
{
    /* Creating a district list. */
    ptrToDistrict listOfDistricts;
    listOfDistricts = createDistrictList();

    /* Reading from file and insert them in district, and town lists. */
    readFromFile(listOfDistricts);

    /* Sorting the data. */
    radixSort(listOfDistricts);

    /* Menu. */
    int selection = 0;
    printf("Palestine tomorrow will be free\n\n");
    do
    {
        printf("1- Print the loaded information:\n");
        printf("2- Print the sorted information:\n");
        printf("3- Add a district to the sorted list of districts:\n");
        printf("4- Add new town to a certain district:\n");
        printf("5- Delete a town from a specific district:\n");
        printf("6- Delete a complete district:\n");
        printf("7- Calculate the population of palestine the max and min population of districts and towns:\n");
        printf("8- Print each district with its total population:\n");
        printf("9- Change the population of a town:\n");
        printf("10- Save to output file:\n");
        printf("11- Enter -1 to exit the program:\n");
        scanf("%d", &selection);

        /* Print the loaded information. */
        if (selection == 1)
            printLoadedInfo();
            /* Print the sorted information. */
        else if (selection == 2)
        {
            sortTowns(listOfDistricts);
            printDistrictTownsWithPopulation(listOfDistricts);
        }
            /* Add a district to the sorted list of districts. */
        else if (selection == 3)
        {
            char insertedDistrictName[32];
            printf("Insert the new district name:");
            getchar(); // Used for gets to work well.
            gets(insertedDistrictName);
            if (checkDistrict(listOfDistricts, insertedDistrictName))
            { // If inserted already exists.
                printf("%s already exists in the list\n", insertedDistrictName);
                continue;
            }
            cleanStringFromExtraSpaces(insertedDistrictName); // Make sure string is clean to handle it.
            insertDistrict(listOfDistricts, insertedDistrictName);
            radixSort(listOfDistricts);
            printf("%s district is added successfully\n", insertedDistrictName);
        }
            /* Add new town to a certain district. */
        else if (selection == 4)
        {
            char insertedDistrictName[32];
            char insertedTownName[32];
            int townPopulation;
            printf("Insert the district name you want to add the town to:");
            getchar();
            gets(insertedDistrictName);
            cleanStringFromExtraSpaces(insertedDistrictName);
            ptrToDistrict selectedDistrict = searchForDistrict(listOfDistricts, insertedDistrictName);
            if (selectedDistrict == NULL)
            {
                printf("%s district is not found in the district list\n", insertedDistrictName);
            }
            else
            {
                printf("\nInsert the Town name you want to add:");
                gets(insertedTownName);
                cleanStringFromExtraSpaces(insertedTownName);
                if (checkTown(selectedDistrict->townsInDistrict, insertedTownName))
                {
                    printf("%s is already in town list.\n", insertedTownName);
                    continue;
                }
                printf("\nInsert town population:");
                scanf("%d", &townPopulation);
                insertTown(selectedDistrict, selectedDistrict->townsInDistrict, insertedTownName, townPopulation);
                sortedTownList(selectedDistrict->townsInDistrict);
                printf("%s is successfully added to %s district\n", insertedTownName, insertedDistrictName);
            }
        }
            /* Delete a town from a specific district. */
        else if (selection == 5)
        {
            char insertedDistrictName[32];
            char insertedTownName[32];
            printf("Insert district name that you want to delete a town from:");
            getchar();
            gets(insertedDistrictName);
            cleanStringFromExtraSpaces(insertedDistrictName);
            ptrToDistrict selectedDistrict = searchForDistrict(listOfDistricts, insertedDistrictName);
            if (selectedDistrict == NULL)
            {
                printf("%s district is not found in the district list\n", insertedDistrictName);
            }
            else
            {
                printf("\nInsert town name you want to delete:");
                gets(insertedTownName);
                cleanStringFromExtraSpaces(insertedTownName);
                deleteTownFromDistrict(listOfDistricts, insertedDistrictName, insertedTownName);
                printf("%s is deleted from %s district\n", insertedTownName, insertedDistrictName);
            }
        }
            /* Delete a district from a district list. */
        else if (selection == 6)
        {
            char insertedDistrictName[32];
            printf("Insert the district name you want to delete:");
            getchar();
            gets(insertedDistrictName);
            cleanStringFromExtraSpaces(insertedDistrictName);
            ptrToDistrict selectedDistrict = searchForDistrict(listOfDistricts, insertedDistrictName);
            if (selectedDistrict == NULL)
            {
                printf("%s district is not found in the district list\n", insertedDistrictName);
            }
            else
            {
                deleteDistrict(listOfDistricts, insertedDistrictName);
                printf("%s is deleted from district list\n", insertedDistrictName);
            }
        }
            /* Calculate population. */
        else if (selection == 7)
            calculateDistrictsPopulation(listOfDistricts);
            /* Print each district with its population. */
        else if (selection == 8)
            printDistrictList(listOfDistricts);
            /* Change town population without entering which district it belongs to. */
        else if (selection == 9)
        {
            char insertedTownName[32];
            int insertedTownPopulation;
            printf("Insert town name that you want to change its population:");
            getchar();
            gets(insertedTownName);
            cleanStringFromExtraSpaces(insertedTownName);
            printf("\nInsert the new population:");
            scanf("%d", &insertedTownPopulation);
            changeTownPopulation(listOfDistricts, insertedTownName, insertedTownPopulation);
        }
            /* Save to output file. */
        else if (selection == 10)
            saveInFile(listOfDistricts);
        else if(selection == -1){
            printf("Best of luck :)\n");
            break;
        }
        else
            printf("You've entered an invalid selection number. Try again :)\n");

    } while (1);
    return 0;
}

void printLoadedInfo()
{
    /* Open file. */
    FILE *file = fopen("../districts.txt", "r");
    if (file == NULL)
    {
        printf("File is not found..");
    }
    char line[50]; // 50 is the maximum size of each line.
    /* Read all the lines until the end is reached. */
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }
    /* Close file. */
    fclose(file);
}
void readFromFile(districtList L)
{
    FILE *file = fopen("../districts.txt", "r");
    if (file == NULL)
    {
        printf("File is not found..");
    }
    char line[50]; // 50 is the maximum size of each line.
    /* Read all the lines until the end is reached. */
    while (fgets(line, sizeof(line), file) != NULL)
    {
        int i = 0, spaceNum = 0; // i used for iterations.
        // spaceNum is used to count how much spaces read from the file to handle them.

        /* Variables to temporarily store the read data in. */
        char inputDName[32];
        char inputTName[32];
        char inputPopulation[7];

        /* This loop used to read and store the district name. */
        while (1)
        {
            if (line[i] == '|') // Reached the end of district name section.
                break;
            if (line[i] != ' ')
            { // Don't store spaces.
                inputDName[i] = line[i];
            }
            else if (line[i - 1] != ' ' && line[i + 1] != ' ' && line[i - 1] != '|' && line[i + 1] != '|')
            { // Unless it's a space between to words
                inputDName[i] = line[i];
            }
            else
            { // It is extra space.
                spaceNum++;
            }
            i++;
        }
        inputDName[i - spaceNum] = '\0'; // Unwanted spaces are excluded.

        int j = 0; // New counter used as index in inputTName.
        i++;       // Move to the next section of the line (inputTName section)

        /* This loop used to read and store the town name. */
        while (1)
        {
            if (line[i] == '|')
                break;
            if (line[i] != ' ')
            {
                inputTName[j] = line[i];
                j++;
            }
            else if (line[i - 1] != ' ' && line[i + 1] != ' ' && line[i - 1] != '|' && line[i + 1] != '|')
            {
                inputTName[j] = line[i];
                j++;
            }
            i++;
        }
        inputTName[j] = '\0';
        int c = 0;
        i++;
        /* This loop used to read and store town population. */
        while (1)
        {
            if (line[i] == NULL) // Reached the end of the line.
                break;

            if (line[i] != ' ')
            {
                inputPopulation[c] = line[i];
                c++;
            }
            i++;
        }
        inputPopulation[c] = '\0';

        if (strlen(inputDName) > maxDigits) // Updated the maxDigits counter.
            maxDigits = strlen(inputDName);
        /* Inserting the district then searching for it to insert the input town to it*/
        insertDistrict(L, inputDName);
        ptrToDistrict headOfTowns = searchForDistrict(L, inputDName);
        insertTown(headOfTowns, headOfTowns->townsInDistrict, inputTName, atoi(inputPopulation));
    }

    fclose(file);
}

districtList createDistrictList()
{
    districtList L;
    L = (districtList)malloc(sizeof(struct district));
    if (L == NULL)
    {
        printf("Could not create district list. List is not found..\n");
        return NULL;
    }
    else
    {
        L->next = NULL;
        L->prev = NULL;
    }

    return L;
}
int checkDistrict(districtList L, char insertedDistrictName[])
{
    ptrToDistrict temp = L->next;
    int duplicated = 0; // A flag to check if the district name is already in the list
    while (temp != NULL)
    {
        if (strcmp(temp->districtName, insertedDistrictName) == 0)
        {
            duplicated = 1;
            return 1; // If the inserted node already exists.
        }
        temp = temp->next;
    }
    return 0;
}
void insertDistrict(districtList L, char insertedDistrictName[])
{
    ptrToDistrict newNode = (ptrToDistrict)malloc(sizeof(struct district));
    strcpy(newNode->districtName, insertedDistrictName); // Needed strcpy to copy the inserted string to the node.
    if (strlen(insertedDistrictName) > maxDigits)        // Updates maxDigits counter.
        maxDigits = strlen(insertedDistrictName);
    newNode->districtPopulation = 0;
    newNode->prev = NULL;
    newNode->next = NULL;
    newNode->townsInDistrict = createTownList();

    if (L == NULL)
    {
        printf("District list is Not Found..\n");
        return;
    }
    else if (L->next == NULL)
    {
        newNode->next = L->next;
        L->next = newNode;
        newNode->prev = L;
    }
    else
    {
        if (!checkDistrict(L, insertedDistrictName))
        {
            ptrToDistrict temp = L->next;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }

            newNode->next = temp->next;
            temp->next = newNode;
            newNode->prev = temp;
        }
        else
            free(newNode);
    }
}
ptrToDistrict copyDistrictNode(districtList L, char insertedDistrictName[], int districtPop)
{
    ptrToDistrict newNode = (ptrToDistrict)malloc(sizeof(struct district));
    strcpy(newNode->districtName, insertedDistrictName); // Needed strcpy to copy the inserted string to the node
    newNode->districtPopulation = districtPop;
    newNode->prev = NULL;
    newNode->next = NULL;
    newNode->townsInDistrict = createTownList();
    if (L == NULL)
    {
        printf("District list Not Found..\n");
    }
    else if (L->next == NULL)
    {
        newNode->next = L->next;
        L->next = newNode;
        newNode->prev = L;
        return newNode;
    }
    else
    {
        ptrToDistrict temp = L->next;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        newNode->next = temp->next;
        temp->next = newNode;
        newNode->prev = temp;
        return newNode;
    }
}
void copyDistrictList(districtList dest, districtList src)
{
    ptrToDistrict temp = src->next;
    while (temp != NULL)
    {
        ptrToDistrict selectedDistrict = copyDistrictNode(dest, temp->districtName, temp->districtPopulation);
        ptrToTown tempTown = temp->townsInDistrict->next;
        while (tempTown != NULL)
        {
            insertTown(selectedDistrict, selectedDistrict->townsInDistrict, tempTown->townName, tempTown->townPopulation);
            tempTown = tempTown->next;
        }
        temp = temp->next;
    }
}
ptrToDistrict searchForDistrict(districtList L, char key[])
{
    ptrToDistrict temp = L->next;
    while (temp != NULL)
    {
        if (strcmp(temp->districtName, key) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}
void calculateDistrictsPopulation(districtList L)
{
    int palestinePopulation = 0;
    ptrToDistrict temp = L->next;
    ptrToDistrict maxDistrictPopulation = temp;
    ptrToDistrict minDistrictPopulation = temp;
    ptrToTown tempTown = temp->townsInDistrict->next;
    ptrToTown maxTownPopulation = tempTown;
    ptrToTown minTownPopulation = tempTown;
    char maxDistrictName[32];
    char minDistrictName[32];
    strcpy(maxDistrictName, temp->districtName);
    strcpy(minDistrictName, temp->districtName);

    /* This loop will iterate through the districts searching for the minimum and maximum district population. */
    /* Then it will iterate through each district town list to find the minimum and maximum population. */
    /* Pointers are assigned to the wanted nodes to print its related data. */
    while (temp != NULL)
    {
        tempTown = temp->townsInDistrict->next;
        palestinePopulation += temp->districtPopulation;
        if (temp->districtPopulation > maxDistrictPopulation->districtPopulation)
            maxDistrictPopulation = temp;
        if (temp->districtPopulation < minDistrictPopulation->districtPopulation)
            minDistrictPopulation = temp;
        while (tempTown != NULL)
        {
            if (tempTown->townPopulation > maxTownPopulation->townPopulation)
            {
                strcpy(maxDistrictName, temp->districtName);
                maxTownPopulation = tempTown;
            }
            if (tempTown->townPopulation < minTownPopulation->townPopulation)
            {
                strcpy(minDistrictName, temp->districtName);
                minTownPopulation = tempTown;
            }
            tempTown = tempTown->next;
        }
        temp = temp->next;
    }
    printf("Palestine has %d Residents.\n", palestinePopulation);
    printf("Maximum district population is in %s district with %d Residents.\n", maxDistrictPopulation->districtName, maxDistrictPopulation->districtPopulation);
    printf("Minimum district population is in %s district with %d Residents.\n", minDistrictPopulation->districtName, minDistrictPopulation->districtPopulation);
    printf("Maximum Town population is in %s Town in %s district with %d Residents.\n", maxTownPopulation->townName, maxDistrictName, maxTownPopulation->townPopulation);
    printf("Minimum Town population is in %s Town in %s district with %d Residents.\n", minTownPopulation->townName, minDistrictName, minTownPopulation->townPopulation);
}
void deleteDistrict(districtList L, char districtName[])
{
    ptrToDistrict selectedDistrict = searchForDistrict(L, districtName);

    deleteTownList(selectedDistrict);
    ptrToDistrict temp = selectedDistrict;

    while (temp != NULL)
    {
        if (strcmp(temp->districtName, districtName) == 0)
        {
            if (temp->next == NULL)
            { // last node in the list
                temp->prev->next = temp->next;
                free(temp);
                return;
            }
            else if (temp->prev == NULL)
            { // first node in the list
                temp->next->prev = temp->prev;
                L->next = temp->next;
                free(temp);
                return;
            }
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            free(temp);
            return;
        }
        temp = temp->next;
    }
}

void deleteDistrictList(districtList L)
{
    ptrToDistrict ptr = L->next;
    L->next = NULL;
    ptrToDistrict temp;

    while (ptr != NULL)
    {
        temp = ptr->next;
        free(ptr);
        ptr = temp;
    }
}
void printDistrictTownsWithPopulation(districtList L)
{
    if (L == NULL)
    {
        printf("District list is not found..\n");
    }
    else if (L->next == NULL)
    {
        printf("District list is empty. Nothing to print..\n");
    }
    else
    {
        ptrToDistrict temp = L->next;

        while (temp != NULL)
        {
            printf("%s with population %d.\nIt's towns: ", temp->districtName, temp->districtPopulation);
            printTownList(temp->townsInDistrict);
            temp = temp->next;
        }
        printf("\n");
    }
}
void printDistrictList(districtList L)
{
    if (L == NULL)
    {
        printf("District list is not found..\n");
    }
    else if (L->next == NULL)
    {
        printf("District list is empty. Nothing to print..\n");
    }
    else
    {
        ptrToDistrict temp = L->next;

        while (temp != NULL)
        {
            printf("%s with population %d ", temp->districtName, temp->districtPopulation);
            temp = temp->next;
        }
        printf("\n");
    }
}

townList createTownList()
{
    townList L;
    L = (townList)malloc(sizeof(struct town));
    if (L == NULL)
    {
        printf("Could not create list. List is not found..\n");
        return NULL;
    }
    else
    {
        L->next = NULL;
        L->prev = NULL;
    }

    return L;
}
void insertTown(ptrToDistrict dist, townList L, char insertedTownName[], int insertedTownPopulation)
{
    ptrToTown newNode = (ptrToTown)malloc(sizeof(struct town));
    strcpy(newNode->townName, insertedTownName); // Needed strcpy() to copy the inserted string to the node
    newNode->townPopulation = insertedTownPopulation;
    dist->districtPopulation += insertedTownPopulation;

    newNode->prev = NULL;
    newNode->next = NULL;

    if (L == NULL)
    {
        printf("Linked List Not Found..\n");
    }
    else if (newNode == NULL)
    {
        printf("Out of space..\n");
    }
    else if (L->next == NULL)
    {
        newNode->next = L->next;
        L->next = newNode;
        newNode->prev = L;
    }
    else
    {
        if (!checkTown(L, insertedTownName))
        { // Check if the inserted node is not in the list(the method will return 0)
            ptrToTown temp = L->next;

            while (temp->next != NULL)
            {
                temp = temp->next;
            }

            newNode->next = temp->next;
            temp->next = newNode;
            newNode->prev = temp;
        }
        else
        {
            free(newNode);
        }
    }
}
void sortedTownList(townList L)
{
    ptrToTown sorted = NULL;
    ptrToTown current = L;
    while (current != NULL)
    {
        ptrToTown next = current->next;
        if (sorted == NULL || sorted->townPopulation >= current->townPopulation)
        {
            current->next = sorted;
            current->prev = NULL;
            if (sorted != NULL)
                sorted->prev = current;
            sorted = current;
        }
        else
        {
            ptrToTown temp = sorted;
            while (temp->next != NULL && temp->next->townPopulation < current->townPopulation)
                temp = temp->next;
            current->next = temp->next;
            if (temp->next != NULL)
                temp->next->prev = current;
            temp->next = current;
            current->prev = temp;
        }
        current = next;
    }
    L = sorted;
}
void changeTownPopulation(districtList dist, char key[], int insertedTownPop)
{
    /* This method will search all town list districts to find the wanted town then change its population. */
    ptrToDistrict selectedDistrict = dist->next;

    while (selectedDistrict != NULL)
    {
        ptrToTown selectedTown = selectedDistrict->townsInDistrict->next;
        while (selectedTown != NULL)
        {
            if (strcmp(selectedTown->townName, key) == 0)
            {
                selectedDistrict->districtPopulation -= selectedTown->townPopulation;
                selectedTown->townPopulation = insertedTownPop;
                selectedDistrict->districtPopulation += selectedTown->townPopulation;
                sortedTownList(selectedDistrict->townsInDistrict);
                printf("%s town population has been changed successfully\n", key);
                return;
            }

            selectedTown = selectedTown->next;
        }
        selectedDistrict = selectedDistrict->next;
    }
    printf("%s is not found in town lists.\n", key);
}
void copyTownList(districtList dest, districtList src)
{

    ptrToDistrict destTemp = dest->next;
    while (destTemp != NULL)
    {
        ptrToDistrict srcTemp = searchForDistrict(src, destTemp->districtName); // Find the district node to copy from.
        if (srcTemp == NULL)
            printf("Node with %s district name is not found\n", destTemp->districtName);
        else
        {
            townList srcTown = srcTemp->townsInDistrict;
            ptrToTown copyTown = srcTown->next;
            while (copyTown != NULL)
            { // Start inserting.
                insertTown(destTemp, destTemp->townsInDistrict, copyTown->townName, copyTown->townPopulation);
                copyTown = copyTown->next;
            }
        }
        destTemp = destTemp->next;
    }
}
int checkTown(townList L, char insertedTownName[])
{
    ptrToTown temp = L->next;
    int duplicated = 0; // A flag to check if the district name is already in the list
    while (temp != NULL)
    {
        if (strcmp(temp->townName, insertedTownName) == 0)
        {
            duplicated = 1;
            return 1; // If the inserted node already exists.
        }
        temp = temp->next;
    }
    return 0;
}
void sortTowns(districtList L)
{
    // This method will sort all districts town lists.
    ptrToDistrict temp = L->next;
    if (temp == NULL)
    {
        printf("District list is empty..");
        return;
    }
    else
    {
        while (temp != NULL)
        {
            sortedTownList(temp->townsInDistrict);
            temp = temp->next;
        }
    }
}
void deleteTownFromDistrict(districtList L, char districtName[], char townName[])
{
    ptrToDistrict selectedDistrict = searchForDistrict(L, districtName);
    if (selectedDistrict == NULL)
        printf("%s district is not found in the district list\n", districtName);
    else
    {
        ptrToTown selectedTown = selectedDistrict->townsInDistrict;

        if (selectedTown == NULL)
        {
            printf("Town list is Not Found..\n");
            return;
        }
        else if (selectedTown->next == NULL)
        {
            printf("Town list is empty. Nothing to delete..\n");
            return;
        }

        ptrToTown temp = selectedTown->next;
        while (temp != NULL)
        {
            if (strcmp(temp->townName, townName) == 0)
            {
                selectedDistrict->districtPopulation -= temp->townPopulation;
                if (temp->next == NULL)
                { // last node in the list
                    temp->prev->next = temp->next;
                    free(temp);
                    return;
                }
                else if (temp->prev == NULL)
                { // first node in the list
                    temp->next->prev = temp->prev;
                    L->next = temp->next;
                    free(temp);
                    return;
                }
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                free(temp);
                return;
            }
            temp = temp->next;
        }
        printf("The element is not found to be deleted..\n");
    }
}
void deleteTownList(ptrToDistrict dist)
{
    ptrToTown L = dist->townsInDistrict;
    dist->districtPopulation = 0;
    if (L == NULL)
    {
        printf("Town list is not found..\n");
        return;
    }
    else if (L->next == NULL)
    {
        printf("Town list is empty. Nothing to delete..\n");
        return;
    }

    ptrToTown ptr = L->next;
    L->next = NULL;
    ptrToTown temp;

    while (ptr != NULL)
    {
        temp = ptr->next;
        free(ptr);
        ptr = temp;
    }

    printf("Town list has been deleted..\n");
}
void printTownList(townList L)
{
    if (L == NULL)
    {
        printf("Town list is Not Found..\n");
    }
    else if (L->next == NULL)
    {
        printf("Town list is empty, nothing to print..\n");
    }
    else
    {
        ptrToTown temp = L->next;

        while (temp != NULL)
        {
            printf("%s %d ", temp->townName, temp->townPopulation);
            temp = temp->next;
        }
        printf("\n");
    }
}

void radixSort(districtList list)
{
    districtList copyList = createDistrictList();
    copyDistrictList(copyList, list);
    // Making all district names string the same size.
    addSpaces(list);
    // Creating an array of the linked list.
    districtList Arr[53]; // Number of english letters with the space.
    // First I initialized each indices. Now each one has a head of district list.
    for (int i = 0; i < 53; i++)
    {
        Arr[i] = createDistrictList();
    }
    // Start sorting based on the most right digit.
    for (int i = maxDigits - 1; i >= 0; --i)
    {
        // Move nodes to indices list based on the current character.
        ptrToDistrict p = list->next; // Used to indicate where to insert (last) the district node based on its wanted digit
        while (p != NULL)
        {
            // insert (last) the district node based on its wanted digit
            if (p->districtName[i] == ' ')
            {
                insertDistrict(Arr[0], p->districtName);
            }
            else if (p->districtName[i] >= 'A' && p->districtName[i] <= 'Z')
            {
                insertDistrict(Arr[p->districtName[i] - 64], p->districtName);
            }
            else if (p->districtName[i] >= 'a' && p->districtName[i] <= 'z')
            {
                insertDistrict(Arr[p->districtName[i] - 70], p->districtName);
            }
            p = p->next;
        }
        deleteDistrictList(list); // Delete the old list so the new one will attach on the same head
        // collecting the words from Arr and insert them in the list
        for (int j = 0; j < 53; ++j)
        {
            ptrToDistrict q = Arr[j]->next;
            while (q != NULL)
            {
                insertDistrict(list, q->districtName);
                q = q->next;
            }
            // clear the array to welcome a new list
            deleteDistrictList(Arr[j]);
        }
    }
    for (int i = 0; i < 53; i++)
    {
        free(Arr[i]); // Free array list
    }
    deleteSpaces(list);           // delete the spaces used to make the strings same size.
    copyTownList(list, copyList); // list contains just the district names Thus this function inserts town lists to their districts.
}
void addSpaces(districtList L)
{
    ptrToDistrict p = L->next;
    while (p != NULL)
    {
        int currentSize = strlen(p->districtName);
        if (currentSize < maxDigits)
        {
            for (int i = currentSize; i < maxDigits; i++) //  After Word
            {
                p->districtName[i] = ' ';
            }
            p->districtName[maxDigits] = '\0';
        }
        p = p->next;
    }
}
void deleteSpaces(districtList L)
{
    // This is for radix sort
    // Clean strings from spaces inserted after it.
    ptrToDistrict temp = L->next;
    while (temp != NULL)
    {
        cleanStringFromExtraSpaces(temp->districtName);
        temp = temp->next;
    }
}
void cleanStringFromExtraSpaces(char insertedString[])
{
    int numberOfSpaces = 0;
    for (int i = strlen(insertedString) - 1; i >= 0; i--)
    {
        if (insertedString[i] == ' ')
        {
            numberOfSpaces++;
        }
        else
            break;
    }
    insertedString[strlen(insertedString) - numberOfSpaces] = '\0';
}

void saveInFile(districtList L)
{
    FILE *file = fopen("../store.txt", "w");
    if (file == NULL)
    {
        printf("Error opening the file.\n");
    }
    ptrToDistrict temp = L->next;
    while (temp != NULL)
    {
        fprintf(file, "%s District, Population = %d\n", temp->districtName, temp->districtPopulation);
        ptrToTown tempTown = temp->townsInDistrict->next;
        while (tempTown != NULL)
        {
            fprintf(file, "%s, %d\n", tempTown->townName, tempTown->townPopulation);
            tempTown = tempTown->next;
        }
        temp = temp->next;
    }

    // Close the file
    fclose(file);
    printf("The data is stored successfully.\n");
}