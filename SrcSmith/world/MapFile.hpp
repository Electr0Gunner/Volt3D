#pragma once

#include <cstdint>

namespace Smith
{
    struct MapHeader
    {
        char magic[6]; //should be {"S" ,"M" ,"I" ,"T" ,"W" ,"\0"}
        uint32_t version; //version for the map format, used for debug and parsing
        uint32_t brush_offset; //offset for where the brushes are found
        uint32_t actor_offset; //offset for where actors are found
    };
    
} // namespace Smith
