scons
rm -rf build
mkdir build
cd build
cmake -D BUILD_SHARED=ON -D BUILD_TEST=OFF ..
cmake --build .
sudo make install
