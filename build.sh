cmake -DCMAKE_BUILD_TYPE=Debug -H. -Bbuild -DCMAKE_CXX_COMPILER=g++-4.9 -DCMAKE_C_COMPILER=gcc-4.9

cd ./build
make
