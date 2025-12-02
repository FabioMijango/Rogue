#pragma once

#include <filesystem>
#include <string>

namespace bb {
    struct Resource {
        std::string name;
        std::filesystem::path path;
    };

    inline const Resource resources[] = {
        {"TILES", "./res/tiles.png"},
    };

}
