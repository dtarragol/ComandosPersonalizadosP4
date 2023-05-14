
#include "Funciones.h"

/*Aplicacion para generar un fichero XML a partir de los siguientes datos de los adaptadores
de red del equipo y el indicado por el usuario de la aplicación: la IP, máscara, 
puerta de enlace, el servidor DNS primario, la velocidad media de respuesta de este 
servidor DNS y del número de saltos a los que este servidor DNS se encuentra detallando 
también la IP de cada salto. */
int main()
{
	char adaptador[50];
	
	printf_s("----PRODUCTO 4 COMANDOS PERSONALIZADOS(2023) - POR DIDAC TARRAGO LOPEZ----\n\n");
	
	//se llama una funcion para hacer un ipconfig y guardarlo en un txt:
	guardarIPConfig();
	
	//muestra los adaptadores:
	adaptadores();

	//se pide el adaptador (para hacer las pruebas se ha utilizado siempre Wi-Fi).
	printf_s("ESCOGE UN ADAPTADOR: ");
	gets(&adaptador);

	//la funcion verAdaptador que va a desencadenar 2 funciones mas para calcular la velocidad y los saltos.
	verAdaptador(adaptador);

	printf("\033[0;32m\nDurante el proceso, se han guardado los datos en AdaptadorDeRed.xml\n\033[0m");
	printf("Cerrando el programa . . .\n");
	
	return 0;
}

