rm -rf build
mkdir build
pushd build
conan install ..
cmake ..
cmake --build .
popd
echo build/bin/RxCppRLP > run.sh
chmod 755 run.sh

