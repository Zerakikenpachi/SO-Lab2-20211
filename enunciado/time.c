/*La estrategia general es hacer un fork para crear un proceso hijo el cual ejecutara 
el comando especificado. Sin embargo, antes de que el proceso hijo ejecute el comando 
espeficado, debera almacenar el tiempo actual (starting time). El padre invocará el wait
 para esperar por la culminación del proceso hijo. Luego, una vez que el proceso hijo 
 culmine, el padre almacenara el tiempo actual en este punto (ending time). La diferencia
  entre los tiempos inicial y final (starting y endind) representará el tiempo gastado 
  para ejecutar el comando. Por ejemplo la salida en pantalla de abajo muestra la cantidad 
  de tiempo para correr el comando ls:*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdio.h>

//the only way to comunicate between process is throug pipeline
int main(int argc, char *argv[]){
    //type from the date
    struct timeval current_time;
    //pipeline
    int fd[2];
    pipe(fd);
    //save time in seconds
    long int time_before;
    int pid = fork();

    if(pid == 0){//child process
        //I got the time 
        gettimeofday(&current_time, NULL);
        //pipeline father write (close read)
        close(fd[0]);
        //convert time in micro seconds
        time_before = current_time.tv_usec;
        write(fd[1], &time_before, sizeof(time_before));
        close(fd[1]);
        //executation command ls 
        execlp("/bin/ls",argv[1],NULL);
        return 0;
    }else{
        //Father process has to wait child proccess execution
        wait(NULL);
        gettimeofday(&current_time, NULL);
        //pipeline father read (close write)
        close(fd[1]);
        long int a = current_time.tv_usec;//the time is taking
        read(fd[0],&time_before,sizeof(time_before));
        printf("elapsed time %ld\n ", a-time_before);
    }
    return 0;
}
