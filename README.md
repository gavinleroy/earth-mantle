# Earth Mantle

Download the earth mantle data into the `data/` directory. The tool currently defaults to using `spherical001.nc.`

Build and run the project
```shell
cd build

cmake -DVTK_DIR="my_path_to_VTK" ..

// Linux
make -j && ./earth-mantle

// MacOS
make -j && ./src/earth-mantle.app/Contents/MacOS/earth-mantle
```