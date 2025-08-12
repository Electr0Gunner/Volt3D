#pragma once

#include <algorithm>

namespace Smith
{
    template <class _T> class TypeRect
    {
        public:
            _T x;
            _T y;
            _T width;
            _T height;
        public:
            TypeRect(_T x, _T y, _T width, _T height): x(x), y(y), width(width), height(height) {}
            TypeRect(const TypeRect<_T>& rect): x(rect.x), y(rect.y), width(rect.width), height(rect.height) {}
            TypeRect() : x(0), y(0), width(0), height(0) {}

            bool Intersects(const TypeRect<_T>& rect) const
            {
                return !((rect.x + rect.width <= x) || (rect.y + rect.height <= y) || (rect.x >= x + width) || (rect.y >= y + height));
            }

            TypeRect<_T> Intersection(const TypeRect<_T>& rect) const
            {
                _T x1 = std::max(x, rect.x);
                _T y1 = std::max(y, rect.y);
                _T x2 = std::max(x + width, rect.x + rect.width);
                _T y2 = std::max(y + height, rect.y + rect.height);
                if (x2 - x1 < 0 || y2 - y1 < 0)
                    return TypeRect<_T>(0, 0, 0, 0);
                else
                    return TypeRect<_T>(x1, y1, x2 - x1, y2 - y1);
            }

            TypeRect<_T> Union(const TypeRect<_T>& rect)
            {
                _T x1 = std::min(x, rect.x);
                _T x2 = std::max(x + width, rect.x + rect.width);
                _T y1 = std::min(y, rect.y);
                _T y2 = std::max(y + height, rect.y + rect.height);
                return TypeRect<_T>(x1, y1, x2 - x1, y2 - y1);
            }

            bool Contains(_T x_pos, _T y_pos) const
            {
                return (x_pos >= x && x_pos < x + width && y_pos >= y && y_pos < y + height);
            }

            bool operator==(const TypeRect<_T>& rect) const
            {
                return (x == rect.x) && (y == rect.y) && (width == rect.width) && (height == rect.height);
            }

            void Offset(_T x_offset, _T y_offset)
            {
                x += x_offset;
                y += y_offset;
            }

    };

typedef TypeRect<int> Rect;
typedef TypeRect<float> FRect;

}