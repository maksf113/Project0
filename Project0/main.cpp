#include <iostream>
#include <array>
#include <string>
#include "Data Structures\List.h"
#include "Data Structures\LinkedSearchTree.h"
#include "Data Structures\SearchTree.h"
#include "Data Structures\RandomizedSearchTree.h"
#include "Graphics\Renderer.h"
#include "Graphics\VAO.h"
#include "Graphics\VBO.h"
#include "Graphics\EBO.h"
#include "Graphics\ShaderProgram.h"
#include "Geometry\Polygon.h"
#include "Domain.h"
#include "Delaunay.h"
#include "Random.h"

int main()
{
    Domain domain;
    const std::vector<Point>& points = domain.points();
    const std::vector<Point>& innerBoundary = domain.innerBoundary();
    delaunay::Delaunay d(points, innerBoundary);
    std::vector<float> verts;
    for (const auto& p : points)
    {
        verts.push_back(float(p.x()));
        verts.push_back(float(p.y()));
    }
    
    List<Polygon*>* triangulation = d.triangles();

    std::vector<float> triVerts;
    Polygon* triangle = triangulation->first();
    std::cout << triangulation->length() << std::endl;
    for (; !triangulation->isHead(); triangle = triangulation->next())
    {
        for (int i = 0; i < 3; i++)
        {
            Vertex* v = triangle->advance();
            Point p = v->point();
            triVerts.push_back(float(p.x()));
            triVerts.push_back(float(p.y()));
        }
    }
    
    Renderer renderer;
    renderer.init();
    renderer.setMesh(verts, triVerts);
    renderer.render();
    renderer.clear();
}
