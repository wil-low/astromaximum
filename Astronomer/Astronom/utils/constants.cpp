void normAngle(double &a)
{
  while (a < 0)
	  a += 360.L;
  while (a >= 360)
	  a -= 360.L;
}
