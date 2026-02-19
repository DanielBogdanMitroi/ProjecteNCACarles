#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "usuarios.h"
#include "archivos.h"
#include "utilidades.h"

/* Realiza el proceso de login */
int login(Usuario *usuario) {
    char username[50], password[100];
    Usuario usuarios[MAX_USUARIOS];
    int total, intentos = 0, i;

    while (intentos < 3) {
        printf("\n=============================\n");
        printf("   SISTEMA DE GESTION ESCOLAR\n");
        printf("=============================\n");
        printf("Intento %d de 3\n\n", intentos + 1);
        leerString(username, sizeof(username), "Usuario: ");
        leerString(password, sizeof(password), "Contrasena: ");

        cargarUsuarios(usuarios, &total);
        for (i = 0; i < total; i++) {
            if (strcmp(usuarios[i].username, username) == 0 &&
                strcmp(usuarios[i].password, password) == 0) {
                *usuario = usuarios[i];
                printf("\nBienvenido, %s!\n", usuario->username);
                pausar();
                return 1;
            }
        }
        printf("\nUsuario o contrasena incorrectos.\n");
        intentos++;
    }
    printf("\nNumero maximo de intentos alcanzado.\n");
    return 0;
}

/* Muestra el menu de gestion de usuarios (solo admin) */
void menuUsuarios(Usuario usuario_actual) {
    int opcion;
    if (usuario_actual.tipo != 1) {
        printf("No tiene permisos para esta accion.\n");
        pausar();
        return;
    }
    do {
        limpiarPantalla();
        printf("=== GESTION DE USUARIOS ===\n");
        printf("1. Listar usuarios\n");
        printf("2. Agregar usuario\n");
        printf("3. Cambiar mi contrasena\n");
        printf("0. Volver\n");
        leerEntero(&opcion, "Seleccione: ");
        switch (opcion) {
            case 1: listarUsuarios(); break;
            case 2: agregarUsuario(usuario_actual); break;
            case 3: cambiarPassword(&usuario_actual); break;
            case 0: break;
            default: printf("Opcion invalida.\n"); pausar();
        }
    } while (opcion != 0);
}

/* Lista todos los usuarios */
void listarUsuarios() {
    Usuario usuarios[MAX_USUARIOS];
    int total, i;
    cargarUsuarios(usuarios, &total);
    limpiarPantalla();
    printf("=== LISTA DE USUARIOS ===\n");
    printf("%-20s %-10s %-15s\n", "Username", "Tipo", "ID Referencia");
    printf("%-20s %-10s %-15s\n", "--------", "----", "-------------");
    for (i = 0; i < total; i++) {
        char tipo[15];
        if (usuarios[i].tipo == 1) strcpy(tipo, "Admin");
        else if (usuarios[i].tipo == 2) strcpy(tipo, "Profesor");
        else strcpy(tipo, "Alumno");
        printf("%-20s %-10s %-15s\n", usuarios[i].username, tipo, usuarios[i].id_referencia);
    }
    printf("\nTotal: %d usuarios\n", total);
    pausar();
}

/* Agrega un nuevo usuario */
void agregarUsuario(Usuario usuario_actual) {
    Usuario usuarios[MAX_USUARIOS];
    int total, i;
    Usuario nuevo;
    memset(&nuevo, 0, sizeof(Usuario));

    if (usuario_actual.tipo != 1) {
        printf("No tiene permisos.\n");
        pausar();
        return;
    }

    cargarUsuarios(usuarios, &total);
    leerString(nuevo.username, sizeof(nuevo.username), "Nombre de usuario: ");
    /* Verificar si ya existe */
    for (i = 0; i < total; i++) {
        if (strcmp(usuarios[i].username, nuevo.username) == 0) {
            printf("El usuario ya existe.\n");
            pausar();
            return;
        }
    }
    leerString(nuevo.password, sizeof(nuevo.password), "Contrasena: ");
    leerEntero(&nuevo.tipo, "Tipo (1=Admin, 2=Profesor, 3=Alumno): ");
    leerString(nuevo.id_referencia, sizeof(nuevo.id_referencia), "ID Referencia (ej. PROF001): ");

    usuarios[total] = nuevo;
    total++;
    guardarUsuarios(usuarios, total);
    printf("Usuario creado exitosamente.\n");
    pausar();
}

/* Cambia la contrasena del usuario actual */
void cambiarPassword(Usuario *usuario_actual) {
    Usuario usuarios[MAX_USUARIOS];
    int total, i;
    char nueva_pass[100], confirmar[100];

    leerString(nueva_pass, sizeof(nueva_pass), "Nueva contrasena: ");
    leerString(confirmar, sizeof(confirmar), "Confirmar contrasena: ");

    if (strcmp(nueva_pass, confirmar) != 0) {
        printf("Las contrasenas no coinciden.\n");
        pausar();
        return;
    }

    cargarUsuarios(usuarios, &total);
    for (i = 0; i < total; i++) {
        if (strcmp(usuarios[i].username, usuario_actual->username) == 0) {
            strcpy(usuarios[i].password, nueva_pass);
            strcpy(usuario_actual->password, nueva_pass);
            break;
        }
    }
    guardarUsuarios(usuarios, total);
    printf("Contrasena cambiada exitosamente.\n");
    pausar();
}
