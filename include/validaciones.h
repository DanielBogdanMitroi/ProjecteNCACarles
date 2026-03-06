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
int validarCadenaNoVacia(const char cadena[], int longitud_minima);
int emailYaExiste(const char email[], const char archivo[], const char id_actual[]);
int validarConfirmacion(const char confirmacion[]);
int validarPuntuacion(float puntuacion, float maximo);
int validarRangoFechas(const char fecha_inicio[], const char fecha_fin[]);
int validarDiaSemana(const char dia[]);
int validarHoraRango(const char hora_inicio[], const char hora_fin[]);
#endif
