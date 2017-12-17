// Gianluca Veschi
// Usuario: gianluca.veschi@udc.es


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>


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



struct node {
	int elem;
	int num_repetitions;
	struct node *left, *right;
};

typedef struct node * position;
typedef struct node * tree;


static struct node * createnode (int e) {
	
	struct node * p = malloc (sizeof (struct node));
	
	if (p == NULL) {
		printf("out of memory\n"); 
		exit (EXIT_FAILURE);
	}
	p->elem = e;
	p->num_repetitions = 1;
	p->left = NULL;
	p->right = NULL;
	return p;
}


tree insert(int e, tree t) {
	
	if (t == NULL) return createnode (e);
	else if (e < t->elem) t->left = insert(e, t->left);
	else if (e > t->elem) t->right = insert(e, t->right);
	else t->num_repetitions++;
	return t;
}


tree createtree () {

	struct node * t = malloc (sizeof (struct node));
	
	if (t == NULL) {
		printf ("Out of memory \n");
		exit (EXIT_FAILURE);
	}
	
	t->left = NULL;
	t->right = NULL;
	
	return t;
}


int isemptytree (tree a) {

	return (a == NULL);
}
	
	
position leftchild (tree t) {

	return t->left;
}
	
	
position rightchild (tree t) {

	return t->right;
}	
	
	
int element (position p) {

	return (p->elem);
}


position find(int elem, tree t)
{
	// Base Cases: t is null or elem is present at root
	if (t == NULL || t->elem == elem)
	return t;
	
	// elem is greater than root's key
	if (t->elem < elem)
	return find(elem, t->right);

	// elem is smaller than root's key
	return find(elem, t->left);
}



tree deletetree (tree t) {

	if (t->left != NULL) deletetree (t->left);
	if (t->right != NULL) deletetree(t->right);
	free (t);
	t -> left = NULL;
	t -> right = NULL;
	return t;
}


int numberofrepetitions (position p) {
    if (p==NULL) return 0;
    else return (p->num_repetitions);
}


int height (tree t) {
	
	int l = -1, r = -1;
	
	struct node * a = t->left;
	struct node * b = t->right;
	
	if (a != NULL) l = 1 + height (a);
	if (b != NULL) r = 1 + height (b);

	if (l < r) l = r;
	
	return l;
}


void visualize_node (tree t) {
	
	struct node * a = t->left;
	struct node * b = t->right;
	
	printf ("(");
	if (a != NULL) visualize_node (a);
	printf (" %d ", t->elem);
	if (b != NULL) visualize_node (b);
	printf (")");
}


void visualize (tree t) {
	
	struct node * a = t->left;
	struct node * b = t->right;
	
	if ((a == NULL) && (b == NULL)) {
		printf ("()");
		return;
	}
	if (a != NULL) visualize_node (a);
	if (b != NULL) visualize_node (b);
}



void test () {

	tree mytree = createtree();
	
	printf ("empty tree: ");
	visualize (mytree);
	
	printf ("\ntree height: %d", height (mytree));
	
	printf ("\ninserting a 3");
	insert (3, mytree);	
	printf ("\ninserting a 1");
	insert (1, mytree);	
	printf ("\ninserting a 2");
	insert (2, mytree);	
	printf ("\ninserting a 5");
	insert (5, mytree);	
	printf ("\ninserting a 4");
	insert (4, mytree);	
	printf ("\ninserting a 5");
	insert (5, mytree);
	
	printf ("\ntree: ");
	visualize (mytree);
	
	printf ("\ntree height: %d", height (mytree));
	
	if (numberofrepetitions (find (1, mytree)) > 0) printf ("\nsearching for 1 and finding 1 repeated: %d times", numberofrepetitions (find (1, mytree)));
	if (numberofrepetitions (find (2, mytree)) > 0) printf ("\nsearching for 2 and finding 2 repeated: %d times", numberofrepetitions (find (2, mytree)));
	if (numberofrepetitions (find (3, mytree)) > 0) printf ("\nsearching for 3 and finding 3 repeated: %d times", numberofrepetitions (find (3, mytree)));
	if (numberofrepetitions (find (4, mytree)) > 0) printf ("\nsearching for 4 and finding 4 repeated: %d times", numberofrepetitions (find (4, mytree)));
	if (numberofrepetitions (find (5, mytree)) > 0) printf ("\nsearching for 5 and finding 5 repeated: %d times", numberofrepetitions (find (5, mytree)));
	if (numberofrepetitions (find (6, mytree)) <= 0) printf ("\nsearching for 6 and finding 6 repeated: %d times", numberofrepetitions (find (6, mytree)));
	
	printf ("\ndeleting all nodes, freeing memory ");
	deletetree (mytree);
	
	printf ("\nempty tree: ");
	visualize (mytree);
	
	printf ("\ntree height: %d", height (mytree));
	
	printf ("\n\n");
}

	

void time_insert (int n) {
	
	int v [2*n], i;
	double t1, t2;
	tree mytree = createtree ();
	
	init_seed ();
	random_init (v, n);
	
	t1 = microseconds ();
	for (i = 0; i <= 2*n; i++) insert (v[i], mytree);
	t2 = microseconds ();
	
	printf ("%d\t%f\t%f\t%f\t%f\n", n, t2-t1, (t2-t1)/pow(n,1.5), (t2-t1)/(pow(n,1.25)), (t2-t1)/n);
}


void time_search (int n) {	
	
	int v [2*n], i;
	double t1, t2;
	tree mytree = createtree ();
	
	init_seed ();
	random_init (v, n);
	
	for (i = 0; i <= 2*n; i++) insert (v[i], mytree);
		
	init_seed ();
	random_init (v, n);
	
	t1 = microseconds ();
	for (i = 0; i <= 2*n; i++) find (v[i], mytree);
	t2 = microseconds ();
	printf ("%d\t%f\t%f\t%f\t%f\n", n, t2-t1, (t2-t1)/pow(n,1.5), (t2-t1)/(pow(n,1.25)), (t2-t1)/n);
	
} 


void time_testing_call () {

	int n, i;
	
	n = 8000;
	printf ("n\tt(n) inserting\tt(n)/pow(n,1.5)\tt(n)/pow(n,1.25)t(n)/n\n");
	for (i = 0; i < 6; i++) {
		time_insert (n);
		n = n*2;
	}
	printf ("\n");
	
	n = 8000;
	printf ("n\tt(n) seraching\tt(n)/pow(n,1.5)\tt(n)/pow(n,1.25)t(n)/n\n");
	for (i = 0; i < 6; i++) {
		time_search (n);
		n = n*2;
	}
}




int main () {
	
	test ();
	time_testing_call ();
	return 0;
}
