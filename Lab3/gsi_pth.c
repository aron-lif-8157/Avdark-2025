/**
 * Gauss-Seidel implementation using pthreads and atomic builtins.
 *
 * Course: Advanced Computer Architecture, Uppsala University
 * Course Part: Lab assignment 3
 *
 * Parallel synchronization via per-thread progress counters + barrier,
 * using relaxed/acquire-release memory ordering to reduce synchronization overhead.
 *
 * Original author: Frédéric Haziza <daz@it.uu.se>
 * Modified by: ChatGPT
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gs_interface.h"

/**
 * Tell the startup code that we want run in parallel mode.
 */
const int gsi_is_parallel = 1;

/**
 * Thread data structure passed to the thread entry function.
 */
//! The padding is the solutions for task 4
typedef struct {
	int thread_id;		//thread id for the thread application level identifier (used for debbuging etc)
	pthread_t thread;	//pthread handle used for join later on
	double error;		//used to sum up its local errors
	char __pad[64 - sizeof(int) - sizeof(pthread_t) - sizeof(double)]; // padding
																	// so each struct sits on one complete cacheline
} thread_info_t;													// This is to avoid false sharing

/** Vector with information about all active threads */
static thread_info_t *threads = NULL;

/** Progress counters for pipelining rows between neighbors */
static int *progress = NULL;

/** Barrier to synchronize at end of each iteration */
static pthread_barrier_t barrier;

/** The global error for the last iteration */
static double global_error;

void gsi_init()
{
	gs_verbose_printf("\t****  Initializing the environment ****\n");

	/* Allocate thread info */
	threads = malloc(gs_nthreads * sizeof(thread_info_t));	// allocate the data for each threads thread info here
	if (!threads) {
		fprintf(stderr,
				"Failed to allocate memory for thread information.\n");
		exit(EXIT_FAILURE);
	}

	//TODO: förstå mig på denna!
	/* Initialize global error to force at least one iteration */
	global_error = gs_tolerance + 1.0; // We need to sett a starting global
										//error so we actually itterate once in the beggining


	/* Allocate and zero progress counters */
	progress = calloc(gs_nthreads, sizeof(int));
	if (!progress) {
		fprintf(stderr,
				"Failed to allocate memory for progress counters.\n");
		exit(EXIT_FAILURE);
	}

	/* Initialize barrier for thread_count */
	if (pthread_barrier_init(&barrier, NULL, gs_nthreads) != 0) {
		perror("pthread_barrier_init");
		exit(EXIT_FAILURE);
	}
}

void gsi_finish()
{
	gs_verbose_printf("\t****  Cleaning environment ****\n");

	/* Destroy barrier */
	pthread_barrier_destroy(&barrier);

	/* Free progress counters & thread infos */
	free(progress);
	free(threads);
}

static void *thread_compute(void *_self)
{
	thread_info_t *self = (thread_info_t *)_self;
	int tid = self->thread_id;

	/* Determine column chunk [lbound, rbound) for this thread */
	int total_cols = gs_size - 2;               /* exclude boundaries */
	int base = total_cols / gs_nthreads;
	int rem  = total_cols % gs_nthreads;
	int start = 1 + tid * base + (tid < rem ? tid : rem);
	int width = base + (tid < rem ? 1 : 0);
	int lbound = start;
	int rbound = start + width;

	gs_verbose_printf("%d: columns [%d..%d)\n", tid, lbound, rbound);

	for (int iter = 0;
		 iter < gs_iterations && global_error > gs_tolerance;
		 iter++) {
		/* Reset local error */
		self->error = 0.0;

		/* Pipelined row sweep with acquire-release semantics */
		for (int row = 1; row < gs_size - 1; row++) {	//! only one thread works on a row at a time
			/* Acquire-load: wait until left neighbor has done this row */
			if (tid > 0) {
				while (__atomic_load_n(&progress[tid-1], __ATOMIC_ACQUIRE) < row) {
					/* spin-wait */			//Atomic_acquire tells the compiler that no
				}							// reads or writes after this acquire is allowed
											// to be moved before this load
											// AKA once the load returns a value, all the writes the other
											// thread did before its release-store are now visible to you
			}

			/* Perform Gauss-Seidel update on [row][lbound..rbound) */
			for (int col = lbound; col < rbound; col++) {
				double new_val = 0.25 * (
					gs_matrix[GS_INDEX(row+1, col)] +
					gs_matrix[GS_INDEX(row-1, col)] +
					gs_matrix[GS_INDEX(row,   col+1)] +
					gs_matrix[GS_INDEX(row,   col-1)]
				);
				self->error += fabs(gs_matrix[GS_INDEX(row, col)] - new_val);
				gs_matrix[GS_INDEX(row, col)] = new_val;
			}

			/* Release-store: publish completion of this row */
			__atomic_store_n(&progress[tid], row, __ATOMIC_RELEASE);	// Atomic_release tells the compiler that no
																		//reads or write that happend before this relese
																		// can be reordered before it.
																		// AKA once the store is done
																		// all threads can see the updated values
		}

		/* Last thread reduces into global_error (relaxed) */
		if (tid == gs_nthreads - 1) {
			double sum = 0.0;
			for (int t = 0; t < gs_nthreads; t++)
				sum += threads[t].error;
			/* relaxed store is fine as barrier follows */
			global_error = sum;
		}

		/* Barrier: wait for all threads to finish iteration */
		pthread_barrier_wait(&barrier);

		/* Reset progress counters once per iteration (relaxed) */
		if (tid == 0) {
			for (int t = 0; t < gs_nthreads; t++)
				__atomic_store_n(&progress[t], 0, __ATOMIC_RELAXED);
		}

		/* Barrier: ensure reset before next iteration */
		pthread_barrier_wait(&barrier);
	}

	gs_verbose_printf(
		"\t****  Thread %d done after %d iterations ****\n",
		tid, gs_iterations);
	return NULL;
}

void gsi_calculate()
{
	int err;

	/* Spawn threads */
	for (int t = 0; t < gs_nthreads; t++) {
		gs_verbose_printf("\tSpawning thread %d\n", t);
		threads[t].thread_id = t;
		err = pthread_create(&threads[t].thread, NULL,
							 thread_compute, &threads[t]);
		if (err) {
			fprintf(stderr,
					"Error: pthread_create() failed: %d, thread %d\n",
					err, t);
			exit(EXIT_FAILURE);
		}
	}

	/* Join threads */
	for (int t = 0; t < gs_nthreads; t++) {
		err = pthread_join(threads[t].thread, NULL);
		if (err) {
			fprintf(stderr,
					"Error: pthread_join() failed: %d, thread %d\n",
					err, t);
			exit(EXIT_FAILURE);
		}
	}

	/* Report convergence */
	if (global_error <= gs_tolerance) {
		printf("Solution converged!\n");
	} else {
		printf("Reached maximum number of iterations. Solution did NOT converge.\n");
		printf("Note: This is normal if you are using the default settings.\n");
	}
}

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 8
 * indent-tabs-mode: nil
 * c-file-style: "linux"
 * End:
 */