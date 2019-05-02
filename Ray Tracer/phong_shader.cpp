#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
  vec3 color = vec3(0,0,0);
  vec3 ambientColor = vec3(0,0,0);
  vec3 diffuseColor = vec3(0,0,0);
  vec3 scpecularColor = vec3(0,0,0);
     ambientColor =  world.ambient_color * color_ambient * world.ambient_intensity; // Ia = RaLa
      for(int i = 0; i < world.lights.size(); ++i){ // lighting of each light
         vec3 L =   (world.lights.at(i)->position - intersection_point);
         vec3 n = normal.normalized();
         vec3 R = (2*dot(L.normalized(),n) * n - L.normalized()).normalized();
         Ray hitObject(intersection_point, L.normalized());
         Hit theHit = world.Closest_Intersection(hitObject);
         double lightDist = L.magnitude();

         vec3 V = (ray.endpoint - intersection_point).normalized();
          if( (theHit.dist > 0 && lightDist > theHit.dist)  && world.enable_shadows == 1){

           }

           else{
               diffuseColor += color_diffuse* world.lights.at(i)->Emitted_Light(L)
                 * fmax(dot( n, L.normalized() ),0 );
               scpecularColor += color_specular * world.lights.at(i)->Emitted_Light(L)
               * pow(fmax(dot(R,V),0) ,specular_power); // specular
             }

      }

      color = ambientColor + diffuseColor + scpecularColor ;

    return color;
  }
