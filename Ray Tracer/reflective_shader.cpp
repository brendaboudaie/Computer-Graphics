#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    // r = light - 2(light dot normal)normal
    //vec3 n = normal.normalized();
    vec3 color = vec3(0,0,0);
    color = (1 - reflectivity) * (shader->Shade_Surface(ray, intersection_point, normal, recursion_depth));
    vec3 R = (ray.direction - (2*(dot(normal,ray.direction))*normal)).normalized() ;
    Ray refRay (intersection_point, R);

    if (recursion_depth > 1 && recursion_depth <= world.recursion_depth_limit){
      color = color + reflectivity * world.Cast_Ray(refRay, recursion_depth - 1);
    }

    return color;

 // shade_surface:
 // color = ...
 // compute reflected ray
 // return color = (1-k) * color + k * cast_ray(reflected ray)

 //(1 - k) * base shader color + k * reflected color
}
