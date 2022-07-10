#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define version "0.1.1 (20210817T0137Z)"


int main(int argc, char* argv[]) {
   long double X0, Y0, Z0, X1, Y1, Z1,
               dX, dY, dZ,
               lat0, lon0, hgt0;
   long double a    = 6378137.0,
          invf = 1.0 / 3.35281066474E-3;
          invf = 298.25722210088;
   long double pi   = 3.1415926535898;
   long double p, f, e2, N, phi0;

   long double e, n, u, s01, Az01, z01;

   f  = 1.0 / invf;
   e2 = f * (2.0 -f);

   if(argc == 7 ){
      //for(int i=0; i<argc; i++)
         // printf("[%s ]\n", argv[i]);
         X0 = atof(argv[1]);
         Y0 = atof(argv[2]);
         Z0 = atof(argv[3]);
         X1 = atof(argv[4]);
         Y1 = atof(argv[5]);
         Z1 = atof(argv[6]);
/*
         printf("Your inputs: \n");
         printf("X0   [%16.4Lf ]\n", X0);
         printf("Y0   [%16.4Lf ]\n", Y0);
         printf("Z0   [%16.4Lf ]\n", Z0);
         printf("X1   [%16.4Lf ]\n", X1);
         printf("Y1   [%16.4Lf ]\n", Y1);
         printf("Z1   [%16.4Lf ]\n", Z1);
*/
         dX   = X1 - X0;
         dY   = Y1 - Y0;
         dZ   = Z1 - Z0;

         p    = sqrtl(X0 * X0 + Y0 * Y0);
         phi0 = atan2l(Z0, (p * (1.0 - e2)) );
         for(int iter=0; iter<10; iter++) {
            N    = a / sqrt(1.0 - e2 * sin(phi0) * sin(phi0));
            hgt0 = p / cos(phi0) - N;
            phi0 = atan2l(Z0, (p * (1.0 - e2 * N / (N + hgt0))));

            lat0 = phi0;
         }
         lon0 = atan2l(Y0, X0);


// topocentric to local horizon system  (c.f. Ben notes)

//      n = -sla*clo*px -sla*slo*py +cla*pz;
//      e =     -slo*px +    clo*py       ;
//      u =  cla*clo*px +cla*slo*py +sla*pz;

//      va =Math.asin(u/rho);
//      az =Math.atan2(e,n);


         e    =             -sin(lon0) * dX +             cos(lon0) * dY +       0.0 * dZ;
         n    = -sin(lat0) * cos(lon0) * dX - sin(lat0) * sin(lon0) * dY + cos(lat0) * dZ;
         u    =  cos(lat0) * cos(lon0) * dX + cos(lat0) * sin(lon0) * dY + sin(lat0) * dZ;


//       Kenu = Rs x Kxyz * Rs'

         printf("\nR' -----------------------------------------------------------------\n");
         printf("n    %20.15f %20.15f %20.15f   -sin(lat0)*cos(lon0) | -sin(lat0)*sin(lon0) | cos(lat0)\n", -sin(lat0) * cos(lon0), -sin(lat0) * sin(lon0), cos(lat0) );
         printf("e    %20.15f %20.15f %20.15f             -sin(lon0) |            cos(lon0) |         0\n",             -sin(lon0),              cos(lon0),       0.0 );
         printf("u    %20.15f %20.15f %20.15f    cos(lat0)*cos(lon0) |  cos(lat0)*sin(lon0) | sin(lat0)\n",  cos(lat0) * cos(lon0),  cos(lat0) * sin(lon0), sin(lat0) );
         printf("--------------------------------------------------------------------\n\n");

         s01  = sqrt(e * e + n * n + u * u);
         Az01 = atan2l(e, n);
         if(Az01<0.0) Az01 += 2.0 * pi;
         z01  = acosl(u / s01);
/*
         printf("My outputs: \n");
         printf("Lat0 [%16.9Lf ]\n", lat0 * 180.0 / pi);
         printf("Lon0 [%16.9Lf ]\n", lon0 * 180.0 / pi);
         printf("hgt0 [%16.9Lf ]\n", hgt0);
         printf("ENU coordinates: \n");
         printf("e    [%16.9Lf ]\n", e);
         printf("n    [%16.9Lf ]\n", n);
         printf("u    [%16.9Lf ]\n", u);
*/
         printf("a f⁻1       = %16.6Lf %16.11Lf\n", a, invf );
         printf("XYZ0 XYZ1   = %16.6Lf %16.6Lf %16.6Lf %16.6Lf %16.6Lf %16.6Lf\n", X0, Y0, Z0, X1, Y1, Z1 );
         printf("dxyz        = %16.6Lf %16.6Lf %16.6Lf \n", dX, dY, dZ);
         printf("lat lon hgt = %16.10Lf %16.10Lf %16.10Lf\n", lat0 * 180.0 / pi, lon0 * 180.0 / pi, hgt0);
         printf("enu         = %16.9Lf %16.9Lf %16.9Lf\n", e, n, u);
         printf("s Az z      = %16.6Lf %16.10Lf %16.10Lf\n", s01, Az01 * 180.0 / pi, z01 * 180.0 / pi);
//         printf("%16.9Lf %16.9Lf %16.9Lf \n%16.6Lf %16.6Lf %16.6Lf %16.6Lf %16.6Lf %16.6Lf \n%16.10Lf %16.10Lf %16.10Lf %16.6Lf %16.10Lf %16.10Lf \n", e, n, u, X0, Y0, Z0, X1, Y1, Z1, lat0 * 180.0 / pi, lon0 * 180.0 / pi, hgt0, s01, Az01 * 180.0 / pi, z01 * 180.0 / pi);
         return 0;
   } else {
         fprintf(stderr, "\nUsage: \n        %s X0 Y0 Z0 X1 Y1 Z1\n", argv[0]);
                // 123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
         fprintf(stderr, "Note: by default, uses the WGS84 ellipsoid parameters.\n");
         fprintf(stderr, "Note: a   : %15.4Lf\n      f^-1: %15.10Lf\n", a, invf);
         fprintf(stderr, "\n                                                  Version: %s\n", version);
         return -1;
   }
}
