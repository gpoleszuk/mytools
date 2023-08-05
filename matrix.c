/*
Compile with
  gcc matrix_inverse.c -o matrix_inverse.e
Execute
./matrix_inverse.e
*/

#include<stdio.h>


int main(void) {
  int n=4;
  //double A[4][4] = {{1, 2, 5, 7}, {2, 3, 1, 0}, {5, 1, 1, 9}, {7, 0, 9, 1}};
  long double A[4][4] = {{1, 1, 1, 1}, {1, 3, 4, 1}, {1, 4, 1, 0}, {1, 1, 0, 1}};
  long double B[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  long double C[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

  /********************************************************/
  // Print
  /********************************************************/
  printf("\n A\n");
  for(int i=0; i<n; i++) {
    for(int j=0; j<n; j++)
      printf("%23.16LE ", A[i][j]);
    printf("\n");
  }
  /********************************************************/

  /********************************************************/
  // Copy
  /********************************************************/
  printf("\n B = A\n");
  for(int i=0; i<n; i++)
    for(int j=0; j<n; j++)
      B[i][j]=A[i][j];
  printf("\n");
  /********************************************************/

  /********************************************************/
  // Print
  /********************************************************/
  printf("\n B\n");
  for(int i=0; i<n; i++) {
    for(int j=0; j<n; j++) {
      B[i][j]=A[i][j];
      printf("%23.16LE ", B[i][j]);
    }
    printf("\n");
  }
  /********************************************************/


  /********************************************************/
  // Transpose
  // C = A'
  /********************************************************/
  printf("\n C = A'\n");
  for(int i=0; i<n; i++)
    for(int j=0; j<n; j++)
        C[i][j]=A[j][i];
  /********************************************************/

  for(int i=0; i<n; i++) {
    for(int j=0; j<n; j++)
      printf("%23.16LE ", C[i][j]);
    printf("\n");
  }

  /********************************************************/
  // Inverse (with no pivoting)
  // Inv(A)
  // Ghilani & Wolf, 2006 pg. 541
  /********************************************************/
  printf("\n B = inv(A)\n");
  for(int k=0; k<n; k++) {
    for(int j=0; j<n; j++) {
      if(j!=k) B[k][j] = B[k][j]/B[k][k];
    }
    B[k][k] = 1.0/B[k][k];
    for(int i=0; i<n; i++)
      if(i!=k) {
        for(int j=0; j<n; j++)
          if(j!=k) B[i][j]=B[i][j]-B[i][k]*B[k][j];
        B[i][k]=-B[i][k]*B[k][k];
      }
  }
  /********************************************************/

  for(int i=0; i<n; i++) {
    for(int j=0; j<n; j++)
      printf("%23.16LE ", B[i][j]);
    printf("\n");
  }

  /********************************************************/
  // Mul
  // Multiply C = A x B
  /********************************************************/
  printf("\n C = A x B\n");
  for(int i=0; i<n; i++) {
    for(int j=0; j<n; j++) {
      C[i][j]=0.0;
      for(int k=0; k<n; k++)
        C[i][j]=C[i][j]+A[i][k]*B[k][j];
    }
  }
  /********************************************************/


  for(int i=0; i<n; i++) {
    for(int j=0; j<n; j++)
      printf("%23.16LE ", C[i][j]);
    printf("\n");
  }
  /********************************************************/


  /********************************************************/
  // Add
  // Sum C = A + B
  /********************************************************/
  printf("\n C = A + B\n");
  for(int i=0; i<n; i++)
    for(int j=0; j<n; j++)
        C[i][j]=A[i][j]+B[i][j];

  for(int i=0; i<n; i++) {
    for(int j=0; j<n; j++)
      printf("%23.16LE ", C[i][j]);
    printf("\n");
  }
  /********************************************************/


  /********************************************************/
  // Sub
  // Sub C = A - B
  /********************************************************/
  printf("\n C = A - B\n");
  for(int i=0; i<n; i++)
    for(int j=0; j<n; j++)
        C[i][j]=A[i][j]-B[i][j];

  for(int i=0; i<n; i++) {
    for(int j=0; j<n; j++)
      printf("%23.16LE ", C[i][j]);
    printf("\n");
  }
  /********************************************************/

  return 0;
}

  /********************************************************/
  // ToDo:
  // Det
  // Det(A)
  /********************************************************/

/**********************************************************/
// Since we are not obliged to use social networks
// would we be safer ignoring them? DT8FV
/**********************************************************/