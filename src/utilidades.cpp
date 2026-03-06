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
    int c;
    printf("%s", mensaje);
    while (scanf("%d", destino) != 1) {
        printf("Error: Debe ingresar un numero entero.\n");
        while ((c = getchar()) != '\n' && c != EOF);
        printf("%s", mensaje);
    }
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Lee un float con mensaje */
void leerFloat(float *destino, const char mensaje[]) {
    int c;
    printf("%s", mensaje);
    while (scanf("%f", destino) != 1) {
        printf("Error: Debe ingresar un numero decimal.\n");
        while ((c = getchar()) != '\n' && c != EOF);
        printf("%s", mensaje);
    }
    while ((c = getchar()) != '\n' && c != EOF);
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

/* Obtiene el siguiente numero de ID para un prefijo dado en un archivo */
int obtenerSiguienteNumeroID(const char archivo[], const char prefijo[]) {
    FILE *f = fopen(archivo, "r");
    char linea[512];
    char id[20];
    int num, max_num = 0;
    int prefijo_len = (int)strlen(prefijo);

    if (f == NULL) return 1;

    while (fgets(linea, sizeof(linea), f)) {
        if (strlen(linea) > 1) {
            id[0] = '\0';
            sscanf(linea, "%19[^|]", id);
            if (strncmp(id, prefijo, prefijo_len) == 0) {
                num = atoi(id + prefijo_len);
                if (num > max_num) max_num = num;
            }
        }
    }
    fclose(f);
    return max_num + 1;
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

/* Muestra el valor actual de un campo de texto */
void mostrarValorActual(const char campo[], const char valor[]) {
    printf("%s: %s\n", campo, valor);
}

/* Muestra el valor actual de un campo entero */
void mostrarValorActualEntero(const char campo[], int valor) {
    printf("%s: %d\n", campo, valor);
}

/* Muestra el valor actual de un campo flotante */
void mostrarValorActualFloat(const char campo[], float valor) {
    printf("%s: %.2f\n", campo, valor);
}

/* Solicita una confirmacion robusta (s/n) */
int solicitarConfirmacion(const char mensaje[]) {
    char resp[10];
    do {
        leerString(resp, sizeof(resp), mensaje);
        if (resp[0] == 's' || resp[0] == 'S') return 1;
        if (resp[0] == 'n' || resp[0] == 'N') return 0;
        printf("Error: Responda con 's' o 'n'.\n");
    } while (1);
}
