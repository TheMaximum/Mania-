# Go to libraries directory
cd ./lib/

# Download version 0.5.3 of the YAML library
git clone https://github.com/jbeder/yaml-cpp.git yaml/
cd yaml/
git reset --hard b57efe94e7d445713c29f863adb8c23438eaa217

# Build the YAML library
mkdir build
cd ./build
cmake -DBUILD_SHARED_LIBS=ON ..
make yaml-cpp

# Return to root directory
cd ../../../

# Build project as normal
./build.sh
