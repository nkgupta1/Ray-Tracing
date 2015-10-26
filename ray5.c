//
// Gradient Shadow with dot product
//
#include <stdio.h>
#include <math.h>
//
#define WIDTH 640
#define HIGHT 480
//values
//Eye
#define ex 0.50000
#define ey 0.50000
#define ez -1.0000
//Screen
#define pz 0.00000
//Floor
#define fy 0.333333
//Light
#define lx 0.00000
#define ly 1.00000
#define lz -0.50000
//Sphere 0
#define sphereNum 3
double cxA[sphereNum];
double cyA[sphereNum];
double czA[sphereNum];
double  rA[sphereNum];
int    red[sphereNum];
int  green[sphereNum];
int   blue[sphereNum];

//
void colorSphere(int num, int r, int g, int b)
{
   red[num] = r;
   green[num] = g;
   blue[num] = b;
}
void defineSpheres()
{
   cxA[0] = 0.500000;
   cyA[0] = 0.500000;
   czA[0] = 0.166667;
    rA[0] = 0.166667;
   colorSphere(0, 0, 0, 255);

   cxA[1] = 0.833333;
   cyA[1] = 0.500000;
   czA[1] = 0.500000;
    rA[1] = 0.166667;
   colorSphere(1, 0, 255, 0);

   cxA[2] = 0.333333;
   cyA[2] = 0.666667;
   czA[2] = 0.666667;
    rA[2] = 0.333333;
   colorSphere(2, 255, 0, 0);
}

void getcolor(int* rgb, int y, int x)
{
   double py,px;  //screen coor
   double rx,ry,rz;  //rays
   double mag;
   double b, c;
   double cx,cy,cz,r;   //sphere stuff
   int s;
   double det; //determinant  
   double posT, negT, minT;
   double nx, ny, nz;  //normals
   double ptx, pty, ptz;
   int onSphere = 0;
   int onFloor  = 0;

   py = 1 - (y*1.0 / (HIGHT));

   onSphere = 0;
   px = x*1.0 / (WIDTH);

   //ray direction
   rx = px - ex;
   ry = py - ey;
   rz = pz - ez;
   mag = sqrt(rx*rx + ry*ry + rz*rz);
   //normalize
   rx = rx / mag;
   ry = ry / mag;
   rz = rz / mag;

   
   minT = 999999;
   int temp;
   for(temp = 0; temp < 3; temp++) rgb[temp] = 0;

   //floor
   double T = (fy - ey) / ry;
   if(T >= 0)
   {
      minT = T;
      rgb[0] = 122;
      rgb[1] = 122;
      rgb[2] = 0;

      nx = 0;
      ny = 1;
      nz = 0;

      onFloor = 1;
   }

   //spheres
   for(s = 0; s < sphereNum; s++)
   {
      det = -1;
      cx = cxA[s];
      cy = cyA[s];
      cz = czA[s];
       r =  rA[s];
      b = 2*(rx*(ex-cx) + ry*(ey-cy)+ rz*(ez-cz));
      c = (ex-cx)*(ex-cx) + (ey-cy)*(ey-cy) + (ez-cz)*(ez-cz) - r*r;
      det = b*b - 4*c;  //a = 1

      if(det < 0) continue;
      else
      {
         posT = (-b + sqrt(det)) / 2;  //a = 1
         negT = (-b - sqrt(det)) / 2;
         if(posT > 0 && posT < minT)
         {
            minT = posT;
            onSphere = 1;
            onFloor  = 0;
            rgb[0] = red[s]/2.0;   //half color is ambient
            rgb[1] = green[s]/2.0;
            rgb[2] = blue[s]/2.0;

            ptx = rx*minT+ex;
            pty = ry*minT+ey;
            ptz = rz*minT+ez;

            nx = (ptx - cx) / r;
            ny = (pty - cy) / r;
            nz = (ptz - cz) / r;
         }
         if(negT > 0 && negT < minT)
         {
            minT = negT;
            onSphere = 1;
            onFloor  = 0;
            rgb[0] = red[s]/2.0;
            rgb[1] = green[s]/2.0;
            rgb[2] = blue[s]/2.0;

            ptx = rx*minT+ex;
            pty = ry*minT+ey;
            ptz = rz*minT+ez;

            nx = (ptx - cx) / r;
            ny = (pty - cy) / r;
            nz = (ptz - cz) / r;
         }
      }
   }

   //shadows

   //fixing self intersection

   ptx = rx*minT+ex;
   pty = ry*minT+ey;
   ptz = rz*minT+ez;

   ptx += nx * .01;
   pty += ny * .01;
   ptz += nz * .01;
   
   if(onFloor)
   {
      if(((int)(ptx / .1 - (ptx < 0)) + (int)(ptz / .1 - (ptz < 0))) % 2 == 0)
      {
         rgb[0] = 0;
         rgb[1] = 122;
         rgb[2] = 122;
      }
   }

   //ray direction

   rx = lx - ptx;
   ry = ly - pty;
   rz = lz - ptz;
   mag = sqrt(rx*rx + ry*ry + rz*rz);
   //normalize
   rx = rx / mag;
   ry = ry / mag;
   rz = rz / mag;

   int gradient = 1;   //1 = no shadow; 0 = gradient
   for(s = 0; s < sphereNum; s++)
   {
      det = -1;
      cx = cxA[s];
      cy = cyA[s]; 
      cz = czA[s];
       r =  rA[s];
      b = 2*(rx*(ptx-cx) + ry*(pty-cy)+ rz*(ptz-cz));
      c = (ptx-cx)*(ptx-cx) + (pty-cy)*(pty-cy) + (ptz-cz)*(ptz-cz) - r*r;
      det = b*b - 4*c;  //a = 1

      // if(det < 0) continue;
      posT = (-b + sqrt(det)) / 2;  //a = 1
      negT = (-b - sqrt(det)) / 2;
      if(det > 0 && (posT > 0 || negT > 0))
      {
         // rgb[0] = rgb[0]/2;
         // rgb[1] = rgb[1]/2;
         // rgb[2] = rgb[2]/2;
         gradient = 0;
         break;
      }
   }
   // if( onFloor ) printf("%d\n", gradient);
   if(gradient)
   {
      double dot = rx * nx + ry * ny + rz * nz;
      // if(onFloor) printf("%f\n", dot);
      if(dot > 0)
      {
         rgb[0] += dot*rgb[0];
         rgb[1] += dot*rgb[1];
         rgb[2] += dot*rgb[2];
      }
   }

}

int main(void)
{
   defineSpheres();
   int rgb[HIGHT][WIDTH][3] ; // red-green-blue for each pixel
   //
   int y , x ;
   //
   FILE* fout ;
   
   //
   for( y = 0 ; y < HIGHT ; y++ )
   {
      // printf("%d\n", y);
      
      for( x = 0 ; x < WIDTH ; x++)
      {
         getcolor(rgb[y][x], y, x);
      }
   }
   //
   //
   //
   fout = fopen( "raytrace.ppm" , "w" ) ;
   //
   fprintf( fout , "P3\n" ) ;
   fprintf( fout , "%d %d\n" , WIDTH , HIGHT ) ;
   fprintf( fout , "255\n" ) ;
   //
   for( y = 0 ; y < HIGHT ; y++ )
   {
      for( x = 0 ; x < WIDTH ; x++)
      {
         fprintf( fout , "%d %d %d\n" ,
          rgb[y][x][0] , rgb[y][x][1] , rgb[y][x][2] ) ;
      }
   }
   close( fout ) ;
   //
   return 0 ;
}
//
// end of file
//
