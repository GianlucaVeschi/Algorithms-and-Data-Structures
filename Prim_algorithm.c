// Gianluca Veschi
// Usuario: gianluca.veschi@udc.es


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include "mytypes.h"



/* It obtains the current time in microseconds */

double microseconds () {
	struct timeval t;
	if (gettimeofday(&t, NULL) < 0)
		return 0.0;
	return (t.tv_usec + t.tv_sec * 1000000.0);
}



/* We generate pseudorandom numbers betweeen -n and +n */

void init_seed() {
	srand(time(NULL));
}

void random_init(int v [], int n) {
	int i, m = 2*n+1;
	for (i=0; i < m; i++) v[i] = (rand() % m) - n;
}



/* Prim algorithm implementation */

queue prim (matrix m, int nodes, queue edges) {
/* calculate the minimum spanning tree returning
the edges of the tree in the ’edges’ queue */
	int min, i, j, k=0;
	edge a;
	int *closest = (int *) malloc (nodes * sizeof (int));
	int *minDistance = (int *) malloc (nodes * sizeof (int));
	
	edges = create_queue (edges);
	minDistance [0] = -1;
	for (i = 1; i < nodes; i++) {
		closest [i] = 0;
		minDistance [i] = m [i][0];
	}
	for (i = 1; i < nodes; i++) {
		min = MAX_SIZE;
		for (j = 1; j < nodes; j++) {
			if ((0 <= minDistance [j]) && (minDistance [j] < min)) {
				min = minDistance [j];
				k = j;
			}
		}
		a.x = k;
		a.y = closest [k];
		a.weight = min;
		edges = enqueue (a, edges);
		minDistance [k] = -1;
		
		for (j = 1; j < nodes; j++) {
			if (m [j][k] < minDistance [j]) {
				minDistance [j] = m [j][k];
				closest [j] = k;
			}	
		}
	}
	free(closest);
	free(minDistance);
	return edges;
}



/*
	Three different test to validate the prim's algorithm
*/

void test1 () {
	
	int i, j;
	matrix m = create_matrix (4);
	queue q;
	
	m [1][0] = 1;
	m [2][0] = 4;
	m [3][0] = 7;
	m [2][1] = 2;
	m [3][1] = 8;
	m [3][2] = 3;
	
	for (j=0; j<4; j++)
		for (i=0; i<=j; i++)
			if (i == j) m [j][i] = 0;
			else m [i][j] = m [j][i];
			
	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) printf (" %d ", m [i][j]);
		printf ("\n");
	}


	show_queue (prim (m, 4, q));
}



void test2 () {
	
	int i, j;
	matrix m = create_matrix (5);
	queue q;
	
	m [1][0] = 1;
	m [2][0] = 8;
	m [3][0] = 4;
	m [4][0] = 7;
	m [2][1] = 2;
	m [3][1] = 6;
	m [4][1] = 5;
	m [3][2] = 9;
	m [4][2] = 5;
	m [4][3] = 3;
	
	for (j=0; j<5; j++)
		for (i=0; i<=j; i++)
			if (i == j) m [j][i] = 0;
			else m [i][j] = m [j][i];
			
	for (i=0; i<5; i++) {
		for (j=0; j<5; j++) printf (" %d ", m [i][j]);
		printf ("\n");
	}


	show_queue (prim (m, 5, q));
}



void test3 () {
	
	int i, j;
	matrix m = create_matrix (7);
	queue q;
	
	m [1][0] = 7;
	m [2][0] = 99;
	m [3][0] = 5;
	m [4][0] = 99;
	m [5][0] = 99;
	m [6][0] = 99;
	m [2][1] = 8;
	m [3][1] = 9;
	m [4][1] = 7;
	m [5][1] = 99;
	m [6][1] = 99;
	m [3][2] = 99;
	m [4][2] = 5;
	m [5][2] = 99;
	m [6][2] = 99;
	m [4][3] = 15;
	m [5][3] = 6;
	m [6][3] = 99;
	m [5][4] = 8;
	m [6][4] = 9;
	m [6][5] = 11;
	
	for (j=0; j<7; j++)
		for (i=0; i<=j; i++)
			if (i == j) m [j][i] = 0;
			else m [i][j] = m [j][i];
			
	for (i=0; i<7; i++) {
		for (j=0; j<7; j++) printf (" %2d ", m [i][j]);
		printf ("\n");
	}


	show_queue (prim (m, 7, q));
}



/*
	Time meassurements
*/

void test_time () {

	double t, t1, t2;
	int i, j = 100, k;
	queue edges;

	for (i = 1; i <= 5; i++) {
		matrix m = create_matrix (j);
		init_matrix (m, j);
		
		t1 = microseconds ();
		prim (m, j, edges);
		t2 = microseconds ();
		t = t2-t1;
		free_matrix (m, j);
			
		if (t < 500) {
			
			printf ("*");
			t1 = microseconds ();
			for (k = 1; k <= 10; k++) {
				m = create_matrix (j);
				init_matrix (m, j);
				prim (m, j, edges);
				free_matrix (m, j);
			}
			t2 = microseconds ();
			t = t2-t1;
			
			t1 = microseconds ();
			for (k = 1; k <= 10; k++) {
				m = create_matrix (j);
				init_matrix (m, j);
				free_matrix (m, j);
			}
			t2 = microseconds ();
			t = (t-(t2-t1))/10;
		}
	printf ("n: %4d  Time: %f	%f	%f	%f\n", j, t, t/pow(j,2.5), t/pow(j,1.75), t/j);

	j = j * 2;
	}
}





/*
	Main function 
*/

int main () {
	
	printf ("\nAlgorithm validation tests: \n\n");
	test1 ();
	test2 ();
	test3 ();
	printf ("\nTimes meassurement tests: \n\n");
	test_time ();

	return 0;
}
