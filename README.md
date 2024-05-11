# SDF Molfile parser and molecule renderer with Raylib

## Compilation
```sh
cc -o test -lraylib -L /usr/local/lib -I /usr/local/include main.c```

## Example files
SDF molfiles of ethanol and methyl vinyl ketone are available. 3D SVG images were generated from these using OpenBabel for comparison with the Raymol output.
Conversion command:
```sh
obabel methyl-vinyl-ketone.sdf -Omethyl-vinyl-ketone.svg -xS
```

By @bdnugget
