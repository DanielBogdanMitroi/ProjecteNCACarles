#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "validaciones.h"

/* Valida formato de email */
int validarEmail(const char email[]) {
    int i, arroba = -1, ultimo_punto = -1;
    int len = strlen(email);
    int count_arroba = 0;

    if (len < 5) return 0;
    if (email[0] == ' ' || email[len-1] == ' ') return 0;
    if (!isalnum(email[0]) || email[len-1] == '.') return 0;

    for (i = 0; i < len; i++) {
        if (!isalnum(email[i]) && email[i] != '@' && email[i] != '.' &&
            email[i] != '_' && email[i] != '-') {
            return 0;
        }
        if (email[i] == '@') {
            count_arroba++;
            arroba = i;
        }
        if (email[i] == '.') {
            ultimo_punto = i;
        }
        if (i > 0 && ((email[i] == '.' && email[i-1] == '.') ||
                      (email[i] == '.' && email[i-1] == '@') ||
                      (email[i] == '@' && email[i-1] == '.'))) {
            return 0;
        }
    }
    if (count_arroba != 1) return 0;
    if (arroba <= 0 || arroba >= len - 2) return 0;
    if (ultimo_punto <= arroba || ultimo_punto >= len - 1) return 0;
    if (ultimo_punto - arroba < 2) return 0;
    return 1;
}

/* Valida formato de fecha YYYY-MM-DD */
int validarFecha(const char fecha[]) {
    int anio, mes, dia;
    int dias_por_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int es_bisiesto;

    if (strlen(fecha) != 10) return 0;
    if (fecha[4] != '-' || fecha[7] != '-') return 0;
    if (sscanf(fecha, "%d-%d-%d", &anio, &mes, &dia) != 3) return 0;
    if (anio < 1900 || anio > 2100) return 0;
    if (mes < 1 || mes > 12) return 0;
    if (dia < 1 || dia > 31) return 0;
    es_bisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
    if (es_bisiesto && mes == 2) {
        dias_por_mes[1] = 29;
    }
    if (dia > dias_por_mes[mes - 1]) return 0;
    return 1;
}

/* Valida formato de telefono */
int validarTelefono(const char telefono[]) {
    int i;
    int len = strlen(telefono);
    int cuenta_digitos = 0;

    if (len < 9 || len > 15) return 0;
    for (i = 0; i < len; i++) {
        if (isdigit(telefono[i])) {
            cuenta_digitos++;
        } else if (telefono[i] != '+' && telefono[i] != '-' && telefono[i] != ' ') {
            return 0;
        }
        if (telefono[i] == '+' && i != 0) return 0;
    }
    if (cuenta_digitos < 9) return 0;
    return 1;
}

/* Valida que una nota este entre 0 y 10 */
int validarNota(float nota) {
    return (nota >= 0.0f && nota <= 10.0f) ? 1 : 0;
}

/* Valida que los creditos sean positivos */
int validarCreditos(int creditos) {
    return (creditos > 0 && creditos <= 20) ? 1 : 0;
}

/* Valida formato de hora HH:MM */
int validarHora(const char hora[]) {
    int h, m;
    if (strlen(hora) != 5) return 0;
    if (sscanf(hora, "%d:%d", &h, &m) != 2) return 0;
    if (h < 0 || h > 23) return 0;
    if (m < 0 || m > 59) return 0;
    return 1;
}

/* Valida que la contrasena tenga al menos 6 caracteres */
int validarPassword(const char password[]) {
    return (strlen(password) >= 6) ? 1 : 0;
}

/* Verifica si un ID existe en un archivo */
int existeID(const char archivo[], const char id[]) {
    FILE *f = fopen(archivo, "r");
    char linea[512];
    char id_linea[20];
    if (f == NULL) return 0;
    while (fgets(linea, sizeof(linea), f)) {
        sscanf(linea, "%19[^|]", id_linea);
        if (strcmp(id_linea, id) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

/* Valida que una cadena no este vacia y tenga longitud minima */
int validarCadenaNoVacia(const char cadena[], int longitud_minima) {
    int len = strlen(cadena);
    int inicio = 0, fin = len - 1;

    while (inicio < len && (cadena[inicio] == ' ' || cadena[inicio] == '\t')) {
        inicio++;
    }
    while (fin >= 0 && (cadena[fin] == ' ' || cadena[fin] == '\t' || cadena[fin] == '\n')) {
        fin--;
    }
    return (fin - inicio + 1 >= longitud_minima) ? 1 : 0;
}

/* Verifica si un email ya existe en un archivo (excluyendo el id_actual) */
int emailYaExiste(const char email[], const char archivo[], const char id_actual[]) {
    FILE *f = fopen(archivo, "r");
    char linea[512];
    char id_linea[20], email_linea[100];

    if (f == NULL) return 0;
    while (fgets(linea, sizeof(linea), f)) {
        id_linea[0] = '\0';
        email_linea[0] = '\0';
        sscanf(linea, "%19[^|]|%*[^|]|%*[^|]|%99[^|]", id_linea, email_linea);
        if (strcmp(email_linea, email) == 0 && strcmp(id_linea, id_actual) != 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}
