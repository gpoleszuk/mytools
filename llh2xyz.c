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
   long double f, e2, N;
   long double g2r = pi / 180.0;


   f  = 1.0 / invf;
   e2 = f * (2.0 -f);

   if(argc == 4 ){
      lat = atof(argv[1]) * g2r;
      lon = atof(argv[2]) * g2r;
      hgt = atof(argv[3]);


      N   = a / sqrt( 1.0 - e2 * sin(lat) * sin(lat));
      X   = (N + hgt) * cos(lat) * cos(lon);
      Y   = (N + hgt) * cos(lat) * sin(lon);
      Z   = (N * (1.0 - e2) + hgt) * sin(lat);

      printf("%16.9Lf %16.9Lf %16.9Lf\n", X, Y, Z);
      return 0;
   } else {
      fprintf(stderr, "\nUsage: \n        %s lat lon hgt\n", argv[0]);
             // 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
      fprintf(stderr, "Note: by default, uses the WGS84 ellipsoid parameters.\n");
      fprintf(stderr, "Note: a   : %15.4Lf\n      f^-1: %15.10Lf\n", a, invf);
      fprintf(stderr, "\n                                                  Version: %s\n", version);
      return -1;
   }
}
