#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define MAX_USUARIOS 5

// Estrutura para armazenar os dados do usuario
typedef struct {
    char nome[50];
    int senha_hash;  // Armazenara o hash da senha, nao a senha em texto claro
    int permissao;   // 0: usuario comum, 1: administrador
} Usuario;

// Lista de usuarios do sistema
Usuario usuarios[MAX_USUARIOS];

// Funcao para calcular o hash simples
int calcular_hash(const char *senha) {
    int hash = 0;
    while (*senha) {
        hash += (int)(*senha);  
        senha++;
    }
    return hash;
}

// Funcao para validar a senha
int validar_senha(const char *senha) {
    int tem_maiuscula = 0, tem_minuscula = 0, tem_numero = 0, tem_caractespec = 0;
    int tamanho = strlen(senha);

    if (tamanho < 8) {
        printf("A senha deve ter pelo menos 8 caracteres\n");
        return 0;  // Falhou na validacao
    }

    for (int i = 0; i < tamanho; i++) {
        if (isupper(senha[i])) tem_maiuscula = 1;
        if (islower(senha[i])) tem_minuscula = 1;
        if (isdigit(senha[i])) tem_numero = 1;
        if (ispunct(senha[i])) tem_caractespec = 1;
    }

    if (tem_maiuscula && tem_minuscula && tem_numero && tem_caractespec) {
        return 1;  // Senha valida
    } else {
        printf("A senha deve conter:\n");
        if (!tem_maiuscula) printf(" - Pelo menos uma letra maiuscula\n");
        if (!tem_minuscula) printf(" - Pelo menos uma letra minuscula\n");
        if (!tem_numero) printf(" - Pelo menos um numero\n");
        if (!tem_caractespec) printf(" - Pelo menos um caractere especial\n");
        return 0;  // Senha invalida
    }
}

// Funcao para cadastrar um novo usuario
int cadastrar_usuario(char *nome, char *senha, int permissao) {
    for (int i = 0; i < MAX_USUARIOS; i++) {
        if (usuarios[i].nome[0] == '\0') {  // Verifica se ha espaco para cadastrar o usuario
            strcpy(usuarios[i].nome, nome);
            usuarios[i].senha_hash = calcular_hash(senha);  // Armazena o hash da senha
            usuarios[i].permissao = permissao;

            // Salva no arquivo de texto
            FILE *arquivo = fopen("usuarios.txt", "a");
            if (arquivo == NULL) {
                perror("Erro ao abrir o arquivo");
                return 0;
            }

            fprintf(arquivo, "Nome: %s\nPermissao: %d\n", nome, permissao);
            fclose(arquivo);

            printf("Usuario %s cadastrado com sucesso\n", nome);
            return 1;
        }
    }
    printf("Limite de usuarios atingido\n");
    return 0;
}

// Funcao para autenticar o usuario
int autenticar_usuario(char *nome, char *senha) {
    int senha_hash = calcular_hash(senha);  // Calcula o hash da senha fornecida
    for (int i = 0; i < MAX_USUARIOS; i++) {
        if (strcmp(usuarios[i].nome, nome) == 0 && usuarios[i].senha_hash == senha_hash) {
            return usuarios[i].permissao;
        }
    }
    return -1;  // Usuario nao encontrado
}

// Funcao para exibir menu de administrador
void menu_administrador() {
    printf("Menu Administrador\n");
    printf("1. Cadastrar Usuario\n");
    printf("2. Visualizar usuarios\n");
    printf("3. Sair\n");
}

// Funcao para exibir menu de usuario comum
void menu_usuario() {
    printf("Menu Usuario\n");
    printf("1. Acessar relatorios\n");
    printf("2. Sair\n");
}

// Funcao principal que controla a autenticacao e acesso ao sistema
int main() {
    char nome[50], senha[50];
    int permissao, opcao;

    // Cadastro de usuarios (apenas um exemplo de preenchimento)
    cadastrar_usuario("marcos", "Marcos@123", 1);  // Marcos, como administrador
    cadastrar_usuario("matheus", "Matheus@123", 0);  // Matheus, como usuario comum
    cadastrar_usuario("pedro", "Pedro@123", 0);  // Pedro, como usuario comum

    printf("Digite seu nome de usuario: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;  // Remover o newline

    printf("Digite sua senha: ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = 0;  // Remover o newline

    // Autentica o usuario e verifica permissoes
    permissao = autenticar_usuario(nome, senha);
    if (permissao == -1) {
        printf("Usuario ou senha invalidos\n");
        return 0;
    }

    // Menu baseado na permissao do usuario
    if (permissao == 1) {
        // Menu para administrador
        while (1) {
            menu_administrador();
            printf("Escolha uma opcao: ");
            scanf("%d", &opcao);

            if (opcao == 1) {
                char novo_nome[50], nova_senha[50];
                int nova_permissao;
                printf("Digite o nome do novo usuario: ");
                scanf("%s", novo_nome);
                printf("Digite a senha do novo usuario: ");
                scanf("%s", nova_senha);
                printf("Digite a permissao (0 para usuario comum, 1 para administrador): ");
                scanf("%d", &nova_permissao);
                cadastrar_usuario(novo_nome, nova_senha, nova_permissao);
            } else if (opcao == 2) {
                printf("Usuarios cadastrados:\n");
                for (int i = 0; i < MAX_USUARIOS; i++) {
                    if (usuarios[i].nome[0] != '\0') {
                        printf("Usuario: %s, Permissao: %d\n", usuarios[i].nome, usuarios[i].permissao);
                    }
                }
            } else if (opcao == 3) {
                printf("Saindo...\n");
                break;
            } else {
                printf("Opcao invalida\n");
            }
        }
    } else {
        // Menu para usuario comum
        while (1) {
            menu_usuario();
            printf("Escolha uma opcao: ");
            scanf("%d", &opcao);

            if (opcao == 1) {
                printf("Acessando relatorios...\n");
            } else if (opcao == 2) {
                printf("Saindo...\n");
                break;
            } else {
                printf("Opcao invalida\n");
            }
        }
    }

    return 0;
}

