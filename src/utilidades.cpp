#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utilidades.h"

/* Elimina el salto de linea al final de una cadena */
void limpiarSaltoLinea(char str[]) {
    int len = strlen(str);
    if (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
        len--;
    }
    if (len > 0 && str[len-1] == '\r') {
        str[len-1] = '\0';
    }
}

/* Lee una cadena de texto con mensaje */
void leerString(char destino[], int tamano, const char mensaje[]) {
    printf("%s", mensaje);
    fgets(destino, tamano, stdin);
    limpiarSaltoLinea(destino);
}

/* Lee un entero con mensaje */
void leerEntero(int *destino, const char mensaje[]) {
    printf("%s", mensaje);
    if (scanf("%d", destino) != 1) {
        *destino = 0;
    }
    getchar();
}

/* Lee un float con mensaje */
void leerFloat(float *destino, const char mensaje[]) {
    printf("%s", mensaje);
    if (scanf("%f", destino) != 1) {
        *destino = 0.0f;
    }
    getchar();
}

/* Limpia la pantalla */
void limpiarPantalla() {
    printf("\033[2J\033[H");
}

/* Pausa hasta que el usuario presione Enter */
void pausar() {
    printf("\nPresione Enter para continuar...");
    getchar();
}

/* Genera un ID con prefijo y numero formateado con ceros */
void generarID(char resultado[], const char prefijo[], int numero) {
    sprintf(resultado, "%s%03d", prefijo, numero);
}

/* Convierte una cadena a mayusculas */
void convertirAMayusculas(char str[]) {
    int i;
    for (i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

/* Convierte una cadena a minusculas */
void convertirAMinusculas(char str[]) {
    int i;
    for (i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

/* Compara dos cadenas, retorna 0 si son iguales */
int compararStrings(const char str1[], const char str2[]) {
    return strcmp(str1, str2);
}
