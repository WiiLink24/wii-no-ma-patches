# Wii no Ma Patches
Patches for the Wii no Ma channel to allow for localization.

I decided to write the patches in C++ for transparency as well as have a better visual representation of what is actually happening. Prior to this, all patches were ASM inserted into specific instructions.

## How it works
We can call on symboled functions from inside the DOL. After compiling, we can use `powerpc-eabi-objcopy` to create the binary blob which will be inserted at the end of the DOL. 

First we modify the DOL header to add a data region to store the binary blob. We then modify the function at `0x80006560` to copy the binary blob to `0x80001800` (Unused exception handler region). This is required as BSS memory is allocated in the region we originally inserted the binary blob into.