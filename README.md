## SINAPS
(Sinaps Is Not A Pattern Scanner)

## Description
SINAPS is a C/C++ library that provides a set of functions for 
signature scanning, pattern matching and memory patching in a target process. 

It is designed to be used in game hacking, reverse engineering and other memory
manipulation scenarios.

## Features
- Signature scanning with a regex-like set of features
- Pattern matching with byte operations (wildcards, addition, shifts, subpatterns, etc.)
- Cross-platform support (Windows, Linux, MacOS, Android)
- Memory patching tools for each platform

## Usage
SINAPS is a static library that can be linked to your project using CMake.
Add the repository as a submodule to your project and include the library in your CMakeLists.txt file.

```cmake
add_subdirectory(libs/sinaps)
target_link_libraries(your_project PRIVATE sinaps)
```

## Examples

Basic usage of the library is shown below. The library provides a set of functions
for finding patterns in memory and patching them with new values.

```cpp
#include <sinaps.hpp>

int main() {
    // Find the address of a pattern in the target process (current process by default)
    uintptr_t address = sinaps::find("00 60 $6A 48 00");
    // Note: $6A tells the scanner to return the address of the 6A byte
    
    if (address) { // If the address is not a nullptr, the pattern was found
        // We can now patch the address with a new set of bytes
        sinaps::patch_t patch = sinaps::match(address, "EA 4B");
        patch.apply();
        
        // The patch_t object can be used to revert the patch later, because
        // it stores the original bytes that were replaced
        // patch.revert();
    } else {
        printf("Failed to find address\n");
    }
}
```

The library also provides a shorthand function for finding and patching patterns in one go.

```cpp
#include <sinaps.hpp>

int main() {
    sinaps::patch_t patch;
    
    // sinaps::match returns a boolean value indicating if the pattern was found
    // and sets the patch object to the address and new bytes
    if (sinaps::match(patch, "00 60 $6A 48 00", "EA 4B")) {
        patch.apply();
    } else {
        printf("Failed to find address\n");
    }
}
```