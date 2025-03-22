run:
    make -C build -j16
    ./build/paintball

run_client:
    make -C build
    ./build/paintball client

setup:
    mkdir -p build/
    cd build && cmake -DCMAKE_BUILD_TYPE=Debug ..
