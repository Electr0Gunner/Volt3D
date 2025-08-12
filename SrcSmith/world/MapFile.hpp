#pragma once

#include <cstdint>

namespace Smith
{

    enum LumpType
    {
        TYPE_INVALID = 0,
        TYPE_BRUSH,
        TYPE_ACTOR,
        TYPE_COUNT,
    };

    #define LUMPS_COUNT (int)LumpType::TYPE_COUNT
    
    struct DataLump
    {
        int data_type;
        int data_offset;
        int data_length;
    };

    struct MapHeader
    {
        char magic[5]; //should be {"SMITH"}
        int version; //version for the map format, used for debug and parsing
        int map_revision;
        int lump_count;
        DataLump lumps[LUMPS_COUNT]; //lumps defining anything in the map
    };
    
} // namespace Smith
