#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#ifndef THREAD_N
#define THREAD_N 4
#endif

#ifndef ITER_N
#define ITER_N 15
#endif

#ifndef gettid
#define gettid() syscall(SYS_gettid)
#endif

int resources = 100;

pthread_cond_t cond;
pthread_mutex_t resource_mutex;

void decrease( FILE * log ) {
    fprintf( log, "Bloqueando Mutex \n" );
    pthread_mutex_lock( &resource_mutex );
    if( resources > 0 ) {
        fprintf( log, "Recurso tomado\n" );
        resources--;
        pthread_cond_signal( &cond );
    }
    fprintf( log, "Recursos restantes %d\n", resources );
    fprintf( log, "Desbloquar mutex\n" );
    pthread_mutex_unlock( &resource_mutex );
}

void * thread_runner( void * args ) {

    /* Get the log file from the args */
    FILE * log = ( ( FILE * )args );

    /* Start processing */
    int tid = gettid();

    fprintf( log, "Thread iniciado id: %d \n", tid );

    fprintf( log, "Iniciando loop dentro del thread%d\n", tid );

    int j;
    for( j = 0; j < ITER_N; j++ ) {
        fprintf( log, "Iteracion %d en el thread %d\n", j, tid );

        fprintf( log, "Mutex bloqueado por el Thread ID: %d\n", tid );
        decrease( log );

    }

}

int main() {

    /* Open log file */
    FILE * log_file = fopen( "BitacoraMonitor.txt", "wSS" );

    /* Lets get ready the thread pool and sem */
    printf("Creando piscina de threads \n");
    pthread_t thread_pool[ THREAD_N ];


    /* Lets start the threads and join them after completion */
    printf(" Thread creado \n");
    int i;
    for( i = 0; i < THREAD_N; i++ ) {
        pthread_create( &thread_pool[ i ], NULL, &thread_runner, log_file );   
    }

    printf(" Thread join \n");
    for( i = 0; i < THREAD_N; i++ ) {
        pthread_join( thread_pool[i], NULL );
    }

    printf(" File cerrada \n");
    fclose( log_file );

}
