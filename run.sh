
cmake --build build
if [ $? -eq 0 ] ; then
    cd build/bin
    ./ReRogue
    cd ../..
else
    echo "Build failed."
fi
