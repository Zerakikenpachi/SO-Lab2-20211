/*
* Unix shell
* A continuación se realiza el programa de inix/c donde se crea un simple Unix shell con una interfaz de linea de comandos
* @author Victor restorepo - victora.agudelo@udea.edu.co
* @author Sebastian Muñoz - sebastiana.munoz@udea.edu.co
* @version 01/05/2021
*/

/*
* Importaciones necesarias
* A continuación se incluyen todos los ficheros necesarios para la elaboración del Unix shell
* @author Victor restorepo - victora.agudelo@udea.edu.co
* @author Sebastian Muñoz - sebastiana.munoz@udea.edu.co
* @version 01/05/2021
*/
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <wait.h>
#define max_args 13
#define maxln_Com_Amb 105

/*
* Declaración de variables
* A continuación se declaran todas las variables que son usadas en la elaboración del Unix shell
* la variable comando sirve para almacenar el comando ingresado por el usuario
* la variable comandoEco sirve para almacenar el mensaje para imprimir en cho
* la variable args sirve para almacenar los argumenros de un comando
* la variable PWD sirve para almacenar el directorio de trabajo
* la variable segPlano sirve para saber si hay ejecucion en segundo plano
* @author Victor restorepo - victora.agudelo@udea.edu.co
* @author Sebastian Muñoz - sebastiana.munoz@udea.edu.co
* @version 01/05/2021
*/
char comando[maxln_Com_Amb];
char comandoEco[maxln_Com_Amb];
char *args[max_args];
char PWD[maxln_Com_Amb];
int segPlano;

/*
* Declaración de cabeceras de funciones
* A continuación se declaran todas las cabeceras de funciones que son usadas en la elaboración del Unix shell
* La función Fexit sirve para finalizar la ejecucion del interprete
* La función cd sirve para cambiar el directorio de trabajo
* La función separaArgs sirve para separar la cadena ingresada por el usuario enpalabras individuales,la 1ra palabra sera considerada comando y el resto sus argumentos
* La función eco sirve para imprimir un mensaje por pantalla
* La función comExterno sirve para ejecutar un comando externo
* La función esSegundoPlano sirve para ver si se desea hacer una ejecución en segundo plano
* La función Ftime sirve para obtener el tiempo
* La función clr sirve para limpiar la consola
* La función pwd sirve para imprimir el directorio de trabajo
* @author Victor restorepo - victora.agudelo@udea.edu.co
* @author Sebastian Muñoz - sebastiana.munoz@udea.edu.co
* @version 01/05/2021
*/
void Fexit(void);
void cd(void);
void separaArgs(void);
void eco(void);
void comExterno(void);
void esSegundoPlano(void);
void Ftime(void);
void clr(void);
void pwd(void);

/*
* Función principal
* Función que realiza todos los procesos de ejecución de diferentes comandos
* Inicialmente obtiene la ruta actual y carga dicha ruta en PWD; luego realiza la impresión  del directorio actual;
* posteriormente realiza una limpieza del buffer de entrada de teclado, borra cualquier contenido previo de comandi;
* adiccionalmente obtiene el comando ingresado por la persona en la shell unix, pasa ese comando a la función comandoEco e
* imprime un mensaje en caso tal que se desee.
* Cuando se contiene información en el comando se procede a validar si hay ejecución en segundo plano, se rectifica dicha ejecución 
* y se separa el comando de sus argumentos para finalmente realizar una comparación entre el comando y los que se desean ejecutar internamente
* y si no se tienen internamente se ejecuta comExterno para hacer dicha ejecución de manera externa
* @author Victor Restrepo - victora.agudelo@udea.edu.co
* @author Sebastian Muñoz - sebastiana.munoz@udea.edu.co
* @version 01/05/2021
*/
int main(void)
{
    getcwd(PWD, maxln_Com_Amb);
    while (1)
    {
        printf("%s%s>", "<wish>", PWD);
        __fpurge(stdin);
        memset(comando, '\0', maxln_Com_Amb);
        scanf("%[^\n]s", comando);
        strcpy(comandoEco, comando);
        if (strlen(comando) > 0)
        {
            segPlano = 0;
            esSegundoPlano();
            separaArgs();
            if (strcmp(comando, "cd") == 0)
            {
                cd();
            }
            else if (strcmp(comando, "clr") == 0)
            {
                clr();
            }
            else if (strcmp(comando, "echo") == 0)
            {
                eco();
            }
            else if (strcmp(comando, "pwd") == 0)
            {
                pwd();
            }
            else if (strcmp(comando, "time") == 0)
            {
                Ftime();
            }
            else if (strcmp(comando, "exit") == 0)
            {
                Fexit();
            }
            else
            {
                comExterno();
            }
        }
    }
    return 0;
}

/*
* Función para imprimir mensaje o echo
* Función que valida la información que llega cuando el usuario envia una instrucción de tipo echo para finalmente imprimir dicha cadena
* @author Victor Restrepo - victora.agudelo@udea.edu.co
* @author Sebastian Muñoz - sebastiana.munoz@udea.edu.co
* @version 01/05/2021
*/
void eco(void)
{
    if (args[1])
    {
        int i;
        int j = 0;
        for (i = 0; i < maxln_Com_Amb; i++)
        {
            if (j)
            {
                printf("%c", comandoEco[i]);
            }
            if (comandoEco[i] == ' ')
            {
                j = 1;
            }
            if (comandoEco[i] == '\0')
            {
                i = maxln_Com_Amb + 1;
                printf("\n");
            }
        }
    }
}

/*
* Función para imprimir la hora
* Función que crea una estructura que maneja el tiempo,creando un apuntador y obteniendo el tiempo local para imprimirlo finalmente
* @author Victor Restrepo - victora.agudelo@udea.edu.co
* @author Sebastian Muñoz - sebastiana.munoz@udea.edu.co
* @version 01/05/2021
*/
void Ftime()
{
    time_t xTiempo;
    struct tm *pxTiempo;
    char strHora[50];
    xTiempo = time(NULL);
    pxTiempo = localtime(&xTiempo);
    strftime(strHora, 50, "%H:%M:%S\n", pxTiempo);
    printf("La hora es: %s", strHora);
}

/*
* Función para limpiar la consola
* Función que limpia la consola, para ello hace uso del comando externo clear
* @author Victor Restrepo - victora.agudelo@udea.edu.co
* @author Sebastian Muñoz - sebastiana.munoz@udea.edu.co
* @version 01/05/2021
*/
void clr()
{
    strcpy(comando, "clear");
    comExterno();
}

/*
* Función para cambiar de directorio
* Función que verifica que el comando llegue con argumentos necesarios para realizar el cambio de directorio
* cuando se tiene el argumento necesario se realiza un cambio de directorio, en caso contrario se envia un mensaje de error
* @author Victor Restrepo - victora.agudelo@udea.edu.co
* @author Sebastian Muñoz - sebastiana.munoz@udea.edu.co
* @version 01/05/2021
*/
void cd()
{
    if (args[1])
    {
        if (chdir(args[1]) != 0)
        {
            printf("Error! %s no existe o no se puede cambiar a este directorio\n", args[1]);
        }
        else
        {
            getcwd(PWD, maxln_Com_Amb);
        }
    }
}

/*
* Función para imprimir el directorio actual de trabajo (PWD)
* @author Victor Restrepo - victora.agudelo@udea.edu.co
* @author Sebastian Muñoz - sebastiana.munoz@udea.edu.co
* @version 01/05/2021
*/
void pwd()
{
    printf("%s\n", PWD);
}

/*
* Función para finalizar la ejecucion del interprete
* @author Victor Restrepo - victora.agudelo@udea.edu.co
* @author Sebastian Muñoz - sebastiana.munoz@udea.edu.co
* @version 01/05/2021
*/
void Fexit()
{
    exit(0);
}

/*
* Función para ejecutar comandos externos
* Esta función crea un proceso hijo (fork), si esté es mejor que 0 retorna error, sino trata de ejecutar el comando con los argumentos que tenga
* si falla notifica un error y cierra el proceso hijo
* @author Victor Restrepo - victora.agudelo@udea.edu.co
* @author Sebastian Muñoz - sebastiana.munoz@udea.edu.co
* @version 01/05/2021
*/
void comExterno()
{
    int pid = 0;
    int status;
    pid = fork();
    if (pid < 0)
        printf("Error! no se pudo crear un proceso hijo");
    if (pid == 0)
    {
        status = execvp(comando, args);
        if (status)
        {
            printf("Error! %s no se reconoce o no se pudo ejecutar\n", comando);
            exit(1);
        }
    }
    if (segPlano == 0)
    {
        wait(NULL);
    }
}

/*
* Función para separar argumentos del comando
* Función que separa palabras individuales usando tokens para finalmente envia el primer argumento como comando para un
* requerimiento de execvp
* @author Victor Restrepo - victora.agudelo@udea.edu.co
* @author Sebastian Muñoz - sebastiana.munoz@udea.edu.co
* @version 01/05/2021
*/
void separaArgs(void)
{
    int i;
    for (i = 0; i < (max_args - 1); i++)
        args[i] = NULL;
    strtok(comando, " "), i = 0;
    args[i] = comando;
    while ((args[++i] = strtok(NULL, " ")) != NULL && i < (max_args - 2));
}

/*
* Función para verificar si se desea ejecutar en segundo plano
* Función que análisa el comando de entrada, si se encuentra un '&' significa que se desea ejecutar en segundo plano 
* requerimiento de execvp
* @author Victor Restrepo - victora.agudelo@udea.edu.co
* @author Sebastian Muñoz - sebastiana.munoz@udea.edu.co
* @version 01/05/2021
*/
void esSegundoPlano()
{
    int i;
    for (i = 0; comando[i] != '\0'; i++)
    {
        if (comando[i] == '&')
        {
            comando[i] = '\0';
            segPlano = 1;
        }
    }
}