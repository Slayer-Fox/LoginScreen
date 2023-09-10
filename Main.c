#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#define wait(t) Sleep(t * 1000)
#define wipe system("cls")
#elif unix
#include <unistd.h>
#define wait(t) sleep(t)
#define wipe system("clear")
#endif

typedef struct{
char username[20];
char password[20];
char name[20];
int type;
}Login;

const Login default_login = {.username = "", .password = ""};

char string[20];
int crypt(char string[20]){
    int c = 0;
    while(c < strlen(string)){
        if('a' <= string[c] && string[c] <= 'm'){string[c] += 13;}
        else if('n' <= string[c] && string[c] <= 'z'){string[c] -= 13;}
        else if('A' <= string[c] && string[c] <= 'M'){string[c] += 13;}
        else if('N' <= string[c] && string[c] <= 'Z'){string[c] -= 13;}
        else if('0' <= string[c] && string[c] <= '4'){string[c] += 5;}
        else if('5' <= string[c] && string[c] <= '9'){string[c] -= 5;}
        c++;
    }
}

int main(){
    FILE *db;
    fopen("db.txt", "a");
    Login inputuser = default_login;
    Login fileuser = default_login;
    while(1){
        wipe;
        printf("Digite uma opcao\n1 - Login\n2 - Cadastro\n3 - Fechar\n");
        int opt = 0;
        scanf("%i", &opt);
        // Opção Login
        if(opt == 1){
            printf("\nDigite o nome de usuario.\n");
            scanf("%s", &inputuser.username);
            if(strlen(inputuser.username) > 15){
                printf("Usuario excede o limite de caracteres.\n");
                wait(2);
            }
            else{
                printf("Digite a senha.\n");
                scanf("%s", &inputuser.password);
                if(strlen(inputuser.password) > 15){
                    printf("Senha excede o limite de caracteres.\n");
                    wait(2);
                }
                else{
                    FILE *db;
                    db = fopen("db.txt", "r+");
                    if(db == NULL){
                        printf("Nao existem usuarios cadastrados.\n");
                        wait(2);
                    }
                    int success = 0;
                    while(fscanf(db, "%s", &fileuser) != EOF){
                        crypt(inputuser.username);
                        crypt(inputuser.password);
                        if(strcmp(inputuser.username, fileuser.username) == 0){
                            if(strcmp(inputuser.password, fileuser.password) == 0){
                                success = 1;
                                break;
                            }
                            else{printf("Falha no login.\n");
                            wait(2);
                            }
                        }
                    }
                    if(success == 1){
                        printf("Bem vindo(a), %s\n", fileuser.name);
                        wait(2);

                        /*
                        
                        IMPLEMENTAÇÃO DO PROGRAMA
                        
                        */
                    
                    }
                    else if(success == 0){
                        printf("Falha no login.\n");
                    }
                    else printf("Falha no sistema.\n");
                    wait(2);
                }
            }
        }
        // Opção Cadastro
        else if(opt == 2){
            int success = 0;
            printf("Digite o nome de usuario desejado. Limite de 15 caracteres.\n");
            scanf("%s", &inputuser.username);
            if(strlen(inputuser.username) > 15){
                printf("Usuario excede o limite de caracteres.\n");
                wait(2);
            }
            else{
                printf("Digite a senha desejada. Limite de 15 caracteres.\n");
                scanf("%s", &inputuser.password);
                if(strlen(inputuser.password) > 15){
                    printf("Senha excede o limite de caracteres.\n");
                    wait(2);
                }
                else{
                    FILE *db;
                    db = fopen("db.txt", "a+");
                    int repeat = 0;
                    while(fscanf(db, "%s", &fileuser) != EOF){
                        crypt(inputuser.username);
                        crypt(inputuser.password);
                    }
                    printf("Digite um nome para esta conta. Limite de 15 caracteres.\n");
                    scanf("%s", &inputuser.name);
                    if(strlen(inputuser.name) > 15){
                        printf("Nome excede o limite de caracteres.\n");
                        wait(2);
                    }
                    else{
                        printf("Atribua uma funcao a conta.\n[1] Gerencia\n[2] Funcionario\n[3] Cliente\n");
                        scanf("%d", &inputuser.type);
                        switch (inputuser.type){
                            case 1:
                                inputuser.type = 1;
                                break;
                            case 2:
                                inputuser.type = 2;
                                break;
                            case 3:
                                inputuser.type = 3;
                                break;
                            default:
                                printf("Valor incorreto, repita o processo com um valor valido.\n%s", &inputuser.type);
                                inputuser.type = 0;
                        }
                        if(inputuser.type == 0) break;
                        while(repeat == 0){
                            crypt(inputuser.username);
                            crypt(inputuser.password);
                        }
                    fclose(db);
                    wait(2);
                    }
                }
            }
            wait(2);
        }
        // Opção Fechar
        else if(opt ==3){
            printf("Fechando...\n");
            break;
        }
        // Opção inexistente
        else {printf("Opcao invalida.\n");
            wait(2);
        }
    }
}