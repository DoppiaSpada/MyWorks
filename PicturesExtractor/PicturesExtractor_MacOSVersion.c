#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

#define NUMFILE 200
#define DIMNOME 50

// 26/03/2024

/*  PicturesExtractor è un mini programma capace di estrarre casualmente un numero scelto di file da una directory sorgente, da copiare a seguire nella directory destinazione. */

//MOLTO UTILE SE SEI UNA PERSONA INDECISA!

//  INVOCAZIONE
//  ./PicturesExtractor dir_source dir_dest n_file_da_estrarre
//  dir_source DEVE contenere l'ultimo carattere '/', dir_dest senza
//  Su MacOS dir_dest verrà accettato anche come file generico e non propriamente come directory.
//  Ciò può causare malfunzionamenti.

int pid = -1;
int pids[NUMFILE];
int n = -1;

void child(char *src, char *dest){
    FILE *file;
    int i = 0;
    char list1[NUMFILE][DIMNOME];
    char list2[NUMFILE][DIMNOME];
    int randomNs[NUMFILE];
    char sys[DIMNOME] = "";

    srand(time(NULL));
    
    strcat(sys, "ls ");
    strcat(sys, src);
    strcat(sys, " > list.txt");
    if(system(sys) != 0){fprintf(stderr, "ls FALLITA\n"); exit(1);};
    
    file = fopen("list.txt", "r");
    while(fscanf(file, "%s", list1[i]) != EOF){
        printf("nome file: %s\n", list1[i]);
        i++;
    }
    
    if(i < n){fprintf(stderr, "Numero di file richiesti eccessivo\n"); exit(1);}
    
    int flag = 0;
    for(int k = 0; k < n; k++){
        randomNs[k] = rand() % i;
        flag = 1;
        while(flag){
            flag = 0;
            for(int j = 0; j < k; j++){
                if(randomNs[k] == randomNs[j]){
                    randomNs[k] = rand() % i;
                    flag = 1;
                    break;
                }
            }
        }
    }
    
    for(int k = 0; k < n; k++){
        strcat(list2[k], src);
        strcat(list2[k],list1[randomNs[k]]);
    }
    
    for(int k = 0; k < n; k++){
        pids[k] = fork();
        if(pids[k] == 0){
            execlp("cp", "cp", list2[k], dest, (char *)0);
            
            perror("Exec FALLITA: ");
            exit(1);
        }
    }
    
    int status = -1;
    int err_code = 0;
    
    for(int k = 0; k < n; k++){
        pid = wait(&status);
    
        if(pid < 0){perror("Wait FALLITA: "); exit(1);}
        
        if(WIFEXITED(status) && WEXITSTATUS(status) == 0){
            printf("Processi terminati con successo %d\n", k);
            continue;
        }
        
        fprintf(stderr, "Processo %d terminato con ANOMALIA\n", pid);
        err_code = 1;
    }
    
    exit(err_code);
}

int main(int argc, char **argv){
    
    if(argc != 4){fprintf(stderr, "Numero argomenti incorretto\n");}
    
    n = atoi(argv[3]);
    pid = fork();
    if(pid == 0){
        child(argv[1], argv[2]);
        exit(1);
    }
    
    int status = -1;
    
    pid = wait(&status);
    if(pid < 0){perror("Wait FALLITA: "); exit(1);}
    
    if(WIFEXITED(status) && WEXITSTATUS(status) == 0){
        printf("Programma terminato CORRETTAMENTE\n");
        exit(0);
    }
    
    fprintf(stderr, "Programma terminato con ANOMALIA\n");
    exit(1);
    
    return 0;
}
