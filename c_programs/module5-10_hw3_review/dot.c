
#include <argp.h>
#include <math.h>
#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Args {
  size_t length;
  size_t nreps;
  bool block;
  size_t unroll_factor;
};

// static memory variable declared here
static struct argp_option options[] = {
  {"length", 'S', "size_t", 0, "Length of each vector"},
  {"nreps", 'r', "size_t", 0, "Number of repetitions"},
  {"block", 'b', NULL, 0, "Compute block dot products (versus a single dot product)"}
};

static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
  struct Args *args = state->input;
  switch (key) {
  case ARGP_KEY_INIT:
    args->length = 24;
    args->nreps = 10;
    args->block = false;
    break;
  case 'S':
    args->length = strtol(arg, NULL, 10);
    break;
  case 'r':
    args->nreps = strtol(arg, NULL, 10);
    break;
  case 'b':
    args->block = true;
    break;
  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

int I = 8;
int J = 4;
double tol = 1e-6;

int c_length[] = {3, 4};


// Function declarations here
double dot_product(int S, const double *a, const double *b);
double vector_norm(const double *a, int S);
int is_orthogonal(int S, const double *a, const double *b, double tol);
void bdot(int I, int S, int J, const double *m, const double *n, double *p);

double dot_product(int S, const double *a, const double *b){

    double sum = 0;
    for (int i = 0; i<S; i++)
        sum += a[i] * b[i];

    return sum;
}

double vector_norm(const double *a, int S){

    double sum = 0;
    for (int i = 0; i<S; i++)
        sum += pow(a[i],2);

    return sqrt(sum);
}

int is_orthogonal(int S, const double *a, const double *b, double tol){
    
    return (fabs(dot_product(S, a, b) <= tol));
 }

// Performs the operation
//   P = M * N
// where M and N have shape (I,S) and (S,J) respectively.
// This version stores M as row-major and N as column-major.
void bdot(int I, int S, int J, const double *m, const double *n, double *p){

    for (int i=0; i<I; i++) {
        for (int j=0; j<J; j++) {
            p[i*J+j] = dot_product(S, &m[i*S], &n[j*S]);
        }
    }
}

static void init_bdot(int I, int S, int J, double *m, double *n) {
    for (int s=0; s<S; s++) {
        for (int i=0; i<I; i++)
            m[i*S + s] = 1000*(i+1) + s+1;
        for (int j=0; j<J; j++)
            n[j*S + s] = 1./(1000*(j+1) + s+1);
    }
}


// Reference matrix-matrix multiply product implementation
void matrix_ref(int I, int S, int J, const double *m, const double *n, double *p){
    for (int i=0; i<I; i++) {
        for (int j=0; j<J; j++) {
            p[i*J+j] = 0.0;
            for (int s=0; s<S; s++) {
                p[i*J+j] += m[i*S + s] * n[j*S + s];
            }
        }
    }
}

static void report_dot(const char *name, const double result, const double ref_result) {

  if (fabs(result - ref_result) > 1e-10) {
    printf("Result = %f failed to validate with expected value %f\n", result, ref_result);
    return;
  }
  printf("%s matches the reference result.\n", name);
}

static void report_is_orthogonal(const char *name, const int result, const int ref_result) {
  if (result != ref_result) {
    printf("Result = %d failed to validate with expected value %d \n", result, ref_result);
    return;
  }
  printf("%s matches the reference result. The two vectors are %s \n", name, ref_result ? "orthogonal.": "not orthogonal.");
}

static void report_vector_norm(const char *name, const double result, const double ref_result) {

  if (fabs(result - ref_result) > 1e-10) {
    printf("Result = %f failed to validate with expected value %f\n", result, ref_result);
    return;
  }
  printf("%s matches the reference result.\n", name);
}

static void report_bdot(const char *name, int I, int J, const double *result, const double *ref_result) {
  if (result && ref_result && result != ref_result) {
    for (int i=0; i<I; i++) {
      for (int j=0; j<J; j++) {
        if (fabs(result[i*J + j] - ref_result[i*J + j]) > 1e-10) {
          printf("Result[%d,%d] = %f failed to validate with expected value %f\n", i, j, result[i*J + j], ref_result[i*J + j]);
          return;
        }
      }
    }
  }
  printf("%s matches the reference result.\n", name);
}


#define REPORT_BDOT(f, I, S, J, m, n, p, p_ref) do { \
        f(I, S, J, m, n, p);                         \
        report_bdot(#f, I, J, p, p_ref);             \
} while (0)

int main(int argc, char **argv){

    struct Args args;
    struct argp argp = {options, parse_opt, NULL, NULL};
    argp_parse(&argp, argc, argv, 0, 0, &args);
    size_t S = args.length;

    switch (args.block) {
    case false: { // single dot product case
        // stack memory variable declarations here
        double a1[] = {1, 0, 0};
        double b1[] = {0, 1, 0};
        double a2[] = {1, 2, 3, 4};
        double b2[] = {1, 1, 1, 1};

        // result of (a1,b1) and (a2,b2) dot products and reference values
        double c1, c2;
        double c1_ref = 0;
        double c2_ref = 10;

        // result of is_orthogonal 
        int flag1, flag2;
        int flag1_ref = 1; // a1 and b1 are orthogonal
        int flag2_ref = 0; // a2 and b2 are not orthogonal

        // result for vector_norm
        double na1, na2, nb1, nb2;
        double na1_ref = 1.0;
        double na2_ref = 5.477225575051661;
        double nb1_ref = 1.0;
        double nb2_ref = 2.0;

        // calls to your functions by reference here
        // print statements to show results
        c1 = dot_product(c_length[0], a1, b1);                               
        report_dot("dot_product of a1 and b1", c1, c1_ref); 
        c2 = dot_product(c_length[1], a2, b2);                               
        report_dot("dot_product of a2 and b2", c2, c2_ref); 

        flag1 = is_orthogonal(c_length[0], a1, b1, tol);
        report_is_orthogonal("is_orthogonal between a1 and b1", flag1, flag1_ref); 
        flag2 = is_orthogonal(c_length[1], a2, b2, tol);
        report_is_orthogonal("is_orthogonal between a2 and b2", flag2, flag2_ref); 

        na1 = vector_norm(a1, c_length[0]);
        nb1 = vector_norm(b1, c_length[0]);
        na2 = vector_norm(a2, c_length[1]);
        nb2 = vector_norm(b2, c_length[1]);
        report_vector_norm("vector_norm of a1", na1, na1_ref);
        report_vector_norm("vector_norm of a2", na2, na2_ref);
        report_vector_norm("vector_norm of b1", nb1, nb1_ref);
        report_vector_norm("vector_norm of b2", nb2, nb2_ref);

        } break;
        case true: { // blocked dot product case
            // Initialize the matrices (as flattened vectors)
            // heap memory allocations here
            double *m = malloc(I * S * sizeof(double));
            double *n = malloc(J * S * sizeof(double));
            double *p = malloc(I * J * sizeof(double));
            double *p_ref = malloc(I * J * sizeof(double));

            init_bdot(I, args.length, J, m, n);
            matrix_ref(I, S, J, m, n, p_ref);
            REPORT_BDOT(bdot, I, S, J, m, n, p, p_ref);

            // free allocated heap memory here
            free(m); free(n); free(p); free(p_ref);
    } break;
    }

    return 0;
}

