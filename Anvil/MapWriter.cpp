#include "MapWriter.hpp"
#include "Editor.hpp"
#include <world/MapFile.hpp>

#include <fstream>
#include <string>

using namespace Anvil;

void MapWriter::WriteMap()
{
    Smith::MapHeader header{};
    memcpy(header.magic, "SMITH", 5);
    header.version = 1;
    header.map_revision = 0;

    std::ofstream out("test.smap", std::ios::binary); //smap is a temp name (smith_map)
    out.write((char*)&header, sizeof(header));
    out.close();
}