#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void mat_vec_row(double* a, double* x, double* y, int n)
{
  int i,j;

  for(i=0;i<n;i++){
    y[i]=0.0;
    for(j=0;j<n;j++){
      y[i]+=a[n*i+j]*x[j];
    }
  } 
}

void mat_vec_row_row_decomp(double* a, double* x, double* y, int n)
{
  int i,j;
#pragma omp parallel for default(none) shared(a, x, y, n) private(i, j)
  for(i=0;i<n;i++){
    y[i]=0.0;
    for(j=0;j<n;j++){
      y[i]+=a[n*i+j]*x[j];
    }
  }  
}

void mat_vec_row_col_decomp(double* a, double* x, double* y, int n)
{
  int i,j;

  for(i=0;i<n;i++){
    double temp = 0.0;
#pragma omp parallel for default(none) shared(a, x, y, n, i) private(j) reduction(+:temp)
    for(j=0; j<n; j++){
      temp += a[n*i+j] * x[j];
    }
    y[i] = temp;
  }  
}

void mat_vec_col(double* a, double* x, double* y, int n)
{
  
  int i,j;

  for(i=0;i<n;i++) y[i]=0.0;
  for(j=0;j<n;j++){
    for(i=0;i<n;i++){
      y[i]+=a[i+j*n]*x[j];
    }
  } 
}

void mat_vec_col_col_decomp(double* a, double* x, double* y, int n)
{
  int i, j;
  for(i=0; i<n; i++) y[i] = 0.0;
  
  #pragma omp parallel default(none) shared(a, x, y, n) private(i, j)
  {
    double* y_local = (double*)calloc(n, sizeof(double));
    #pragma omp for
    for(j=0; j<n; j++){
      for(i=0; i<n; i++){
        y_local[i] += a[i+j*n] * x[j];
      }
    }
    #pragma omp critical
    {
      for(i=0; i<n; i++){
        y[i] += y_local[i];
      }
    }

    free(y_local);
  }
}

void mat_vec_col_row_decomp(double* a, double* x, double* y, int n)
{
  
  int i,j;
  for(i=0; i<n; i++) y[i] = 0.0;

  for(j=0; j<n; j++){
#pragma omp parallel for default(none) shared(a, x, y, n, j) private(i)
    for(i=0; i<n; i++){
      y[i] += a[i+j*n] * x[j];
    }
  }
}

