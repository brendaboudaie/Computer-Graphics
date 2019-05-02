#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    Hit hit {this, 0, part};
    double t;

    vec3 d = ray.direction;
    vec3 e_c = ray.endpoint - center;
    double b = dot(d, e_c);
    double a = dot(d,d);
    double c = dot(e_c, (e_c)) - pow(radius,2);
    double discriminant = pow(b,2) - (a*c); // b^2 - 4ac

    if (discriminant == 0){
      t = (-1 * (b/a));
      if (t > small_t) {
        hit.dist = t;
      }
    }
    else if (discriminant > 0) {
      double t0 = ((-1 * b) - sqrt(pow(b,2) - (a*c)))/a; // substration option
      double t1 = ((-1 * b) + sqrt(pow(b,2) - (a*c)))/a;  //addition option

      if (t0 > small_t && t1 > small_t){
        hit.dist = std::min(t0,t1);
      }
    }
    else if (discriminant < 0){
      hit.dist = -1 ;
    }
    return hit;
    // return {0,0,0};
}

vec3 Sphere::Normal(const vec3& point, int part) const //
{
    vec3 normal;
    normal = (point - center).normalized();
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    vec3 v = {radius, radius, radius};
    vec3 min = center - v;
    vec3 max = center + v;
    box.lo = min;
    box.hi = max;
    // TODO; // calculate bounding box
    return box;
}
