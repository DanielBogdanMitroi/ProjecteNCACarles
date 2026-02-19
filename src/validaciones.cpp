#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "validaciones.h"

/* Valida formato de email */
int validarEmail(const char email[]) {
    int i, arroba = 0, punto = 0;
    int len = strlen(email);
    if (len < 5) return 0;
    for (i = 0; i < len; i++) {
        if (email[i] == '@') arroba++;
        if (email[i] == '.' && arroba > 0) punto++;
    }
    return (arroba == 1 && punto >= 1) ? 1 : 0;
}

/* Valida formato de fecha YYYY-MM-DD */
int validarFecha(const char fecha[]) {
    int anio, mes, dia;
    if (strlen(fecha) != 10) return 0;
    if (sscanf(fecha, "%d-%d-%d", &anio, &mes, &dia) != 3) return 0;
    if (mes < 1 || mes > 12) return 0;
    if (dia < 1 || dia > 31) return 0;
    if (anio < 1900 || anio > 2100) return 0;
    return 1;
}

/* Valida formato de telefono */
int validarTelefono(const char telefono[]) {
    int i;
    int len = strlen(telefono);
    if (len < 9 || len > 15) return 0;
    for (i = 0; i < len; i++) {
        if (!isdigit(telefono[i]) && telefono[i] != '+' && telefono[i] != '-') return 0;
    }
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
