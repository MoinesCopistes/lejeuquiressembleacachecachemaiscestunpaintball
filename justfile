run:
    make -C build
    ./build/paintball

setup:
    mkdir -p build/
    cd build && cmake ..
