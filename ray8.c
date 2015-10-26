//
// Reflection
//  structs
//
#include <stdio.h>
#include <math.h>
#include <string.h>
//
#define WIDTH 640
#define HIGHT 480
//values
//Floor
#define fy 0.333333
//Sphere 0
#define sphereNum 3

#define recurDepth 1

typedef struct
{
   double x;
   double y;
   double z;
} vector;

typedef struct
{
   int r;
   int g;               
   int b;
} color;

typedef struct
{
   vector cen;
   double r;
   color rgb;
} sphere;


vector light = {.00000, 1.00000, -.50000};
sphere spheres[sphereNum];

//
void initV(vector* v, double xt, double yt, double zt)
{
   v -> x = xt;
   v -> y = yt;
   v -> z = zt;
}
void initC(color* c, int rt, int gt, int bt)
{
   c -> r = rt;
   c -> g = gt;
   c -> b = bt;
}
void initS(sphere* s, vector vt, double rt, color ct)
{
   s -> cen = vt;
   s -> r = rt;
   s -> rgb = ct;
}

double dot(vector* a, vector* b)
{
   double d = (a -> x * b -> x) + (a -> y * b -> y) + (a -> z * b -> z);
   return d;
}

double magnitude(vector v)
{
   return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

void normalize(vector* v)
{
   double mag = magnitude(*v);
   //normalize
   v -> x = v -> x / mag;
   v -> y = v -> y / mag;
   v -> z = v -> z / mag;
}

vector reflect(vector r, vector n)
{
   normalize(&r);
   vector w;
   double dotp = dot(&r, &n);
   w.x = -1*r.x + 2*(r.x - (dotp) * n.x);
   w.y = -1*r.y + 2*(r.y - (dotp) * n.y);
   w.z = -1*r.z + 2*(r.z - (dotp) * n.z);

   return w;
}

void defineSpheres()
{

   sphere s;
   vector v;
   color c;
   double r = 0;

   initV(&v, 0.500000, 0.500000, 0.166667);
   r = 0.166667;
   initC(&c, 0, 0, 255);
   initS(&s, v, r, c);
   // memcpy(spheres + 0, &s, sizeof(sphere));
   spheres[0] = s;


   initV(&v, 0.833333, 0.500000, 0.500000);
   r = 0.166667;
   initC(&c, 0, 255, 0);
   initS(&s, v, r, c);
   // memcpy(spheres + 1, &s, sizeof(sphere));
   spheres[1] = s;


   initV(&v, 0.333333, 0.666667, 0.666667);
   r = 0.333333;
   initC(&c, 255, 0, 0);
   initS(&s, v, r, c);
   // memcpy(spheres + 2, &s, sizeof(sphere));
   spheres[2] = s;
}

void getcolor(color* rgb, vector eye, vector ray, int depth)
{
   // double py,px, pz;  //screen coor
   // vector ray; //rays
   // double mag;
   double b, c;
   double cx,cy,cz,r;   //sphere stuff
   int s;
   double det; //determinant  
   double posT, negT, minT;
   vector normal;
   vector point;
   int onSphere = 0;
   int onFloor  = 0;

   onSphere = 0;


   // mag = sqrt(ray.x*ray.x + ray.y*ray.y + ray.z*ray.z);
   // //normalize
   // ray.x = ray.x / mag;
   // ray.y = ray.y / mag;
   // ray.z = ray.z / mag;
   // normalize(&ray);

   
   minT = 999999;
   // int temp;
   // for(temp = 0; temp < 3; temp++) rgb[temp] = 0;
   rgb -> r = 0;
   rgb -> g = 0;
   rgb -> b = 0;

   //floor
   double T = (fy - eye.y) / ray.y;
   if(T >= 0)
   {
      minT = T;
      rgb -> r = 122;
      rgb -> g = 122;
      rgb -> b = 0;

      normal.x = 0;
      normal.y = 1;
      normal.z = 0;

      onFloor = 1;
   }

   //spheres
   for(s = 0; s < sphereNum; s++)
   {
      det = -1;
      cx = spheres[s].cen.x;
      cy = spheres[s].cen.y;
      cz = spheres[s].cen.z;
      r = spheres[s].r;

      b = 2*(ray.x*(eye.x-cx) + ray.y*(eye.y-cy)+ ray.z*(eye.z-cz));
      c = (eye.x-cx)*(eye.x-cx) + (eye.y-cy)*(eye.y-cy) + (eye.z-cz)*(eye.z-cz) - r*r;
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
            rgb -> r = spheres[s].rgb.r/2.0;   //half color is ambient
            rgb -> g = spheres[s].rgb.g/2.0;
            rgb -> b = spheres[s].rgb.b/2.0;

            point.x = ray.x*minT+eye.x;
            point.y = ray.y*minT+eye.y;
            point.z = ray.z*minT+eye.z;

            normal.x = (point.x - cx) / r;
            normal.y = (point.y - cy) / r;
            normal.z = (point.z - cz) / r;
         }
         if(negT > 0 && negT < minT)
         {
            minT = negT;
            onSphere = 1;
            onFloor  = 0;
            rgb -> r = spheres[s].rgb.r/2.0;
            rgb -> g = spheres[s].rgb.g/2.0;
            rgb -> b = spheres[s].rgb.b/2.0;

            point.x = ray.x*minT+eye.x;
            point.y = ray.y*minT+eye.y;
            point.z = ray.z*minT+eye.z;

            normal.x = (point.x - cx) / r;
            normal.y = (point.y - cy) / r;
            normal.z = (point.z - cz) / r;
         }
      }
   }

   //shadows

   //fixing self intersection

   point.x = ray.x*minT+eye.x;
   point.y = ray.y*minT+eye.y;
   point.z = ray.z*minT+eye.z;

   point.x += normal.x * .01;
   point.y += normal.y * .01;
   point.z += normal.z * .01;
   
   if(onFloor)
   {
      if(((int)(point.x / .1 - (point.x < 0)) + (int)(point.z / .1 - (point.z < 0))) % 2 == 0)
      {
         rgb -> r = 0;
         rgb -> g = 122;
         rgb -> b = 122;
      }
   }

   //ray direction

   vector rayLight;

   rayLight.x = light.x - point.x;
   rayLight.y = light.y - point.y;
   rayLight.z = light.z - point.z;
   // mag = sqrt(ray.x*ray.x + ray.y*ray.y + ray.z*ray.z);
   // //normalize
   // ray.x = ray.x / mag;
   // ray.y = ray.y / mag;
   // ray.z = ray.z / mag;
   normalize(&rayLight);

   int gradient = 1;   //1 = no shadow; 0 = gradient
   for(s = 0; s < sphereNum; s++)
   {
      det = -1;
      cx = spheres[s].cen.x;
      cy = spheres[s].cen.y;
      cz = spheres[s].cen.z;
       r = spheres[s].r;
      b = 2*(rayLight.x*(point.x-cx) + rayLight.y*(point.y-cy)+ rayLight.z*(point.z-cz));
      c = (point.x-cx)*(point.x-cx) + (point.y-cy)*(point.y-cy) + (point.z-cz)*(point.z-cz) - r*r;
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
      double dot = rayLight.x * normal.x + rayLight.y * normal.y + rayLight.z * normal.z;
      // if(onFloor) printf("%f\n", dot);
      if(dot > 0)
      {
         rgb -> r += dot*rgb -> r;
         rgb -> g += dot*rgb -> g;
         rgb -> b += dot*rgb -> b;
      }
   }

   //reflection
   if((depth < recurDepth) && (onSphere || onFloor))
   {
      
      color newColor;
      normalize(&ray);
      normalize(&normal);
      vector direction = reflect(ray, normal);
      // printf("%f %f %f %f %f %f\n", direction.x, ray.x, direction.y, ray.y, direction.z, ray.z);
      // printf("hi\n");
      getcolor(&newColor, point, direction, depth+1);
      // printf("bye\n");

      if(newColor.r != 255 || newColor.g != 255 || newColor.b != 255)
      {
         rgb -> r = rgb -> r * .65 + newColor.r * .35;
         rgb -> g = rgb -> g * .65 + newColor.g * .35;
         rgb -> b = rgb -> b * .65 + newColor.b * .35;
      }
   }

}

int main(void)
{
   defineSpheres();
   color rgb[HIGHT][WIDTH] ; // red-green-blue for each pixel
   vector eye = {.50000, .50000, -1.0000};

   
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
         vector pt = {x*1.0 / (WIDTH), 1 - (y*1.0 / (HIGHT)), 0};
         vector ray;

         //ray direction
         ray.x = pt.x - eye.x;
         ray.y = pt.y - eye.y;
         ray.z = pt.z - eye.z;
         normalize(&ray);
         getcolor(&rgb[y][x], eye, ray, 0);
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
          rgb[y][x].r , rgb[y][x].g , rgb[y][x].b ) ;
      }
   }
   close( fout ) ;
   //
   return 0 ;
}
//
// end of file
//
