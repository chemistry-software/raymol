# SDF Molfile parser and molecule renderer with Raylib

## Compilation
```sh
zig cc -o test -lraylib -L /usr/local/lib -I /usr/local/include main.c -DDEBUG
or
gcc $(pkg-config --cflags raylib) -L/usr/local/lib main.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o ass -DDEBUG 
```
Or without DEBUG flag to omit debug logging

## Example files
SDF molfiles of ethanol and methyl vinyl ketone are available. 3D SVG images were generated from these using OpenBabel for comparison with the Raymol output.
Conversion command:
```sh
obabel methyl-vinyl-ketone.sdf -Omethyl-vinyl-ketone.svg -xS
```

## TODO
- Drag and drop molfiles [Raylib supporst this nicely](https://www.raylib.com/examples/core/loader.html?name=core_drop_files)
- Use meshes instead of DrawSphere/Cylinder for performance (GPU already getting toasty)
- Use periodic table data for colors and bond properties etc, made an `Element` struct for this


By [@bdnugget](https://t.me/bdnugget)
