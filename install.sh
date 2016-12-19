cd ./lib/yaml
mkdir build
cd ./build
cmake -DBUILD_SHARED_LIBS=ON ..
make

cd ../../../

./build.sh
