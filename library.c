/*
  Project Title: Library Book Tracking System
  Student Name: Pratyush Deo Singh
  Register No: 25BCE5101
  DA1 - C Programming Mini Project
  Department: BCE
  Course: Structured and Object Oriented Programming
  Faculty: Dr. Dinakaran M
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ========================= Constants ========================= */
#define MAX_BOOKS 500
#define TITLE_LEN 100
#define AUTHOR_LEN 60
#define STATUS_LEN 20
#define REGNO_LEN 20
#define DATA_FILE "library.dat"

/* ========================= Struct Definition ========================= */
/* Book struct holds all information about a single library book */
typedef struct {
    int BookID;                  /* Unique identifier for the book */
    char Title[TITLE_LEN];      /* Title of the book */
    char Author[AUTHOR_LEN];    /* Author name */
    char Status[STATUS_LEN];    /* "Available" or "Issued" */
    char IssuedToRegNo[REGNO_LEN]; /* empty if available, student reg no if issued */
} Book;

/* ========================= Global Variables ========================= */
Book books[MAX_BOOKS];  /* Array to store all book records */
int bookCount = 0;      /* Current number of books in the system */

/* ========================= Function Prototypes ========================= */
void loadFromFile(void);
void saveToFile(void);
void addBook(void);
void issueBook(void);
void returnBook(void);
void searchByID(void);
void searchByKeyword(void);
void displayAllBooks(void);
void displayIssuedBooks(void);
void printBookDetails(const Book *b);
void printTableHeader(void);
void printTableRow(const Book *b);
int findBookIndexByID(int id);
void toLowerStr(const char *src, char *dest, int maxLen);
void clearInputBuffer(void);
void displayMenu(void);

/* ========================= Main Function ========================= */
int main(void)
{
    int choice;

    /* Load existing records from binary file on startup */
    loadFromFile();

    printf("==========================================================\n");
    printf("       LIBRARY BOOK TRACKING SYSTEM\n");
    printf("       Pratyush Deo Singh | 25BCE5101\n");
    printf("==========================================================\n");

    /* Main menu loop — runs until user selects Exit */
    while (1) {
        displayMenu();
        printf("Enter your choice (1-8): ");

        /* Input validation: ensure the user enters an integer */
        if (scanf("%d", &choice) != 1) {
            printf("\n[ERROR] Invalid input. Please enter a number between 1 and 8.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        /* Menu routing using switch/case */
        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                issueBook();
                break;
            case 3:
                returnBook();
                break;
            case 4:
                searchByID();
                break;
            case 5:
                searchByKeyword();
                break;
            case 6:
                displayAllBooks();
                break;
            case 7:
                displayIssuedBooks();
                break;
            case 8:
                /* Save all records before exiting */
                saveToFile();
                printf("\nAll records saved. Exiting. Goodbye!\n");
                return 0;
            default:
                printf("\n[ERROR] Invalid choice. Please select between 1 and 8.\n");
                break;
        }
    }

    return 0;
}

/* ========================= Function Definitions ========================= */

/*
 * displayMenu — prints the menu options to the screen.
 * Called at the start of every loop iteration.
 */
void displayMenu(void)
{
    printf("\n----------------------------------------------------------\n");
    printf("  MENU\n");
    printf("----------------------------------------------------------\n");
    printf("  1. Add Book\n");
    printf("  2. Issue Book to Student\n");
    printf("  3. Return Book\n");
    printf("  4. Search Book by ID\n");
    printf("  5. Search Book by Title Keyword\n");
    printf("  6. Display All Books\n");
    printf("  7. Display Currently Issued Books\n");
    printf("  8. Exit\n");
    printf("----------------------------------------------------------\n");
}

/*
 * clearInputBuffer — discards leftover characters in stdin.
 * Prevents scanf issues when user enters unexpected input.
 */
void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; /* discard each character until newline or EOF */
}

/*
 * toLowerStr — converts a string to lowercase for case-insensitive comparison.
 * Copies src into dest in lowercase, up to maxLen-1 characters.
 */
void toLowerStr(const char *src, char *dest, int maxLen)
{
    int i;
    for (i = 0; i < maxLen - 1 && src[i] != '\0'; i++) {
        dest[i] = (char)tolower((unsigned char)src[i]);
    }
    dest[i] = '\0';
}

/*
 * findBookIndexByID — searches the books array for a given BookID.
 * Returns the index if found, or -1 if not found.
 */
int findBookIndexByID(int id)
{
    int i;
    for (i = 0; i < bookCount; i++) {
        if (books[i].BookID == id) {
            return i;
        }
    }
    return -1; /* not found */
}

/*
 * printTableHeader — prints a formatted header row for the book table.
 */
void printTableHeader(void)
{
    printf("\n%-8s %-30s %-25s %-12s %-12s\n",
           "BookID", "Title", "Author", "Status", "IssuedTo");
    printf("-------- ------------------------------ "
           "------------------------- ------------ ------------\n");
}

/*
 * printTableRow — prints one book record as a formatted table row.
 */
void printTableRow(const Book *b)
{
    /* If IssuedToRegNo is empty, show "-"; otherwise print the string */
    if (b->IssuedToRegNo[0] == '\0') {
        printf("%-8d %-30.30s %-25.25s %-12s %-12s\n",
               b->BookID, b->Title, b->Author, b->Status, "-");
    } else {
        printf("%-8d %-30.30s %-25.25s %-12s %-12s\n",
               b->BookID, b->Title, b->Author, b->Status, b->IssuedToRegNo);
    }
}

/*
 * printBookDetails — prints full details of a single book in a vertical format.
 */
void printBookDetails(const Book *b)
{
    printf("\n  Book ID       : %d\n", b->BookID);
    printf("  Title         : %s\n", b->Title);
    printf("  Author        : %s\n", b->Author);
    printf("  Status        : %s\n", b->Status);
    if (b->IssuedToRegNo[0] != '\0') {
        printf("  Issued To     : %s\n", b->IssuedToRegNo);
    } else {
        printf("  Issued To     : None\n");
    }
}

/* -------------------- File Handling Functions -------------------- */

/*
 * loadFromFile — reads all Book records from the binary file "library.dat"
 * into the books[] array on program startup.
 * If the file does not exist, it starts with an empty library (no error).
 */
void loadFromFile(void)
{
    FILE *fp = fopen(DATA_FILE, "rb");

    if (fp == NULL) {
        /* File not found — this is normal on first run */
        printf("[INFO] No existing data file found. Starting with empty library.\n");
        bookCount = 0;
        return;
    }

    /* Read records one by one until EOF */
    bookCount = 0;
    while (bookCount < MAX_BOOKS &&
           fread(&books[bookCount], sizeof(Book), 1, fp) == 1) {
        bookCount++;
    }

    fclose(fp);
    printf("[INFO] Loaded %d book(s) from %s.\n", bookCount, DATA_FILE);
}

/*
 * saveToFile — writes all Book records from the books[] array
 * to the binary file "library.dat", overwriting previous contents.
 */
void saveToFile(void)
{
    FILE *fp = fopen(DATA_FILE, "wb");

    if (fp == NULL) {
        printf("[ERROR] Could not open %s for writing.\n", DATA_FILE);
        return;
    }

    if (bookCount > 0) {
        size_t written = fwrite(books, sizeof(Book), bookCount, fp);
        if ((int)written != bookCount) {
            printf("[ERROR] Only %d of %d record(s) written to %s.\n",
                   (int)written, bookCount, DATA_FILE);
            fclose(fp);
            return;
        }
    }

    fclose(fp);
    printf("[INFO] %d book(s) saved to %s.\n", bookCount, DATA_FILE);
}

/* -------------------- Core Feature Functions -------------------- */

/*
 * addBook — prompts the user for book details and adds a new Book record.
 * Validates that the BookID is positive and not a duplicate.
 * Sets default status to "Available" and IssuedToRegNo to empty string.
 */
void addBook(void)
{
    Book newBook;
    int id;

    printf("\n--- Add New Book ---\n");

    /* Check if library is full */
    if (bookCount >= MAX_BOOKS) {
        printf("[ERROR] Library is full. Cannot add more books.\n");
        return;
    }

    /* Get and validate BookID */
    printf("Enter Book ID (positive integer): ");
    if (scanf("%d", &id) != 1 || id <= 0) {
        printf("[ERROR] Invalid Book ID. Must be a positive integer.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    /* Check for duplicate BookID */
    if (findBookIndexByID(id) != -1) {
        printf("[ERROR] Book ID %d already exists. Duplicate IDs are not allowed.\n", id);
        return;
    }

    newBook.BookID = id;

    /* Get book title */
    printf("Enter Title: ");
    if (fgets(newBook.Title, TITLE_LEN, stdin) == NULL) {
        printf("[ERROR] Failed to read title.\n");
        return;
    }
    /* Remove trailing newline from fgets */
    newBook.Title[strcspn(newBook.Title, "\n")] = '\0';

    /* Validate title is not empty */
    if (strlen(newBook.Title) == 0) {
        printf("[ERROR] Title cannot be empty.\n");
        return;
    }

    /* Get author name */
    printf("Enter Author: ");
    if (fgets(newBook.Author, AUTHOR_LEN, stdin) == NULL) {
        printf("[ERROR] Failed to read author.\n");
        return;
    }
    newBook.Author[strcspn(newBook.Author, "\n")] = '\0';

    /* Validate author is not empty */
    if (strlen(newBook.Author) == 0) {
        printf("[ERROR] Author cannot be empty.\n");
        return;
    }

    /* Set default values for a newly added book */
    strcpy(newBook.Status, "Available");
    newBook.IssuedToRegNo[0] = '\0';

    /* Add to array */
    books[bookCount] = newBook;
    bookCount++;

    /* Save immediately so data is not lost */
    saveToFile();

    printf("\n[SUCCESS] Book \"%s\" (ID: %d) added successfully.\n",
           newBook.Title, newBook.BookID);
}

/*
 * issueBook — issues a book to a student.
 * The book must exist and have Status "Available".
 * Changes Status to "Issued" and stores the student's register number.
 */
void issueBook(void)
{
    int id, idx;
    char regNo[REGNO_LEN];

    printf("\n--- Issue Book to Student ---\n");

    /* Check if there are any books in the system */
    if (bookCount == 0) {
        printf("[INFO] No books in the library yet.\n");
        return;
    }

    /* Get the BookID to issue */
    printf("Enter Book ID to issue: ");
    if (scanf("%d", &id) != 1 || id <= 0) {
        printf("[ERROR] Invalid Book ID.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    /* Find the book */
    idx = findBookIndexByID(id);
    if (idx == -1) {
        printf("[ERROR] Book ID %d not found.\n", id);
        return;
    }

    /* Check if already issued */
    if (strcmp(books[idx].Status, "Issued") == 0) {
        printf("[ERROR] Book \"%s\" is already issued to student %s.\n",
               books[idx].Title, books[idx].IssuedToRegNo);
        return;
    }

    /* Get student register number as string */
    printf("Enter Student Register Number: ");
    if (fgets(regNo, REGNO_LEN, stdin) == NULL) {
        printf("[ERROR] Failed to read register number.\n");
        return;
    }
    /* Remove trailing newline */
    regNo[strcspn(regNo, "\n")] = '\0';

    /* Validate register number is not empty */
    if (strlen(regNo) == 0) {
        printf("[ERROR] Register Number cannot be empty.\n");
        return;
    }

    /* Update book record */
    strcpy(books[idx].Status, "Issued");
    strncpy(books[idx].IssuedToRegNo, regNo, REGNO_LEN - 1);
    books[idx].IssuedToRegNo[REGNO_LEN - 1] = '\0';

    /* Save changes */
    saveToFile();

    printf("\n[SUCCESS] Book \"%s\" (ID: %d) issued to student %s.\n",
           books[idx].Title, books[idx].BookID, books[idx].IssuedToRegNo);
}

/*
 * returnBook — returns a previously issued book.
 * The book must exist and have Status "Issued".
 * Resets Status to "Available" and clears IssuedToRegNo.
 */
void returnBook(void)
{
    int id, idx;

    printf("\n--- Return Book ---\n");

    if (bookCount == 0) {
        printf("[INFO] No books in the library yet.\n");
        return;
    }

    printf("Enter Book ID to return: ");
    if (scanf("%d", &id) != 1 || id <= 0) {
        printf("[ERROR] Invalid Book ID.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    /* Find the book by ID */
    idx = findBookIndexByID(id);
    if (idx == -1) {
        printf("[ERROR] Book ID %d not found.\n", id);
        return;
    }

    /* Verify the book is currently issued */
    if (strcmp(books[idx].Status, "Available") == 0) {
        printf("[ERROR] Book \"%s\" is not currently issued. It is already available.\n",
               books[idx].Title);
        return;
    }

    printf("[INFO] Returning book \"%s\" from student %s.\n",
           books[idx].Title, books[idx].IssuedToRegNo);

    /* Reset the book status */
    strcpy(books[idx].Status, "Available");
    books[idx].IssuedToRegNo[0] = '\0';

    /* Save changes */
    saveToFile();

    printf("[SUCCESS] Book \"%s\" (ID: %d) returned successfully.\n",
           books[idx].Title, books[idx].BookID);
}

/*
 * searchByID — prompts for a BookID and displays the matching book details.
 */
void searchByID(void)
{
    int id, idx;

    printf("\n--- Search Book by ID ---\n");

    if (bookCount == 0) {
        printf("[INFO] No books in the library yet.\n");
        return;
    }

    printf("Enter Book ID to search: ");
    if (scanf("%d", &id) != 1 || id <= 0) {
        printf("[ERROR] Invalid Book ID.\n");
        clearInputBuffer();
        return;
    }
    clearInputBuffer();

    idx = findBookIndexByID(id);
    if (idx == -1) {
        printf("[INFO] No book found with ID %d.\n", id);
    } else {
        printf("\n--- Book Found ---");
        printBookDetails(&books[idx]);
    }
}

/*
 * searchByKeyword — searches all book titles for a keyword (case-insensitive).
 * Displays all matching books in a table format.
 */
void searchByKeyword(void)
{
    char keyword[TITLE_LEN];
    char lowerTitle[TITLE_LEN];
    char lowerKeyword[TITLE_LEN];
    int i, found = 0;

    printf("\n--- Search Book by Title Keyword ---\n");

    if (bookCount == 0) {
        printf("[INFO] No books in the library yet.\n");
        return;
    }

    printf("Enter keyword to search in titles: ");
    if (fgets(keyword, TITLE_LEN, stdin) == NULL) {
        printf("[ERROR] Failed to read keyword.\n");
        return;
    }
    keyword[strcspn(keyword, "\n")] = '\0';

    if (strlen(keyword) == 0) {
        printf("[ERROR] Keyword cannot be empty.\n");
        return;
    }

    /* Convert keyword to lowercase for case-insensitive matching */
    toLowerStr(keyword, lowerKeyword, TITLE_LEN);

    /* Search through all books */
    for (i = 0; i < bookCount; i++) {
        toLowerStr(books[i].Title, lowerTitle, TITLE_LEN);

        /* Check if the keyword is a substring of the title */
        if (strstr(lowerTitle, lowerKeyword) != NULL) {
            if (found == 0) {
                printf("\n--- Matching Books ---");
                printTableHeader();
            }
            printTableRow(&books[i]);
            found++;
        }
    }

    if (found == 0) {
        printf("[INFO] No books found with keyword \"%s\" in the title.\n", keyword);
    } else {
        printf("\nTotal matches: %d\n", found);
    }
}

/*
 * displayAllBooks — displays all books in the library in a formatted table.
 */
void displayAllBooks(void)
{
    int i;

    printf("\n--- All Books in Library ---\n");

    if (bookCount == 0) {
        printf("[INFO] No books in the library yet.\n");
        return;
    }

    printTableHeader();
    for (i = 0; i < bookCount; i++) {
        printTableRow(&books[i]);
    }
    printf("\nTotal books: %d\n", bookCount);
}

/*
 * displayIssuedBooks — displays only the books that are currently issued,
 * along with the register number of the student they are issued to.
 */
void displayIssuedBooks(void)
{
    int i, found = 0;

    printf("\n--- Currently Issued Books ---\n");

    if (bookCount == 0) {
        printf("[INFO] No books in the library yet.\n");
        return;
    }

    /* Iterate and filter only issued books */
    for (i = 0; i < bookCount; i++) {
        if (strcmp(books[i].Status, "Issued") == 0) {
            if (found == 0) {
                printTableHeader();
            }
            printTableRow(&books[i]);
            found++;
        }
    }

    if (found == 0) {
        printf("[INFO] No books are currently issued.\n");
    } else {
        printf("\nTotal issued books: %d\n", found);
    }
}
