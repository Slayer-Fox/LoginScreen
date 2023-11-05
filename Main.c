#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#define getchar() getch()
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

#define default_login {.username = "", .password = "", .name = "", .type = "0"};

Login input_user = default_login;
Login file_user = default_login;
Login prime_user = default_login;

void getty(){
  int c;
  while ((c = fgetc(stdin)) != '\n' && c != EOF) {
    printf("%d", c);
  };
}

void get_user_input(char * buf, int size) {
  int c;
  int i = 0;
  while ((c = fgetc(stdin)) != EOF) {
    if (c == '\n') {
      break;
    } 
    if (i + 1 < size || (size == 1 && i == 0)) {
      buf[i++] = c;
    }
  }
  if (size > 1) {
    buf[i] = '\0';
  }
}

void encrypt(char string[20]) {
    size_t c = 0;
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

void relatorio(){
    if (*prime_user.type == '1'){
        FILE *db = fopen("db", "rb");
        printf("________________________________________________________________________________\n");
        while (fread(&file_user, sizeof(Login), 1, db) != 0) {
            encrypt(file_user.name);
            encrypt(file_user.username);
            printf("| Nome: %15s | Usuario: %15s | ", file_user.name, file_user.username);
            if(*file_user.type == '1'){
                printf("Tipo de conta:     Gerente|\n");
            }
            else if(*file_user.type == '2'){
                printf("Tipo de conta: Funcionario|\n");
            }
            else if(*file_user.type == '3'){
                printf("Tipo de conta:     Cliente|\n");
            }
            else{
                printf("%s\n", file_user.type);
            }
            printf("|_______________________|__________________________|___________________________|\n");
        }
    }
    else printf("Voce nao possui acesso a essa funcao.\n");
}

void deletar(){
    printf("Digite o nome de usuario da conta que sera deletada: ");
    scanf("%s", input_user.username);
    FILE *db = fopen("db", "r+");
    FILE *temp_db = fopen("temp_db", "w");
    int found = 0;
    while (fread(&file_user, sizeof(Login), 1, db) != 0) {
        encrypt(file_user.username);
        if (strcmp(file_user.username, input_user.username) == 0) found = 1;
        else fwrite(&file_user, sizeof(Login), 1, temp_db);
    }
    fclose(db);
    fclose(temp_db);
    remove("db");
    rename("temp_db", "db");
    if (found) printf("Conta deletada com sucesso!\n");
    else printf("A conta com o nome de usuario especificado nao foi encontrada.\n");
}

void menu(){
    char menuopt;
    while(1){
        wipe;
        printf("Digite a opcao desejada:\n1 - Relatorio de usuarios cadastrados\n"
        "2 - Deletar por nome de usuario\n3 - Sair\n");
        get_user_input(&menuopt, 1);
        if(menuopt == '1'){
            relatorio();
        }
        else if (menuopt == '2') {
            deletar();
        }
        else if(menuopt == '3'){
            printf("Saindo...\n");
            break;
        }
        else{
            printf("Selecione uma opcao valida\n");
        }
        getty();
    }
}

void entrar() {
    printf("\nDigite o nome de usuario.\n");
    get_user_input(input_user.username, 16);
    if (strlen(input_user.username) > 15) {
        printf("Usuario excede o limite de caracteres.\n");
        wait(2);
        return;
    }
    printf("Digite a senha.\n");
    get_user_input(input_user.password, 16);
    if (strlen(input_user.password) > 15) {
        printf("Senha excede o limite de caracteres.\n");
        wait(2);
        return;
    }
    FILE *db;
    db = fopen("db", "r+");
    if (db == NULL) {
        printf("Nao foi possivel encontrar o banco de dados.\n");
        wait(2);
        db = fopen("db", "a");
        return;
    }
    int success = 0;
    encrypt(input_user.username);
    encrypt(input_user.password);
    while (fread(&file_user, sizeof(Login), 1, db) != 0) {
        if (strcmp(input_user.username, file_user.username) == 0) {
            if (strcmp(input_user.password, file_user.password) == 0) {
                success = 1;
                break;
            }
        }
    }
    if (success == 1) {
        encrypt(file_user.name);
        printf("Bem vindo(a), %s\n", file_user.name);
        wait(1);
        prime_user = file_user;
        encrypt(prime_user.username);
        encrypt(prime_user.password);
        menu();
        wait(1);
        return;
    } 
    else if (success == 0) {
        printf("Falha no login.\n");
    }
    getty();
}

void cadastrar() {
    Login input_user = default_login;
    Login file_user = default_login;
    FILE *db;
    db = fopen("db", "a+");
    int valid = 1;
    printf("Digite o nome de usuario desejado. Limite de 15 caracteres.\n");
    get_user_input(input_user.username, 16);
    encrypt(input_user.username);
    if (strlen(input_user.username) > 15) {
        printf("Usuario excede o limite de caracteres.\n");
        wait(2);
        return;
    }
    while (fread(&file_user, sizeof(Login), 1, db) != 0) {
        while (strcmp(input_user.username, file_user.username) == 0) {
            printf("Ja existe uma conta com esse nome de usuario.\n");
            wait(2);
            return;
        }
    }
    printf("Digite a senha desejada. Limite de 15 caracteres.\n");
    get_user_input(input_user.password, 16);
    encrypt(input_user.password);
    if (strlen(input_user.password) > 15) {
        printf("Senha excede o limite de caracteres.\n");
        wait(2);
        return;
    }
    printf("Digite um nome para esta conta. Limite de 15 caracteres.\n");
    get_user_input(input_user.name, 16);
    encrypt(input_user.name);
    if (strlen(input_user.name) > 15) {
        printf("Nome excede o limite de caracteres.\n");
        wait(2);
        return;
    }
    printf(
        "Atribua uma funcao a conta.\n[1] Gerencia\n"
        "[2] Funcionario\n[3] Cliente\n");
    get_user_input(input_user.type, 2);
    if (input_user.type[0] == '1')
        valid = 1;
    else if (input_user.type[0] == '2')
        valid = 2;
    else if (input_user.type[0] == '3')
        valid = 3;
    else {
        printf("Opcao invalida, tente novamente.\n");
        valid = 0;
    }
    if (valid != 0) {
        int erro = fwrite(&input_user, sizeof(Login), 1, db);
        if (erro != 0)
            printf("Salvo com sucesso!\n");
        else
            printf("Erro no salvamento, tente novamente.\n");
    }
    fclose(db);
    getty();
}

int main() {
    while (1) {
        wipe;
        printf("Digite uma opcao\n1 - Login\n2 - Cadastro\n3 - Fechar\n");
        char opt;
        get_user_input(&opt, 1);
        if (opt == '1') {
            entrar();
        } else if (opt == '2') {
            cadastrar();
        } else if (opt == '3') {
            printf("Fechando...\n");
            break;
        } else {
            printf("Opcao invalida.\n");
            getty();
        }
    }
}