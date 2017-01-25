# Install Git, Boost and MySQL
sudo apt-get -y install git libboost-dev libboost-iostreams-dev libboost-regex-dev libmysqlcppconn-dev libcurl4-openssl-dev

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

# Create and go to JSON directory
mkdir json
cd ./json

# Download version 2.0.10 of the JSON library (remove older/existing version)
rm ./json.hpp
wget https://github.com/nlohmann/json/releases/download/v2.0.10/json.hpp

# Return to lib directory
cd ../

# Download version 1.8.1 of the PugiXML library
git clone https://github.com/zeux/pugixml.git pugixml/
cd pugixml/
git reset --hard d2deb42

# Build the PugiXML library
cmake -DCMAKE_BUILD_TYPE=Debug .
make

# Return to root directory
cd ../../
