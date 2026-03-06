#ifndef UTILIDADES_H
#define UTILIDADES_H
void limpiarSaltoLinea(char str[]);
void leerString(char destino[], int tamano, const char mensaje[]);
void leerEntero(int *destino, const char mensaje[]);
void leerFloat(float *destino, const char mensaje[]);
void limpiarPantalla();
void pausar();
void generarID(char resultado[], const char prefijo[], int numero);
int obtenerSiguienteNumeroID(const char archivo[], const char prefijo[]);
void convertirAMayusculas(char str[]);
void convertirAMinusculas(char str[]);
int compararStrings(const char str1[], const char str2[]);
void mostrarValorActual(const char campo[], const char valor[]);
void mostrarValorActualEntero(const char campo[], int valor);
void mostrarValorActualFloat(const char campo[], float valor);
int solicitarConfirmacion(const char mensaje[]);
#endif
