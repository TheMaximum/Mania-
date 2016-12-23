# Install Boost and xerces
sudo apt-get -y install libboost-dev

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

# Return to lib directory
cd ../../

# Download version 1.8.1 of the PugiXML library
git clone https://github.com/zeux/pugixml.git pugixml/
cd pugixml/
git reset --hard d2deb42

# Build the PugiXML library
cmake -DCMAKE_BUILD_TYPE=Debug .
make

# Return to root directory
cd ../../

# Build project as normal
./build.sh
