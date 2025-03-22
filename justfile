run:
    make -C build -j16
    ./build/paintball

setup:
    mkdir -p build/
    cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..
