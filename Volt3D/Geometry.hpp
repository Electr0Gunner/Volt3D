#pragma once

#include "Common.hpp"

#include <vector>
#include <glm/glm.hpp>

namespace Volt
{
    struct Plane
    {
        Vector3 normal;
        float distance;

        Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3)
        {
            normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
            distance = glm::dot(normal, p1);
        }

        float DistanceTo(const Vector3&& point) const
        {
            return glm::dot(normal, point) - distance;
        }
    };


    struct Polygon
    {
        std::vector<Vector3> vertices;

        void Add(const Vector3& vertex)
        {
            vertices.push_back(vertex);
        }

        void Clear()
        {
            vertices.clear();
         }

        size_t Size() const
        {
            return vertices.size();
        }
    };

    Polygon CreateQuadOnPlace(const Plane& plane);
    Polygon ClipPolygonAgainstPlace(const Polygon& poly, const Plane& place);
    Vector3 IntersectEdgeWithPlace(const Vector3& p1, const Vector3& p2, const Plane& place);
    std::vector<Vector3> TriangulagePolygon(const Polygon& poly);
        
    
} // namespace Volt
