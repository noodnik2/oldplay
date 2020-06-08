
/*

	This program demonstrates the use of mutexes to lock threads.

	Logic:

		main()
			creates mutex
			creates two threads:
			thread 1
			thread 2

		thread1()
			forever {
				lock mutex
				print top of thread 1
				sleep 5
				print bottom of thread 1
				unlock mutex
			}


		thread2()
			forever {
				lock mutex
				print thread 2 active
				unlock mutex
			}
				

*/

#include	<stdio.h>
#include	<pthread.h>

static pthread_mutex_t sparemutex= PTHREAD_MUTEX_INITIALIZER ;
static pthread_mutex_t threadmutex= PTHREAD_MUTEX_INITIALIZER ;
static pthread_mutex_t mainmutex= PTHREAD_MUTEX_INITIALIZER ;

static void* thread1(void*) ;
static void* thread2(void*) ;

int main()
{
	pthread_t thr1, thr2 ;
	pthread_attr_t thrattr ;

	/* prevent threads from working */
	pthread_mutex_lock(&threadmutex) ;

	pthread_attr_init(&thrattr) ;
	if (pthread_create(&thr1, &thrattr, thread1, 0)< 0) {
		perror("can't start thread 1") ;
		return(1) ;
	}
	if (pthread_create(&thr2, &thrattr, thread2, 0)< 0) {
		perror("can't start thread 1") ;
		return(1) ;
	}

	printf("Threads launched\n") ;
	/* start threads */
	pthread_mutex_unlock(&threadmutex) ;

	pthread_mutex_lock(&mainmutex) ;

	/* put main thread to sleep until threads wake it */
	pthread_mutex_lock(&mainmutex) ;

	/* wait for threads to synchronize */
	pthread_mutex_lock(&threadmutex) ;
	printf("Threads freed main program - exiting\n") ;
	return(0) ;
}


/*

*/

static void* thread1(void*p)
{
	while(1) {
		pthread_mutex_lock(&threadmutex) ;
		printf("top of thread 1\n") ;
#if 1
		usleep(5000000UL) ;
#else
		usleep(300000UL) ;
#endif
		printf("bottom of thread 1\n") ;
		pthread_mutex_unlock(&threadmutex) ;
	}
	return(p) ;
}

static void* thread2(void*p)
{
	int i ;
	static int smstate= 0 ;

	for (i= 0; i< 20; i++) {
		if (smstate) {
			pthread_mutex_unlock(&sparemutex) ;
			smstate= 0 ;
		}
		else {
			pthread_mutex_lock(&sparemutex) ;
			smstate= 1 ;
		}
		pthread_mutex_lock(&threadmutex) ;
		printf(
			"thread 2 active - spare mutex %s\n",
			smstate? "locked": "unlocked"
		) ;
		pthread_mutex_unlock(&threadmutex) ;
	}

	/* restart main thread so program exits */
	pthread_mutex_unlock(&mainmutex) ;
	return(p) ;
}


/* end of file */
