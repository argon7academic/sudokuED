#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHUNK 1024
struct node{
    int pos;
    int size;
    int* data;
    struct node *next;
};
char *read_and_copy(char *filename);
int compare(char* sol, size_t size_unsol, char* unsol, int verbose);
int write_out(char* file, char *buffer, size_t size, int verbose);
int find_empty_cell(char* sudoku,size_t size_sudoku);
int is_it_cool(char* sudoku,int pos,int val);
int is_val_in_row(char* sudoku, int pos, int val);
int is_val_in_col(char* sudoku,int pos,int val);
int is_val_in_square(char* sudoku,int pos,int val);
int solve( char *buffer, size_t size,struct node** head);
int verify(char *buffer,size_t size,struct node** head);
int print(char *buffer, size_t size);
void append(struct node** head_ref, int* new_data,int size, int pos);
void deleteList(struct node** head_ref);
void printList(struct node *node);

int main(int argc , char* argv[]){
    struct node* head = NULL;

    //verify parameters
    if(!(argc==4 || argc==5)){
        printf("USAGE: %s <unsolved_sudoku> <solution> <output_file> [verbose]\n", argv[0]);
        printf("[verbose] must be == 'v' or left empty\n");
        exit(1);}
    if(argc==5 && strcmp(argv[4],"v")!=0){
        printf("USAGE: %s <unsolved_sudoku> <solution> <output_file> [verbose]\n", argv[0]);
        printf("[verbose] must be == 'v' or left empty\n");
        exit(1);}

    //variables
    int verbose=0;
    if(argc==5) verbose=1;
    char unsol[81];
    char sol[81];

    //read and copy files
    strcpy(unsol, read_and_copy(argv[1]));
    strcpy(sol, read_and_copy(argv[2]));

    //prints sudoku unsolved and the solution...
    if(verbose==1){
        printf("The solution given ...");
        print(sol,sizeof(sol));
        printf("\nUnsolved sudoku...");
        print(unsol,sizeof(unsol));
        printf("\n");}

    //compare the solution to the unsolved sudoku
    if(verbose==1)printf("Comparing the solution and the unsolved sudoku...\n\n");
    if(0==compare(sol,sizeof (sol),unsol,verbose))printf("\nsolving...\n\n");
    else{printf("\n unsolved file == solved file. Solving not required...\n"); return 0;}
    if(find_empty_cell(unsol,sizeof(unsol))==-1){
        printf("\n unsolved file has no empty cells to fill. There is no solution...\n");
        return 0;}

    //verify
    verify(unsol,sizeof (unsol),&head);
    //printlist
    if(verbose==1){printList(head);}
    //solve
    solve(unsol,sizeof (unsol),&head);

    //saves it
    if(verbose==1)printf("writing solution to file...\n\n");
    write_out(argv[3],unsol,sizeof (unsol),verbose);

    //prints out possible solution found...
    if(verbose==1){
        printf("\nPossible solution achieved...");
        print(unsol,sizeof (unsol));}

    //compare the solution to the solved sudoku
    if(verbose==1)printf("\nComparing the provided solution to ours...\n\n");
    if(0==compare(sol,sizeof (sol),unsol,verbose))printf("\nfailed to solve...\n\n");
    else printf("\nsolved.\nSaved in \"%s\"\n\n",argv[3]);
    return 0;}

char* read_and_copy(char *filename){
    FILE *pFile;
    long lSize;
    char *buffer;
    size_t result;
    pFile = fopen(filename, "r");
    if (pFile==NULL){
        fputs("File error\n", stderr);
        exit(1);}
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);
    if(lSize>CHUNK){lSize=CHUNK;}
    buffer = (char *)malloc(sizeof(char)*(unsigned long)lSize + 1);
    if (buffer == NULL) {
        fputs("Memory error", stderr);
        exit(1);}
    memset(buffer, 0, sizeof(char)*(unsigned long)lSize + 1);
    result = fread(buffer, sizeof(char), (unsigned long)lSize, pFile);
    if ((unsigned)result != lSize) {
        fputs("Reading error", stderr);
        exit(1);}
    fclose(pFile);
    return buffer;
}

int compare(char* unsol,size_t size_unsol, char *sol, int verbose){
    int result=1;
    if(verbose==1){
        for (unsigned long i=0; i < size_unsol; i++){
            printf("%c",sol[i]);
            if(sol[i]==unsol[i]){
                printf(" | sol = %c | OK\n",unsol[i]);}
            else {
                printf(" | sol = %c\n",unsol[i]);
                result = 0;}}}
    else{
        for (unsigned long i=0; i < sizeof(sol); i++){
            if(sol[i]!=unsol[i]){
                result = 0;
                break;}}}
    return(result);
}

int write_out(char* file, char *buffer, size_t size, int verbose){
    FILE *fp;
    fp = fopen(file, "w+");
    if (fp != NULL) {
        if(verbose==1){
            for (unsigned long i = 0; i < size ; i++) {
                printf("%c", buffer[i]);
                fputc (buffer[i], fp);}
            fclose(fp);}
        else if(verbose!=1){
            for (unsigned long i = 0; i < size ; i++) {
                fputc (buffer[i], fp);}
            fclose(fp);}}
    else {
        printf("\n Unable to Create or Open the Sample.txt File");
        exit(1);}
    return 0;
}

int print(char *buffer, size_t size){
    for (unsigned long i = 0; i < size; i++) {
        if(i%3 == 0)printf(" ");
        if(i%9 == 0)printf("\n");
        if(i%27 == 0)printf("\n");
        printf("%c ", buffer[i]);}
    printf("\n");
    return 0;
}

int find_empty_cell(char* sudoku,size_t size_sudoku)
{
    int result=-1;
    for (int i=0;i<(int)size_sudoku;i++) {
        if(sudoku[i]=='0'){
            result=i;
            break;}}
    return result;
}

int is_val_in_row(char* sudoku,int pos,int val)
{
    int r=(pos/9);
    r=r*9;
    int result=0;
    for(int i = 0; i < 9 ; i++){
        if( (sudoku[r+i] - '0' ) == val ){
            result=1;
            break;}}
    return result;
}

int is_val_in_col(char* sudoku,int pos,int val)
{
    int r=pos/9;
    r=r*9;
    r=pos-r;
    int result=0;
    for(int i = 0; i < 9 ; i++){
        if( (sudoku[r] - '0' ) == val ){
            result=1;
            break;}
        r=r+9;}
    return result;
}

int is_val_in_square(char* sudoku,int pos,int val)
{
    int r=(pos/27);
    r=r*27;
    int c=(pos/9);
    c=c*9;
    c=pos-c;
    int l = c/3;
    int start= r+l*3;
    int result=0;
    for(int i = 0; i < 3 ; i++){
        for(int k=0; k < 3; k++){
            if( (sudoku[start+i+k*9] - '0' ) == val ){
                result=1;
                break;}
            if(result==1)break;}}
    return result;
}

int is_it_cool(char* sudoku,int pos,int val){
    int result=1;
    if( !(is_val_in_square(sudoku,pos,val)==0 && is_val_in_col(sudoku,pos,val)==0 && is_val_in_row(sudoku,pos,val)==0))result=0;
    return result;
}


int solve( char *buffer, size_t size,struct node** head){
    struct node *current = *head;
    int empty_cell_spot;
    empty_cell_spot = find_empty_cell(buffer,size);
    if(empty_cell_spot==-1) return 1;
    while (current->next != NULL && current->pos !=empty_cell_spot)
        current = current->next;
    for (int num = 0; num < current->size; num++){
        if (is_it_cool(buffer,empty_cell_spot,current->data[num])){
            buffer[empty_cell_spot] = (char)current->data[num]+ '0';
            if (solve(buffer,size,head))  return 1;
            buffer[empty_cell_spot] = (char)0 + '0';}
    }
    return 0;
}

void append(struct node** head_ref, int* new_data,int size, int pos)
{
    struct node* new_node = (struct node*) malloc(sizeof(struct node));
    struct node *last = *head_ref;
    new_node->data = (int*) malloc(sizeof(int)*(unsigned long)size);
    new_node->size=size;
    new_node->pos=pos;
    for(int i=0; i < size ; i++){
        new_node->data[i]  = new_data[i];
    }
    new_node->next = NULL;
    if (*head_ref == NULL)
    {
        *head_ref = new_node;
        return;}
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
    return;
}

void deleteList(struct node** head_ref)
{
    struct node* current = *head_ref;
    struct node* next;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    *head_ref = NULL;
}

void printList(struct node *node)
{
    printf("\nSaved list in structure:\n");
    while (node != NULL)
    {
        for(int i=0; i < node->size; i++){
            printf("%d ", node->data[i]);
        }
        printf("\n");
        node = node->next;
    }
}


int verify(char *buffer,size_t size,struct node** head){
    int array_size=0;
    int pos=0;
    for (int i=0;i<(int)size;i++) {
        if(buffer[i]=='0')
        {
            pos=i;
            for (int num = 1; num <= 9; num++){
                if (is_it_cool(buffer,pos,num)){
                    array_size++;}}
            int *arr=(int*) malloc(sizeof (int)*(unsigned long)array_size);
            int i_arr=0;
            for (int num = 1; num <= 9; num++){
                if (is_it_cool(buffer,pos,num)){
                    arr[i_arr]=num;
                    i_arr++;}}
            append(head,arr,array_size,pos);
            array_size=0;
            i_arr=0;
            pos=0;
        }
    }
    return 0;
}
