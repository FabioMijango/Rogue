#pragma once

#include <string>

namespace bb {
    struct Resource {
        std::string name;
        std::string imagePath;
        std::string metaPath;
    };

    inline const Resource resources[] = {
        {"tiles", "./res/tiles.png", "./res/tiles.txt"},
    };

}
