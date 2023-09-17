#!/bin/bash

echo "User Input $# parameters"
echo "Inputs: $@"

echo "    X0 Y0 Z0 dl Az  v sX0 sY0 sZ0   sXY   sXZ   sYZ  sdl   sAz    sv"
#    X0 Y0 Z0 dl Az  v sX0 sY0 sZ0   sXY   sXZ   sYZ  sdl   sAz    sv
echo $1 $2 $3 $4 $5 $6  $7  $8  $9 ${10} ${11} ${12} ${13} ${14} ${15}| awk '{
#  pi  = 3.14159265358979323846264338327950288419716939937508;
  pi  = 3.1415926535897932;
  r2d = 180.0 / pi;
  d2r = 1.0 / r2d;

  X0 = $1;  sX0 = $7;  sXY = $10;
  Y0 = $2;  sY0 = $8;  sXZ = $11;
  Z0 = $3;  sZ0 = $9;  sYZ = $12;
  dl = $4;  sdl = $13;
  Az = $5;  sAz = $14;
  v  = $6;  sv  = $15;

  printf("Input data\n");
  printf("X0: %14.9f +/- %14.9f\n", X0, sX0);
  printf("Y0: %14.9f +/- %14.9f   %14.9f\n", Y0, sY0, sXY);
  printf("Z0: %14.9f +/- %14.9f   %14.9f %14.9f\n", Z0, sZ0, sXZ, sYZ);
  printf("dl: %14.9f +/- %14.9f\n", dl, sdl);
  printf("Az: %14.9f +/- %14.9f\n", Az, sAz);
  printf("v : %14.9f +/- %14.9f\n", v, sv);

  Az  =  Az * d2r;
  v   =   v * d2r;
  sAz = sAz * d2r;
  sv  =  sv * d2r;

  X1 = X0 + dl * sin(v) * sin(Az);
  Y1 = Y0 + dl * sin(v) * cos(Az);
  Z1 = Z0 + dl * cos(v);

  for(i = 1; i<=18; i++) {   F[i] = 0.0; }
  for(i = 1; i<=36; i++) { Kll[i] = 0.0; }
  for(i = 1; i<=18; i++) {  FK[i] = 0.0; }
  for(i = 1; i<=9;  i++) { Kff[i] = 0.0; }

  # for(i=1; i<=18; i++) { printf("%14.9f%s", F[i],   (i%6)!=0?" ":"\n"); }; printf("\n");
  # for(i=1; i<=36; i++) { printf("%14.9f%s", Kll[i], (i%6)!=0?" ":"\n"); }; printf("\n");
  # for(i=1; i<=18; i++) { printf("%14.9f%s", FK[i],  (i%6)!=0?" ":"\n"); }; printf("\n");
  # for(i=1; i<=9;  i++) { printf("%14.9f%s", Kff[i], (i%3)!=0?" ":"\n"); }; printf("\n");

  cosAz = cos(Az);
  sinAz = sin(Az);
  cosv  = cos(v);
  sinv  = sin(v);

  F[ 1] =  1.0;
  F[ 8] =  1.0;
  F[15] =  1.0;
  F[ 4] =     sinv*sinAz;
  F[ 5] =  dl*sinv*cosAz;
  F[ 6] =  dl*cosv*sinAz;
  F[10] =     sinv*cosAz;
  F[11] = -dl*sinv*sinAz;
  F[12] =  dl*cosv*cosAz;
  F[16] =     cosv;
  F[18] = -dl*sinv;

#           X0   Y0   Z0              dl                  Az                   v
#       |  1.0  0.0  0.0  sin(v)*sin(Az)   dl*sin(v)*cos(Az)   dl*cos(v)*sin(Az) |
#   F = |  0.0  1.0  0.0  sin(v)*cos(Az)  -dl*sin(v)*sin(Az)   dl*cos(v)*cos(Az) |
#       |  0.0  0.0  1.0  cos(v)                         0.0  -dl*sin(v)         |
#

#
#       | sXX  sXY  sXZ                |
#       | sXY  sYY  sYZ                |
# Kll = | sXZ  sYZ  sZZ                |
#       |   0    0    0  sdl           |
#       |   0    0    0    0  sAz      |
#       |   0    0    0    0    0   sv |
#

  Kll[ 1] = sX0 * sX0;
  Kll[ 2] = sXY;
  Kll[ 3] = sXZ;
  Kll[ 7] = sXY;
  Kll[ 8] = sY0 * sY0;
  Kll[ 9] = sYZ;
  Kll[13] = sXZ;
  Kll[14] = sYZ;
  Kll[15] = sZ0 * sZ0;
  Kll[22] = sdl * sdl;
  Kll[29] = sAz * sAz;
  Kll[36] = sv  *  sv;

# Matrix print
  printf("\n");
  for(i=1; i<=18; i++) { printf("%24.16E%s", F[i], (i%6)!=0?" ":"\n"); }
  printf("\n");

# Matrix print
  for(i=1; i<=36; i++) { printf("%24.16E%s", Kll[i], (i%6)!=0?" ":"\n"); }
  printf("\n");

# Matrix inverse
#  n=6;
#  for(k=1; k<=n; k++) {
#    for(j=1; j<=n; j++)
#      if(j!=k) A[(k-1)*n+j] = A[(k-1)*n+j]/A[(k-1)*n+k];
#    A[(k-1)*n+k] = 1.0/A[(k-1)*n+k];
#    for(i=1; i<=n; i++)
#      if(i!=k) {
#        for(j=1;j<=n;j++)
#          if(j!=k) A[(i-1)*n+j]=A[(i-1)*n+j]-A[(i-1)*n+k]*A[(k-1)*n+j];
#        A[(i-1)*n+k]=-A[(i-1)*n+k]*A[(k-1)*n+k];
#
#      }
#  }

# Matrix print
#  n = 6;
#  for(i=1; i<=n*n; i++) { printf("%14.7E%s", A[i], (i%n)!=0?" ":"\n"); }
#  printf("\n");

#  n = 6;
# Check inverse matrix
#  for(i=1; i<=n; i++) {  for(j=1; j<=n; j++) {  for(k=1; k<=n; k++) {
#        C[(i-1)*n+j] = C[(i-1)*n+j] + A[(i-1)*n+k] * Kll[(k-1)*n+j];
#      }    }  }

# Matrix clear identity garbage off-diagonal and print
#  for(i=1; i<=n*n; i++) {
#    if(C[i]<1E-17) C[i]=0.0;  # It is not correct to do
#    printf("%14.7E%s", C[i], (i%n)!=0?" ":"\n");
#  }
#  printf("\n");


# Matrix mult F x Kll
  for(i=1; i<=3; i++) {  for(j=1; j<=6; j++) {  for(k=1; k<=6; k++) {
        FK[(i-1)*6+j] = FK[(i-1)*6+j] + F[(i-1)*6+k] * Kll[(k-1)*6+j];
      }    }  }

# Matrix mult F x Kll x F
  for(i=1; i<=3; i++) {  for(j=1; j<=3; j++) {  for(k=1; k<=6; k++) {
        Kff[(i-1)*3+j] = Kff[(i-1)*3+j] + FK[(i-1)*6+k] * F[(j-1)*6+k];
      }    }  }

#  for(i=1; i<=18; i++) { printf("%14.9f%s", FK[i], (i%6)!=0?" ":"\n"); }
#  printf("\n");

  printf("\n");
  for(i=1; i<=9; i++) { printf("%24.16E%s", Kff[i], (i%3)!=0?" ":"\n"); }
  printf("\n");


  sX1 = sqrt(Kff[(1-1)*3+1]);
  sY1 = sqrt(Kff[(2-1)*3+2]);
  sZ1 = sqrt(Kff[(3-1)*3+3]);
  sYX =     (Kff[(1-1)*3+2]);
  sZX =     (Kff[(1-1)*3+3]);
  sZY =     (Kff[(2-1)*3+3]);

  printf("Print results\n");
  printf("X1: %14.9f +/- %14.9f\n", X1, sX1);
  printf("Y1: %14.9f +/- %14.9f   %14.9f\n", Y1, sY1, sYX);
  printf("Z1: %14.9f +/- %14.9f   %14.9f %14.9f\n", Z1, sZ1, sZX, sZY);
  printf("\n");
  printf("For error ellipse: %14.6E %14.6E %14.6E  %14.9f\n", Kff[1], Kff[5], Kff[2], 100.0*Kff[2]/sqrt(Kff[1]*Kff[5]));
}'
