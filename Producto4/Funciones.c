#include "Funciones.h"

//guardar los adaptadores en un archibo de texto
void guardarIPConfig() {
	system("ipconfig -all > ipconfig.txt");
}
//mostrar el contenido de un archibo de texto con los adaptadores de red
void adaptadores() {
	char auxiliariliar[200];
	FILE* ad;
	fopen_s(&ad, "ipconfig.txt", "r");
	printf("ADAPTADORES:\n\033[0;35m");
	while (fgets(auxiliariliar, 200, ad))
	{
		if (strstr(auxiliariliar, "Adaptador"))
		{
			printf("%s", auxiliariliar);
		}
	}
    printf("\n\033[0m");
	fclose(ad);
}
//extraer adaptador y guardar caracteristicas en un archivo
void verAdaptador(char* adaptador) {
    FILE* ad;
    FILE* datos;
    char buffer[200];
    char* memoria01, * memoria02, * memoriaSinSalto;
    char direccionDNS[200];
    char XML[] = "AdaptadorDeRed.xml"; //documento XML donde se almacenaran los datos
    char ref[] = ":";
    char* next_token;
    ad = fopen("ipconfig.txt", "r");//documento temporal 
    datos = fopen(XML, "w");
    fputs("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<network_adapter>\n", datos);
    fprintf(datos, "\t<nombredaptador>\"%s\"</nombredaptador>\n", adaptador);
    fgets(buffer, 200, ad);
    printf_s("------------------------\n");
    printf("Datos del Adaptador \"\033[0;35m%s\033[0m\":\n", adaptador);
    if (ad == NULL) printf("documento no encontrado\n");
    else {
        while (fgets(buffer, 200, ad)) {
            if (strstr(buffer, adaptador) != NULL) {
                fgets(buffer, 200, ad);
                while (!strstr(buffer, "IPv4")) fgets(buffer, 200, ad);
                //recogiendo las IP del adaptador en "ipconfig.txt
                memoria01 = strtok(buffer, ref);
                memoria01 = strtok(NULL, ref);
                printf("IPv4 \033[0;35m");
                fprintf(datos, "\t<ip_address>");
                printf(memoria01);
                while (*memoria01 != '\n') {
                    if (isdigit(*memoria01) || *memoria01 == '.') {
                        printf("%c", *memoria01);
                        fprintf(datos, "%c", *memoria01);
                    }
                    ++memoria01;
                }
                fprintf(datos, "</ip_address>\n");
                printf("\n");
                fgets(buffer, 200, ad);
                while (!strstr(buffer, " subred")) fgets(buffer, 200, ad);
                memoria01 = strtok_s(buffer, ref, &next_token);
                memoria01 = strtok_s(NULL, ref, &next_token);
                ++memoria01;
                //recogiendo la mascara de subred del adaptador en "ipconfig.txt
                if ((memoria02 = strchr(memoria01, '\n')) != NULL) {
                    *memoria02 = '\0';
                    printf("\033[0mSubred \033[0;35m%s", memoria01);
                    fprintf(datos, "\t<subnet_mask>%s</subnet_mask>\n", memoria01);
                }
                printf("\n");
                fgets(buffer, 200, ad);
                while (!strstr(buffer, "Puerta de enlace predeterminada")) fgets(buffer, 200, ad);
                //recogiendo la puerta de enlace predeterminada del adaptador en "ipconfig.txt
                if ((memoria02 = strchr(memoria01, '\n')) != NULL) {
                    //quitando el salto de linea
                    memoriaSinSalto = quitarSaltoLinea(memoria01);
                    printf("\033[0mPuerta predeterminada\033[0;35m %s", memoriaSinSalto);
                    fprintf(datos, "\t<gateway>%s</gateway>\n", memoriaSinSalto);
                }
                printf("\n");
                fgets(buffer, 200, ad);
                while (!strstr(buffer, "Servidores DNS")) {
                    fgets(buffer, 200, ad);
                    direccionDNS[0] = " ";
                    strcpy(direccionDNS, &buffer[46]);
                }
                //recogiendo las DNS del adaptador en "ipconfig.txt
                if ((memoria02 = strchr(memoria01, '\n')) != NULL) {
                    *memoria02 = '\0';
                    //Quitando salto de linea
                    memoriaSinSalto = quitarSaltoLinea(direccionDNS);
                    printf("\033[0mServidor DNS\033[0;35m %s\033[0m", memoriaSinSalto);
                    fprintf(datos, "\t<dns_server>%s</dns_server>\n", memoriaSinSalto);
                }
                printf_s("\n------------------------\n");
                printf("\033[0;32mCalculando velocidad %s . . .\n\n\033[0m", direccionDNS);
                printf("Velocidad DNS \033[0;31m%d milisengundos\n\033[0m", calcularVelocidad(direccionDNS));
                fprintf(datos, "\t<velocidad>%d</velocidad>\n", calcularVelocidad(memoria01));
                printf_s("------------------------\n");
                printf("\033[0;32mCalculando saltos\n. . .\n\n\033[0m");
                saltos(datos, memoria01);
                printf_s("------------------------\n");
            }
        }
        fputs("</network_adapter>", datos);
        fclose(ad);
        fclose(datos);
        remove("ipconfig.txt");
    }
}
//contar los saltos
void saltos(FILE* docXML, char IP[]) {
    char auxiliar[200];
    char ms[] = "ms"; //donde va a cortar las lineas del txt
    int s = 0; // variable para almacenar el numero de saltos
    char* auxiliar2;
    FILE* t;
    strcpy(auxiliar, "tracert "); //comando para enviar paquetes de datos y muestra los saltos
    strcat(auxiliar, IP);
    strcat(auxiliar, " > Tracert.txt"); //documento donde se almacenaran los datos
    system(auxiliar);
    t = fopen("Tracert.txt", "r");
    if (t == NULL) {
        printf("documento no encontrado\n");
    }
    else {
        while (fgets(auxiliar, 200, t)) {
            if (strstr(auxiliar, ms)) {
                //imprime los saltos con su numero de salto
                printf("\033[0;35m(Salto %i)\033[0m %s", s+1, &auxiliar[32]);
                auxiliar2 = strchr(auxiliar, ']');
                if (auxiliar2) {
                    *auxiliar2 = '\0';
                    auxiliar2 = strchr(auxiliar, '[') + 1;
                    //guarda los saltos en el XML
                    fprintf(docXML, "\t<ipsalto>%s</ipsalto>\n", auxiliar2);
                }
                else {
                    auxiliar2 = &auxiliar[strlen(auxiliar) - 2];
                    *auxiliar2 = '\0';
                    auxiliar2 = &auxiliar[32];
                    fprintf(docXML, "\t<ipsalto>%s</ipsalto>\n", auxiliar2);
                }
                s++;
            }
        }
        fclose(t);
    }
    remove("Tracert.txt");
    printf("DNS %s \033[0;31m%d saltos\n\033[0m", IP, s);
    fprintf(docXML, "\t<numsaltos>%d</numsaltos>\n", s);
}
//calcular velocidad de una direccion IP
int calcularVelocidad(char DNS[200]) {
    //char* next_token, next_token2;
    //errno_t err;
    char ping[] = "ping ", com[250], comPing[] = " > PING.txt", char1[] = "Media = ", char2[250];
    char* memoria;
    char Vel[100];
    FILE* f;
    
    strcpy(com, ping);
    strcat(com, DNS);
    com[strcspn(com, "\n")] = 0;
    strcat(com, comPing);
    system(com);
    f = fopen("PING.txt", "r");
    if (f == NULL) printf("El archivo no existe\n");
    else {
        while (fgets(char2, 250, f)) {
            if (strstr(char2, char1) != NULL) {
                memoria = strstr(char2, char1);
                memoria += strlen(char1);
                int i;
                for (i = 0; isdigit(*memoria); i++) {
                    Vel[i] = *memoria;
                    memoria += 1;
                }
                Vel[i] = '\0';
                break;
            }
        }
        fclose(f);
    }
    return atoi(Vel);
}
char* quitarSaltoLinea(char* linea) {
    char new_str[100];
    int j = 0;
    for (int i = 0; i < strlen(linea); i++) {
        if (linea[i] != '\n') {
            new_str[j] = linea[i];
            j++;
        }
    }
    new_str[j] = '\0';
    return new_str;
}