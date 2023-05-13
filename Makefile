CC=gcc
FLAGS=-pthread

semaphore: semaforo.c
	$(CC) $(FLAGS) -o semaphore semaforo.c

monitor: monitor.c
	$(CC) $(FLAGS) -o monitor monitor.c
