#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "parser.h"

#define TAM 100

char mypath[TAM];
char ** items;
int background, num;

void dir_actual();
void cambiar_dir();
void imprimir(); 
void limpiar(); 
void tiempo();
void salir();
void codExterno();

int main()
{	
	char comando[TAM];
	pid_t pid;
	int seguir;
	
	dir_actual();

	seguir = 1;
	while(seguir == 1)
	{
	  
	  printf ("user@user:%s$ ", mypath);
	  fgets (comando, TAM, stdin);
	  
	  num = separaItems (comando, &items, &background);

	  if(background == 1){
	    pid = fork();
	    
	    if(pid > 0){
	      printf("%d\n", (int)pid);
	      continue;
	    }
	    else if(pid == 0){
	      seguir = 0;
	    }
	    else{
	      printf("No se pudo ejecutar en segundo plano");
	    }
	  }
	  
	  if(strcmp(items[0],"exit") == 0){
	    salir();
	  }
	  else if(strcmp(items[0], "pwd") == 0){
	    printf("%s\n", mypath);
	  }
	  else if(strcmp(items[0], "cd") == 0){
	    cambiar_dir();
	  }
	  else if(strcmp(items[0], "clr") == 0){
	    limpiar();
	  }
	  else if(strcmp(items[0], "echo") == 0){
	    imprimir();
	  }
	  else if(strcmp(items[0], "time") == 0){
	    tiempo();
	  }
	  else{
	    codExterno();
	  }
		
	}
	
	return 0;
}

void dir_actual()
{
    getcwd(mypath, TAM);
} 

void cambiar_dir() 
{
  if(chdir(items[1]) == 0)
    dir_actual();
} 

void imprimir() 
{
  int i;
  
  for(i = 1; i < num; i++)
    printf("%s ", items[i]);
  
  printf("\n");
} 

void limpiar()
{
  printf("\033[2J\033[1;1H");
}

void tiempo()
{
  time_t hora;
  struct tm *ts;
  char buf[80];
  
  // hora actual
  hora = time(NULL);
  
  // formato de hora
  ts = localtime(&hora);
  strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S", ts);
  printf("%s\n", buf);
} 

void salir()
{ 
   exit(0); 
}

void codExterno()
{
  char ejec[] = "/bin/";
  if(background == 0){
    pid_t pid;

    pid = fork();
  
    if(pid == 0){
      strcat(ejec, items[0]);

      if(execv(ejec, items)){
	printf("Comando no reconocido o no se pudo ejecutar\n");
	exit(1);
      }
    }else if(pid > 0){
      // Padre
      wait(NULL);
    }else{ 
      // Error 
      printf("No se pudo crear un proceso hijo"); 
    }
  }
  else{
    strcat(ejec, items[0]);

    if(execv(ejec, items)){
      printf("Comando no reconocido o no se pudo ejecutar\n");
      exit(1);
    }
  }
}
