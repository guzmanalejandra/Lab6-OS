#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>
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

int shared_counter = 0;
sem_t ctr;

void * thread_runner( void * args ) {
    /* Get the log file from the args */
    FILE * log = ( ( FILE * )args );

    /* Start processing */
    int tid = gettid();

    fprintf( log, "Iniciando Thread: %d \n", tid );

    fprintf( log, "Inicializando loop dentro del thread %d\n", tid );

    int j;
    for( j = 0; j < ITER_N; j++ ) {
        fprintf( log, "Iteracion %d en el thread %d\n", j, tid );
        
        fprintf( log, "Recurso tomado, enviando un wait al semaforo...\n");
        sem_wait( &ctr );
        fprintf( log, "se ha enviado señal correctamente en el thread %d usando el recurso compartido\n", tid );
        shared_counter++;
        fprintf( log, "Incrementando contador a %d en el thread %d \n", shared_counter, tid);
        fprintf( log, "Desbloqueando recurso, enviandole mensaje a semaforo...\n");
        sem_post( &ctr );
        fprintf( log, "Señal exitosa tid %d.\n", tid);
        sleep( 1 );
    }

    fprintf( log, "Terminando Thread tid %d\n", tid);
    pthread_exit( NULL );


}

int main() {

    FILE * log_semaphore = fopen( "BitacoraSemaforo.txt", "w" );

    /* Lets get ready the thread pool and sem */
    printf(" Creando psicina de Threads \n");
    pthread_t thread_pool[ THREAD_N ];
    printf(" Sem INIT \n");
    sem_init( &ctr, 0, 1 );

    /* Lets start the threads and join them after completion */
    printf(" Thread creado \n");
    int i;
    for( i = 0; i < THREAD_N; i++ ) {
        pthread_create( &thread_pool[ i ], NULL, &thread_runner, log_semaphore );   
    }

    printf(" Thread join \n");
    for( i = 0; i < THREAD_N; i++ ) {
        pthread_join( thread_pool[i], NULL );
    }

    printf(" File cerrado \n");
    fclose( log_semaphore );
    return 0;

}
