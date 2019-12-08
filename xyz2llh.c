#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define version "0.1.0 (20191128T1428Z)"


int main(int argc, char* argv[]) {
   long double X, Y, Z,
               lat, lon, hgt;
   long double a    = 6378137.0,
          invf = 1.0 / 3.35281066474E-3;
   long double pi   = 3.1415926535898;
   long double p, f, e2, N, phi0;

   f  = 1.0 / invf;
   e2 = f * (2.0 -f);

   if(argc == 4 ){
         X = atof(argv[1]);
         Y = atof(argv[2]);
         Z = atof(argv[3]);

         p    = sqrtl(X * X + Y * Y);
         phi0 = atan2l(Z, (p * (1.0 - e2)) );
         for(int iter=0; iter<10; iter++) {
            N    = a / sqrt(1.0 - e2 * sin(phi0) * sin(phi0));
            hgt = p / cos(phi0) - N;
            phi0 = atan2l(Z, (p * (1.0 - e2 * N / (N + hgt))));

            lat = phi0;
         }
         lon = atan2l(Y, X);

         printf("%16.10Lf %16.10Lf %16.10Lf \n", lat * 180.0 / pi, lon * 180.0 / pi, hgt);
         return 0;
   } else {
         fprintf(stderr, "\nUsage: \n        %s X Y Z\n", argv[0]);
                // 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
         fprintf(stderr, "Note: by default, uses the WGS84 ellipsoid parameters.\n");
         fprintf(stderr, "Note: a   : %15.4Lf\n      f^-1: %15.10Lf\n", a, invf);
         fprintf(stderr, "\n                                                  Version: %s\n", version);
         return -1;
   }
}
