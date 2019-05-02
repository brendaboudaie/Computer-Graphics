#include "mesh.h"
#include <fstream>
#include <string>
#include <limits>
#include "plane.h"

// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file)
{
    std::ifstream fin(file);
    if(!fin)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e;
    vec3 v;
    box.Make_Empty();
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
            box.Include_Point(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }
    }
    number_parts=triangles.size();
}

Hit Mesh::Intersection(const Ray& ray, int part) const
{
    // TODO;
    Hit hit;
    hit.object = this;
    hit.part = part;
    hit.dist = -1;

    double dist = 0.0;
    bool usedYet = true; // checking only one triangle

    for(size_t i = 0; i < triangles.size(); i++){
        if(Intersect_Triangle(ray, i, dist)){
            if(usedYet){
              usedYet = false;
              hit.object = this;
              hit.dist = dist;
              hit.part = i;
            }
      }
  }
    return hit;
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const vec3& point, int part) const
{
  assert(part>=0);
  ivec3 tri = triangles[part];

  vec3 A = vertices[tri[0]];
  vec3 B = vertices[tri[1]];
  vec3 C = vertices[tri[2]];

  vec3 AB = B - A; // ray AB
  vec3 AC = C - A; // ray AC

  vec3 n = cross(AB, AC).normalized();

  return n;
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
bool Mesh::Intersect_Triangle(const Ray& ray, int tri, double& dist) const
{
    // Triangle vertices
    vec3 triangleA = vertices[triangles[tri][0]]; // A
    vec3 triangleB = vertices[triangles[tri][1]]; // B
    vec3 triangleC = vertices[triangles[tri][2]]; // C
    vec3 intersect;

    Hit hit = Plane(triangleA, Normal(triangleA, tri)).Intersection(ray, tri);

    if (!hit.object) { // no intersection
        return false;
    }

    intersect = ray.Point(dist);

    vec3 u = ray.direction;
    vec3 AB = triangleB - triangleA;
    vec3 AC = triangleC - triangleA;
    vec3 v = intersect - triangleA;

    double d = dot(cross(u, AB), AC); // denominator

    if (!d) {
        return false;
    }

    // calc barycentric weights
    double gamma = dot(cross(u, AB), v) / d;
    double beta = dot(cross(AC, u), v) / d;
    double alpha = 1 - (gamma + beta);

    // compare against weight_tol to prevent ray going between triangles
    if (gamma > -weight_tol && beta > -weight_tol && alpha > -weight_tol) {
        dist = hit.dist;
        return true;
    }

    return false;
}

// Compute the bounding box.  Return the bounding box of only the triangle whose
// index is part.
Box Mesh::Bounding_Box(int part) const
{
    Box b;
    TODO;
    return b;
  }
