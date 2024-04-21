#pragma once

#include <utility>
#include <vector>
#include <string>

#ifdef SINAPS_SHARED
#ifdef _WIN32
#ifdef SINAPS_EXPORTS
#define SINAPS_API __declspec(dllexport)
#else
#define SINAPS_API __declspec(dllimport)
#endif
#else
#define SINAPS_API
#endif
#else
#define SINAPS_API
#endif

/// Memory manipulation namespace definition (platform specific)
namespace sinaps::mem {

    /**
     * Write data to a memory region
     * @param address The address of the memory region
     * @param data The data to write to the memory region
     * @param size The size of the data to write to the memory region
     * @return True if the data was written successfully, false otherwise
     */
    SINAPS_API bool write(void *address, const void *data, size_t size);

    /**
     * Read data from a memory region
     * @param address The address of the memory region
     * @param data The data to read from the memory region
     * @param size The size of the data to read from the memory region
     * @return True if the data was read successfully, false otherwise
     */
    SINAPS_API std::vector<uint8_t> read(void *address, size_t size);

    /**
     * Get the base address of a module
     * @param moduleName The name of the module
     * @return The base address of the module
     */
    SINAPS_API void *getModuleBase(const char *moduleNam = nullptr);

}