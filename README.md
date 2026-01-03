# cvtwebp

Simple tool I made to convert images to WebP format using the command line. For my own convenience.

## Dependencies

* Imagemagick++
* C++17 capable compiler

## Build

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```

## Usage

See `cvtwebp --help` for usage information.

```bash
./cvtwebp <input_image> [output_image.webp]
```
