
if [[ $1 = "release" ]]; then
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    echo "Building in Release mode"
else
    cmake -B build -DCMAKE_BUILD_TYPE=Debug
    echo "Building in Debug mode"
fi
cmake --build build
