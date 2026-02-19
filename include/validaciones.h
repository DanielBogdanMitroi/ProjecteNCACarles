#ifndef VALIDACIONES_H
#define VALIDACIONES_H
int validarEmail(const char email[]);
int validarFecha(const char fecha[]);
int validarTelefono(const char telefono[]);
int validarNota(float nota);
int validarCreditos(int creditos);
int validarHora(const char hora[]);
int validarPassword(const char password[]);
int existeID(const char archivo[], const char id[]);
#endif
