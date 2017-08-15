#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

typedef struct{
    char isbn[15];
    char title[51];
    char author[51];
    float price;
}Books;

void printBook(Books book);
void printLine();
void printTitle();
Books getBook();
void getString(char *str, int length);

int callback(void *data, int argc, char **argv, char **azColName);

int main(int argc, char* argv[])
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char sql[500];
    const char* data = "Callback function called";
    
    /* Open database */
    rc = sqlite3_open("books.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }else{
        fprintf(stderr, "Opened database successfully\n");
    }
    
    char isbn[15];
    printf("Input ISBN to delete: ");
    getString(isbn, 14);
    
    /* Create SQL statement */
    sprintf(sql, "SELECT * from BOOKS WHERE ISBN='%s';", isbn);
    
    printTitle();
    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if(rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        printf("Do you want to delete this Book? [Y/N]: ");
        char yn = getchar();
        if(yn=='y' || yn=='Y'){
            sprintf(sql, "DELETE from BOOKS WHERE ISBN='%s';", isbn);
            int rcDelete = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
            if(rcDelete != SQLITE_OK ){
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            }else{
                fprintf(stdout, "Operation done successfully\n");
            }
        }
    }
    
    sqlite3_close(db);
    return 0;
}
int callback(void *data, int argc, char **argv, char **azColName){
    Books b;
    strcpy(b.isbn, argv[0] ? argv[0] : "NULL");
    strcpy(b.title, argv[1] ? argv[1] : "NULL");
    strcpy(b.author, argv[2] ? argv[2] : "NULL");
    b.price = atof(argv[3]);
    
    printBook(b);
    printLine();
    return 0;
}
void printBook(Books book){
    printf( "| %-14s | %-26s | %-20s | %6.2f |\n",
           book.isbn, book.title, book.author, book.price);
}
void printLine(){
    printf( "+-%-14s-+-%-26s-+-%-20s-+-%-6s-+\n", "--------------",
           "--------------------------", "--------------------", "------");
}
void printTitle(){
    printLine();
    printf( "| %-14s | %-26s | %-20s | %-6s |\n", "ISBN", "Title",
           "Author", "Price");
    printLine();
}
void getString(char *str, int length){
    //clear keyboard buffer on UNIX
    fseek(stdin, 0, SEEK_END);
    //clear keyboard buffer on Windows
    fflush(stdin);
    //input string
    fgets(str, length, stdin);
    str[strlen(str)-1] = '\0';
    //clear keyboard buffer on UNIX
    fseek(stdin, 0, SEEK_END);
    //clear keyboard buffer on Windows
    fflush(stdin);
}
Books getBook(){
    Books book;
    printf("Input Book isbn: ");
    getString(book.isbn, 14);
    printf("Input Book title: ");
    getString(book.title, 50);
    printf("Input Book author: ");
    getString(book.author, 50);
    printf("Input Book price: ");
    scanf("%f", &book.price);
    return book;
}