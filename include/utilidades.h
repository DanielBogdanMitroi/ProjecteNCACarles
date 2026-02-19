#ifndef UTILIDADES_H
#define UTILIDADES_H
void limpiarSaltoLinea(char str[]);
void leerString(char destino[], int tamano, const char mensaje[]);
void leerEntero(int *destino, const char mensaje[]);
void leerFloat(float *destino, const char mensaje[]);
void limpiarPantalla();
void pausar();
void generarID(char resultado[], const char prefijo[], int numero);
void convertirAMayusculas(char str[]);
void convertirAMinusculas(char str[]);
int compararStrings(const char str1[], const char str2[]);
#endif
