#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#define wait(t) Sleep(t * 1000)
#define wipe system("cls")
#elif unix
#include <unistd.h>
#define wait(t) sleep(t)
#define wipe system("clear")
#endif

typedef struct {
    char username[20];
    char password[20];
    char name[20];
    char type[1];
} Login;

const Login default_login = {.username = "", .password = ""};

Login inputuser = default_login;
Login fileuser = default_login;

void encrypt(char string[20]) {
    int c = 0;
    while (c < strlen(string)) {
        if ('a' <= string[c] && string[c] <= 'm')
            string[c] += 13;
        else if ('n' <= string[c] && string[c] <= 'z')
            string[c] -= 13;
        else if ('A' <= string[c] && string[c] <= 'M')
            string[c] += 13;
        else if ('N' <= string[c] && string[c] <= 'Z')
            string[c] -= 13;
        else if ('0' <= string[c] && string[c] <= '4')
            string[c] += 5;
        else if ('5' <= string[c] && string[c] <= '9')
            string[c] -= 5;
        c++;
    }
}

void entrar() {
    printf("\nDigite o nome de usuario.\n");
    scanf("%s", &inputuser.username);
    if (strlen(inputuser.username) > 15) {
        printf("Usuario excede o limite de caracteres.\n");
        wait(2);
    } else {
        printf("Digite a senha.\n");
        scanf("%s", &inputuser.password);
        if (strlen(inputuser.password) > 15) {
            printf("Senha excede o limite de caracteres.\n");
            wait(2);
        } else {
            FILE *db;
            db = fopen("db", "r+");
            if (db == NULL) {
                printf("Nao existem usuarios cadastrados.\n");
                wait(2);
            }
            int success = 0;
            encrypt(inputuser.username);
            encrypt(inputuser.password);
            while (fscanf(db, "%s", &fileuser) != EOF) {
                if (strcmp(inputuser.username, fileuser.username) == 0) {
                    if (strcmp(inputuser.password, fileuser.password) == 0) {
                        success = 1;
                        break;
                    }
                }
            }
            if (success == 1) {
                encrypt(fileuser.name);
                printf("Bem vindo(a), %s\n", fileuser.name);
                wait(2);

                /*

                IMPLEMENTAÇÃO DO PROGRAMA

                */

            } else if (success == 0) {
                printf("Falha no login.\n");
            } else
                printf("Falha no sistema.\n");
            wait(2);
        }
    }
}

void cadastrar() {
    Login inputuser = default_login;
    Login fileuser = default_login;

    FILE *db;
    db = fopen("db", "a+");
    int valid = 1;
    printf("Digite o nome de usuario desejado. Limite de 15 caracteres.\n");
    scanf("%s", &inputuser.username);
    encrypt(inputuser.username);
    if (strlen(inputuser.username) > 15) {
        printf("Usuario excede o limite de caracteres.\n");
        wait(2);
        return;
    }
    while (fscanf(db, "%s", &fileuser) != EOF) {
        while (strcmp(inputuser.username, fileuser.username) == 0) {
            printf(
                "Ja existe uma conta com esse nome de usuario, tente outro.");
            wait(2);
            return;
        }
    }
    printf("Digite a senha desejada. Limite de 15 caracteres.\n");
    scanf("%s", &inputuser.password);
    encrypt(inputuser.password);
    if (strlen(inputuser.password) > 15) {
        printf("Senha excede o limite de caracteres.\n");
        wait(2);
    } else {
        printf("Digite um nome para esta conta. Limite de 15 caracteres.\n");
        scanf("%s", &inputuser.name);
        encrypt(inputuser.name);
        if (strlen(inputuser.name) > 15) {
            printf("Nome excede o limite de caracteres.\n");
            wait(2);
        } else {
            printf(
                "Atribua uma funcao a conta.\n[1] Gerencia\n[2] "
                "Funcionario\n[3] Cliente\n");
            scanf("%s", &inputuser.type);
            if (inputuser.type[0] == '1')
                valid = 1;
            else if (inputuser.type[0] == '2')
                valid = 2;
            else if (inputuser.type[0] == '3')
                valid = 3;
            else {
                printf("Opcao invalida, tente novamente.");
                valid = 0;
            }
            if (valid != 0) {
                int erro = (fwrite(&inputuser, sizeof(Login), 1, db) &&
                            fwrite("\n", sizeof(char), 1, db));
                if (erro != 0)
                    printf("Salvo com sucesso!\n");
                else
                    printf("Erro no salvamento, tente novamente.\n");
            }
        }
    }
    fclose(db);
    wait(2);
    return;
}

int main() {
    FILE *db;
    fopen("db", "a");
    while (1) {
        wipe;
        printf("Digite uma opcao\n1 - Login\n2 - Cadastro\n3 - Fechar\n");
        int opt = 0;
        scanf("%i", &opt);
        if (opt == 1) {
            entrar();
        } else if (opt == 2) {
            cadastrar();
        } else if (opt == 3) {
            printf("Fechando...\n");
            break;
        } else {
            printf("Opcao invalida.\n");
            wait(2);
        }
    }
}