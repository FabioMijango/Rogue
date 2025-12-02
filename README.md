# Rogue
Inspired in the 1980's Rogue game, is a simple dungeon crawler game made with C++ and SFML library.

Explore randomly generated dungeons, fight monsters and upgrade your gear.


*Project just for learning purposes.*


## Quick start

### Command line

1. Clone this repository.
2. Navigate to the project directory.
   ```bash
     cd Rogue
   ```
3. Build project
   ```bash
      cmake -B build
      cmake --build build
   ```
### IDE
1. Clone this repository
2. Open the project folder in your favorite IDE that supports CMake (e.g., CLion, Visual Studio Code, etc.).
3. Run the CMake configuration to generate the build files.

### > Linux
Note that you may need to install development packages for SFML dependencies.

   ```
      sudo apt update
      sudo apt install \
          libxrandr-dev \
          libxcursor-dev \
          libxi-dev \
          libudev-dev \
          libfreetype-dev \
          libflac-dev \
          libvorbis-dev \
          libgl1-mesa-dev \
          libegl1-mesa-dev \
          libfreetype-dev
   ```

## Resources
- [Rogue assets v0.5](https://sethbb.itch.io/32rogues)