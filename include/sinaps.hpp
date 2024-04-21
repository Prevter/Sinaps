#pragma once

#include <utility>
#include <vector>
#include <string>

#include "sinaps/platform.hpp"

/// Sinaps namespace definition
namespace sinaps {

    /**
     * A structure, containing the information about a patched memory region
     * The structure contains the address of the patched memory region, the size of the patched memory region,
     * the original data of the patched memory region, the patch data of the patched memory region and a flag that
     * indicates whether the patch was applied.
     * @param address The address of the patched memory region
     * @param size The size of the patched memory region
     * @param original The original data of the patched memory region
     * @param patch The patch data of the patched memory region
     * @param applied A flag that indicates whether the patch was applied
     */
    struct SINAPS_API patch_t {
        void *address{};
        size_t size{};
        std::vector<uint8_t> original;
        std::vector<uint8_t> patch;
        bool applied{};

        patch_t(void *address, size_t size, std::vector<uint8_t> original, std::vector<uint8_t> patch, bool applied)
                : address(address), size(size), original(std::move(original)), patch(std::move(patch)),
                  applied(applied) {}

        patch_t() = default;

        patch_t(const patch_t &other) = default;

        patch_t &operator=(const patch_t &other) = default;

        patch_t(patch_t &&other) noexcept = default;

        patch_t &operator=(patch_t &&other) noexcept = default;

        ~patch_t() = default;

        /**
         * Apply the patch, by writing the patch data to the memory region
         * @return true if the patch was successfully applied, false otherwise
         */
        bool apply();

        /**
         * Revert the patch, by writing the original data to the memory region
         * @return true if the patch was successfully reverted, false otherwise
         */
        bool revert();

        /**
         * Write the patch data to the memory region
         * @param apply A flag that indicates whether the patch should be applied
         * @return true if the patch data was successfully written to the memory region, false otherwise
         */
        bool write(bool apply);
    };

    /**
     * Scan a memory region for a pattern
     * @param pattern The pattern to find
     * @param start The start address of the memory region
     * @param end The end address of the memory region
     * @return The address of the pattern in the memory region
     */
    SINAPS_API uintptr_t find(const std::string &pattern, uintptr_t start, uintptr_t end);

    /**
     * Scan a memory region for a pattern
     * @param pattern The pattern to find
     * @param module The name of the module to scan
     * @return The address of the pattern in the memory region
     */
    SINAPS_API uintptr_t find(const std::string &pattern, const std::string &module = "");

    SINAPS_API patch_t match(const std::string &pattern, const std::string &mask, uintptr_t start, uintptr_t end);

    SINAPS_API patch_t match(const std::string &pattern, const std::string &mask, const std::string &module = "");

    SINAPS_API patch_t match(uintptr_t address, const std::string &mask);

}
