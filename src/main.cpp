#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estructuras.h"
#include "usuarios.h"
#include "menu.h"
#include "archivos.h"
#include "utilidades.h"

int main() {
    /* Crear directorios y archivos iniciales si no existen */
    crearArchivosIniciales();

    Usuario usuario_actual;
    memset(&usuario_actual, 0, sizeof(Usuario));

    /* Mostrar login */
    if (login(&usuario_actual)) {
        menuPrincipal(usuario_actual);
    }

    printf("Hasta luego!\n");
    return 0;
}
