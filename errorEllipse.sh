if [ $# -lt 3 ]; then
  echo "Usage:";
  echo "  errorEllipse.sh sxx   syy   sxy | gnuplot -p"
  echo "  errorEllipse.sh 4.837223   2.948404   0.6452550 | gnuplot -p"
  exit 1
fi

echo $1 $2 $3 | awk '{
  sx2 = $1;
  sy2 = $2;
  sxy = $3;

#  pi  = 4.0 * atan2(1.0,1.0);
#  pi = 3.14159265358979323846264338327950288419716939937508;
  pi = 3.1415926535897932;
  r2d = 180.0 / pi;
  d2r = 1.0 / r2d;

  n   = (sx2 - sy2);
  p   = (sx2 + sy2);
  m   = sqrt(0.25 * n * n + sxy * sxy);
  a   = sqrt(0.5 * p + m);
  b   = sqrt(0.5 * p - m);
  t1  = atan2(sxy, a*a - sx2);
  t1  = (t1 < 0.0)?pi+t1:t1;
#  t2  = -0.5 * atan2((2.0 * sxy), (sx2 - sy2));
#  t2  = (t2 < 0.0)?(pi/2.0)+t2:t2;
  sx = sqrt(sx2);
  sy = sqrt(sy2);
  rho = sxy / sqrt(sx2 * sy2);

  t    = t1;
  sint = sin(t);
  cost = cos(t);
  bx   = sqrt( a*a*cost*cost + b*b*sint*sint );
  by   = sqrt( a*a*sint*sint + b*b*cost*cost );

  printf("#sxsyr: %14.9f %14.9f %14.9f\n", sx, sy, rho);
  printf("#Bound: %14.9f %14.9f\n", bx, by);
  printf("#abt  : %14.9f %14.9f %14.9f\n", a, b, t1 * r2d);
  #printf("#%14.9f %14.9f %14.9f  %14.9f\n", sx2, sy2, sxy, rho * 100.0);

  printf("#\n");
  #a    = 10.0;
  #b    =  5.0;
  #t    = 60.0 * d2r;

  printf("$DATA <<EOD\n");
  for(i = 0; i<360; i++) {
    alpha = i * d2r;
    #y = sqrt(b*b - b*b*x*x/a*a);
    #x = sqrt(a*a - a*a*y*y/b*b);
    x  = a * cos(alpha);
    y  = b * sin(alpha);
    #printf("%14.9f   %14.9f %14.9f   %14.9f %14.9f\n", t, x, y, xr, yr);
    xr = x * cost - y * sint;
    yr = x * sint + y * cost;
    printf("%20.12E %20.12E\n", xr, yr);
  }
  max=(bx>by)?bx:by;
  max*=1.1;  # increase 10%
  printf("EOD\n");
  printf("set term svg size 400,400 font \"Times,12\"; set size square\n");
  printf("set grid; unset key;\n");
  printf("set xrange[%0.16E:%0.16E];\n", -max, max);
  printf("set yrange[%0.16E:%0.16E];\n", -max, max);
  printf("set object 1 rect from %0.16E,%0.16E to %0.16E,%0.16E lw 1 dt 2 fillstyle transparent solid 0.0;\n", -bx, -by, bx, by);
  printf("set output \"ellipse.svg\";\n");
  printf("plot $DATA u 1:2 with dots;\n");
  printf("unset output\n");
# x*x/a*a + y*y/b*b = 1.0
# x*x = a*a - a*a*y*y/b*b
# y*y = b*b - b*b*x*x/a*a
# x   = +/-sqrt(a*a - a*a*y*y/b*b)
# y   = +/-sqrt(b*b - b*b*x*x/a*a)

}'
