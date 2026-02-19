#ifndef USUARIOS_H
#define USUARIOS_H
#include "estructuras.h"
int login(Usuario *usuario);
void menuUsuarios(Usuario usuario_actual);
void listarUsuarios();
void agregarUsuario(Usuario usuario_actual);
void cambiarPassword(Usuario *usuario_actual);
#endif
