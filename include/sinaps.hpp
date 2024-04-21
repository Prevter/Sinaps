#pragma once

#include <utility>
#include <vector>
#include <string>

#include "sinaps/platform.hpp"
#include "sinaps/extras.hpp"

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

        // metadata for the patch
        std::string module;
        uintptr_t offset{};

        patch_t(void *address, std::vector<uint8_t> original, std::vector<uint8_t> patch)
                : address(address), size(original.size()), original(std::move(original)), patch(std::move(patch)),
                  applied(false) {}

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
        [[nodiscard]] bool write(bool apply) const;
    };

    struct SINAPS_API module_t {
        std::string name;
        uintptr_t base;
        uintptr_t size;

        module_t(std::string name, uintptr_t base, uintptr_t size)
                : name(std::move(name)), base(base), size(size) {}

        module_t(const module_t &other) = default;

        module_t &operator=(const module_t &other) = default;

        module_t(module_t &&other) noexcept = default;

        module_t &operator=(module_t &&other) noexcept = default;

        ~module_t() = default;

        bool operator==(const module_t &other) const {
            return name == other.name && base == other.base && size == other.size;
        }

        bool operator!=(const module_t &other) const {
            return !(*this == other);
        }

    };

    /**
     * Set the default module to use for the find function
     * @param module The default module to use
     */
    SINAPS_API void setDefaultModule(module_t module);

    /**
     * Scan a memory region for a pattern
     * @param pattern The pattern to find
     * @param module The module to scan
     * @return The address of the pattern in the memory region
     */
    SINAPS_API uintptr_t find(const std::string &pattern, const module_t& module);

    /**
     * Scan a memory region for a pattern
     * @param pattern The pattern to find
     * @return The address of the pattern in the memory region
     */
    SINAPS_API uintptr_t find(const std::string &pattern);

    SINAPS_API patch_t match(uintptr_t address, const std::string &mask);

    SINAPS_API Result<patch_t> match(const std::string &pattern, const std::string &mask, const module_t& module);

    SINAPS_API Result<patch_t> match(const std::string &pattern, const std::string &mask);

    /// === Multiple mode === ///

    SINAPS_API std::vector<patch_t> matchAll(const std::string &pattern, const std::string &mask, const module_t& module);

    SINAPS_API std::vector<patch_t> matchAll(const std::string &pattern, const std::string &mask);

    SINAPS_API std::vector<uintptr_t> findAll(const std::string &pattern, const std::string &mask, const module_t& module);

    SINAPS_API std::vector<uintptr_t> findAll(const std::string &pattern, const std::string &mask);

}
