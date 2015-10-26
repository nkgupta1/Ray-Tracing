//
// Basic 2D spheres
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

int main(void)
{
   defineSpheres();
   int rgb[HIGHT][WIDTH][3] ; // red-green-blue for each pixel
   //
   int y , x ;
   //
   FILE* fout ;
   double py,px;  //screen coor
   double rx,ry,rz;  //rays
   double mag;
   double b, c;
   double cx,cy,cz,r;   //sphere stuff
   int s;
   double det; //determinant  
   double posT, negT, minT;
   //
   for( y = 0 ; y < HIGHT ; y++ )
   {
      // printf("%d\n", y);
      py = 1 - (y*1.0 / (HIGHT));
      for( x = 0 ; x < WIDTH ; x++)
      {
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
         for(temp = 0; temp < 3; temp++) rgb[y][x][temp] = 0;
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
                  rgb[y][x][0] = red[s];
                  rgb[y][x][1] = green[s];
                  rgb[y][x][2] = blue[s];
               }
               if(negT > 0 && negT < minT)
               {
                  minT = negT;
                  rgb[y][x][0] = red[s];
                  rgb[y][x][1] = green[s];
                  rgb[y][x][2] = blue[s];
               }

            }

         }

         
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
