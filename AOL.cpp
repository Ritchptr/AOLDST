#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct tree
{
    char slank[51];
    char meaning[101];
    struct tree *right, *left;
};

struct tree* root = NULL;

struct tree* make(char slank[51], char meaning[101]){
    struct tree* baru = (struct tree*)malloc(sizeof(struct tree));
    strcpy(baru->slank, slank);
    strcpy(baru->meaning, meaning);
    baru->left = NULL;
    baru->right = NULL;
    return baru;
}

struct tree* insert(struct tree* root, char slank[51], char meaning[101]){
    
    if(root == NULL){
        root = make(slank, meaning);
    }else if(strcmp(slank, root->slank) <= 0){
        root->left = insert(root->left, slank, meaning);
    }else{
        root->right = insert(root->right, slank, meaning);
    }
    return root;
}

void readfile(){
    FILE *fl;
    fl = fopen("Dictionary.txt", "r");

    char slank[51];
    char meaning[101];

    while(!feof(fl)){
        fscanf(fl, "%[^-]-%[^\n]\n",slank, meaning);
        root = insert(root, slank, meaning);
    }
    fclose(fl);
}

void search(struct tree* root, char key[], int *flag){
    if(root == NULL){
        return;
    }else if(strcmp(root->slank, key) == 0){
        printf("Slang Word   : %s\nDescription  : %s\n\n", root->slank, root->meaning);
        *flag = 1;
        return;
    }else if(strcmp(key, root->slank) < 0){
        search(root->left, key, flag);
    }else{
        search(root->right, key, flag);
    }
}

void prefix(struct tree* root, char key[], int* flag){
    
    if(root == NULL){
        return;
    }

    prefix(root->left, key, flag);

    if(strncmp(root->slank, key, strlen(key)) == 0){
        printf("Slang Word   : %s\nDescription  : %s\n\n", root->slank, root->meaning);
        *flag = 1;
    }

    prefix(root->right, key, flag);
}

void Inorder(struct tree* root){
    if(root == NULL)return;
    Inorder(root->left);
    printf("Slang Word   : %s\nDescription  : %s\n\n", root->slank, root->meaning);
    Inorder(root->right);
}

bool strchk(char key[]){

    if(strchr(key, ' ') == NULL && strlen(key) > 1) return true;
    else return false;
}

bool countword(char key[101]){
    char* cpy = strdup(key);
    char* token = strtok(cpy, " ");
    int count = 0;
    while(token != NULL){
        count++;
        token = strtok(NULL, " ");
    }
    free(cpy);
    if(count >= 2) return true;
    else return false;
}

void enter(){
    printf("Press Enter To Continue......\n");
    getchar();
    getchar();
}

void menu(){
    printf("Menu :\n");
    printf("    1. Release a new slang word\n");
    printf("    2. Search a slang word\n");
    printf("    3. View all slang word starting with a certain prefix word\n");
    printf("    4. View all slang word\n");
    printf("    5. Exit\n\n");
    printf("Please select number of menu : ");
}

void menu1(){

    char slank[51];
    char meaning[101];
    while(1){
        printf("Input a new slang word [Must be more than 1 characters and contains no space]: ");
        scanf(" %[^\n]", slank);
        if(strchk(slank)) break;
        else continue;
    }

    while(1){
        printf("Input a new slang word [Must be more than 2 words]: ");
        scanf(" %[^\n]", meaning);
        if(countword(meaning)) break;
        else continue;
    }

    insert(root, slank, meaning);

    //Menambahkan slang ke Dictionary.txt
    FILE *fl;
    fl = fopen("Dictionary.txt", "a");
    fprintf(fl, "\n%s-%s", slank, meaning);
    fclose(fl);

    puts("");
    printf("Successfully updated a slang word.\n");
    enter();
}

void menu2 (){
    char key[51];
    while(1){
        printf("Input a slang word to be searched [Must be more than 1 characters and contains no space]:");
        scanf(" %[^\n]", key);
        if(strchk(key)) break;
        else continue;
    }
    puts("");
    int flag = 0;
    search(root, key, &flag);
    if(!flag)printf("There is no prefix \"%s\" in the dictionary.\n", key);
    enter();
}

void menu3 (){
    char key[51];
    printf("Input a prefix to be searched:");
    scanf(" %s", key);

    puts("");

    int flag = 0;
    prefix(root, key, &flag);

    if(!flag)printf("There is no prefix \"%s\" in the dictionary.\n", key);
    enter();
}



int main(){

    readfile();     //Membaca file dan memasukan ke dalam tree
    
    while(1){
        char flag;
        menu();
        scanf(" %c", &flag);
        
        switch (flag)
        {
        case '1':
            menu1();
            continue;
        
        case '2' :
            menu2();
            continue;
        
        case '3' :
            menu3();
            continue;

        case '4' :
            Inorder(root);
            enter();
            continue;

        case '5' :
            printf("Thank You\n");
            return 0;

        default:
            printf("Please Input correct number [1-5]\n\n");
            enter();
            continue;
        }
    }
    return 0;
}