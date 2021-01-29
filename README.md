# blacklibrarydb
Black Library database library

# Dependencies

On Ubuntu 18.04 libsqlite3-dev is required

# How to build

`cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=install . -Bbuild`

`make -Cbuild -j$(nproc) install`
