//#include <pthread.h>
//#include <stdio.h>
//
///* this function is run by the second thread */
//void *inc_x(void *x_void_ptr)
//{
//
///* increment x to 100 */
//    int *x_ptr = (int *)x_void_ptr;
//    while(++(*x_ptr) < 100) {
//        printf("%d\n", *x_ptr);
//    }
//
//    printf("x increment finished\n");
//
///* the function must return something - NULL will do */
//    return NULL;
//
//}
//
//int main(void)
//{
//
//    int x = 0, y = 0;
//
///* show the initial values of x and y */
//    printf("x: %d, y: %d\n", x, y);
//
///* this variable is our reference to the second thread */
//    pthread_t inc_x_thread;
//
///* create a second thread which executes inc_x(&x) */
//    if(pthread_create(&inc_x_thread, NULL, inc_x, &x)) {
//
//        fprintf(stderr, "Error creating thread\n");
//        return 1;
//
//    }
///* increment y to 100 in the first thread */
//    while(++y < 100) {
//        printf("%d\n", y);
//    }
//
//    printf("y increment finished\n");
//
///* wait for the second thread to finish */
//    if(pthread_join(inc_x_thread, NULL)) {
//
//        fprintf(stderr, "Error joining thread\n");
//        return 2;
//
//    }
//
///* show the results - x is now 100 thanks to the second thread */
//    printf("x: %d, y: %d\n", x, y);
//
//    return 0;
//
//}

//#include <stdio.h>
//#include <stdlib.h>
//#include <pthread.h>
//#include <unistd.h>
//
//#define NTHREADS 30
//
//long random_at_most(long max) {
//    unsigned long
//    // max <= RAND_MAX < ULONG_MAX, so this is okay.
//            num_bins = (unsigned long) max + 1,
//            num_rand = (unsigned long) RAND_MAX + 1,
//            bin_size = num_rand / num_bins,
//            defect   = num_rand % num_bins;
//
//    long x;
//    do {
//        x = random();
//    }
//        // This is carefully written not to overflow
//    while (num_rand - defect <= (unsigned long)x);
//
//    // Truncated division is intentional
//    return x/bin_size;
//}
//
//void *myFun(void *x)
//{
//    int tid;
//    tid = *((int *) x);
//    long delay = random_at_most(60);
//    sleep(delay);
//    printf("Hi from thread %d! Sleep %ul\n", tid, delay);
//    return NULL;
//}
//
//int main(int argc, char *argv[])
//{
//    pthread_t threads[NTHREADS];
//    int thread_args[NTHREADS];
//
//    int rc, i;
//
//    /* spawn the threads */
//    for (i=0; i<NTHREADS; ++i)
//    {
//        thread_args[i] = i;
//        printf("spawning thread %d\n", i);
//        rc = pthread_create(&threads[i], NULL, myFun, (void *) &thread_args[i]);
//    }
//
//    /* wait for threads to finish */
//    for (i=0; i<NTHREADS; ++i) {
//        rc = pthread_join(threads[i], NULL);
//    }
//
//    return 0;
//}


//#include<stdio.h>
//#include<pthread.h>
//#include <unistd.h>
//
//void* say_hello(void* data)
//{
//    char *str;
//    str = (char*)data;
//    int i;
//    for(i = 0; i < 10; ++i)
//    {
//        printf("%s\n",str);
//        sleep(1);
//    }
//}
//
//void main()
//{
//    pthread_t t1,t2;
//
//    pthread_create(&t1,NULL,say_hello,"hello from 1");
//    pthread_create(&t2,NULL,say_hello,"hello from 2");
//    pthread_join(t1,NULL);
//}


//#include <stdio.h>
//#include <pthread.h>
//#include <unistd.h>
//
//void* producer(void *ptr) {
//    int i;
//
//    for (i = 0; i < 50; i++) {
//        usleep(50000);
//        printf("Write %d\n", i);
//    }
//    printf("Write tertertert\n");
//    pthread_exit(0);
//    return NULL;
//}
//
//void* consumer(void *ptr) {
//    int i;
//
//    for (i = 0; i < 50; i++) {
//        usleep(50000);
//        printf("Read %d\n", i);
//    }
//    printf("Read tertertert\n");
//    pthread_exit(0);
//    return NULL;
//}
//
//int main(int argc, char **argv) {
//    pthread_t pro, con;
//
//     // Create the threads
//    pthread_create(&con, NULL, consumer, NULL);
//    pthread_create(&pro, NULL, producer, NULL);
//
//    // Wait for the threads to finish
//    // Otherwise main might run to the end
//    // and kill the entire process when it exits.
//
//    pthread_join(&pro, NULL);
//    pthread_join(&con, NULL);
//    sleep(10);
//
//    return 0;
//}

/*
 *  Solution to Producer Consumer Problem
 *  Using Ptheads, a mutex and condition variables
 *  From Tanenbaum, Modern Operating Systems, 3rd Ed.
 */

/*
    In this version the buffer is a single number.
    The producer is putting numbers into the shared buffer
    (in this case sequentially)
    And the consumer is taking them out.
    If the buffer contains zero, that indicates that the buffer is empty.
    Any other value is valid.
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 10			/* Numbers to produce */
pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;
int buffer = 0;

void* producer(void *ptr) {
    int i;

    for (i = 1; i <= MAX; i++) {
        pthread_mutex_lock(&the_mutex);	/* protect buffer */
        /* If there is something in the buffer then wait */
        while (buffer != 0) {
            pthread_cond_wait(&condp, &the_mutex);
        }
        usleep(50000);
        buffer = i;
        printf("Write %d\n", i);
        pthread_cond_signal(&condc);	/* wake up consumer */
        pthread_mutex_unlock(&the_mutex);	/* release the buffer */
    }
    pthread_exit(0);
}

void* consumer(void *ptr) {
    int i;

    for (i = 1; i <= MAX; i++) {
        pthread_mutex_lock(&the_mutex);	/* protect buffer */
        /* If there is nothing in the buffer then wait */
        while (buffer == 0) {
            pthread_cond_wait(&condc, &the_mutex);
        }
        usleep(100000);
        buffer = 0;
        printf("Read %d\n", i);
        pthread_cond_signal(&condp);	    /* wake up consumer */
        pthread_mutex_unlock(&the_mutex);	/* release the buffer */
    }
    pthread_exit(0);
}

int main(int argc, char **argv) {
    pthread_t pro, con;

    // Initialize the mutex and condition variables
    /* What's the NULL for ??? */
    pthread_mutex_init(&the_mutex, NULL);
    pthread_cond_init(&condc, NULL);		/* Initialize consumer condition variable */
    pthread_cond_init(&condp, NULL);		/* Initialize producer condition variable */

    // Create the threads
    pthread_create(&con, NULL, consumer, NULL);
    pthread_create(&pro, NULL, producer, NULL);

    // Wait for the threads to finish
    // Otherwise main might run to the end
    // and kill the entire process when it exits.
    pthread_join(&con, NULL);
    pthread_join(&pro, NULL);

    printf("fsdf;ldslsjflksjfljflsdjf");

    // Cleanup -- would happen automatically at end of program
    pthread_mutex_destroy(&the_mutex);	/* Free up the_mutex */
    pthread_cond_destroy(&condc);		/* Free up consumer condition variable */
    pthread_cond_destroy(&condp);		/* Free up producer condition variable */

}