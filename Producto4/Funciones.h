
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define _CRT_SECURE_NO_WARNINGS

//guardar los adaptadores en un archibo de texto
void guardarIPConfig();

//mostrar el contenido de un archibo de texto con los adaptadores de red
void adaptadores();

//extraer adaptador y guardar caracteristicas en un archivo
void verAdaptador(char* adaptador);

//calcular velocidad de una direccion IP
int calcularVelocidad(char DNS[200]);

//contar los saltos
void saltos(FILE* docXML, char IP[]);


char* quitarSaltoLinea(char* linea);
