#pragma once

#include <cstdint>

namespace Volt
{
    struct MapHeader
    {
        char magic[5]; //should be {"V" ,"M" ,"A" ,"P" ,"\0"}
        uint32_t version; //version for the map format, used for debug
        uint32_t brush_offset; //offset for where the brushes are found
        uint32_t actor_offset; //offset for where actors are found
    };
    
} // namespace Volt
