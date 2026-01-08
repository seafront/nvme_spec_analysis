#ifndef NVME_GET_FEATURES_H
#define NVME_GET_FEATURES_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Command Dword Structures (Figure 193, 194)
//=============================================================================

/**
 * Select Values for Get Features Command
 * Figure 193
 */
enum class GetFeaturesSelect : uint8_t {
    Current               = 0b000, // Current
    Default               = 0b001, // Default
    Saved                 = 0b010, // Saved
    SupportedCapabilities = 0b011, // Supported Capabilities
    // 0b100 to 0b111: Reserved
};

/**
 * Get Features - Command Dword 10
 * Figure 193
 */
union GetFeaturesCDW10 {
    struct {
        uint32_t FID : 8;       // Bits 07:00 - Feature Identifier
        uint32_t SEL : 3;       // Bits 10:08 - Select
        uint32_t Reserved : 21; // Bits 31:11 - Reserved
    } bits;
    uint32_t raw;
};

/**
 * Get Features - Command Dword 14
 * Figure 194
 */
union GetFeaturesCDW14 {
    struct {
        uint32_t UIDX : 7;      // Bits 06:00 - UUID Index
        uint32_t Reserved : 25; // Bits 31:07 - Reserved
    } bits;
    uint32_t raw;
};

//=============================================================================
// Completion Queue Entry Structures (Figure 196)
//=============================================================================

/**
 * Get Features - Completion Queue Entry Dword 0 (when Select = 011b)
 * Figure 196
 */
union GetFeaturesCQEDW0Capabilities {
    struct {
        uint32_t SVBL : 1;      // Bit 0 - Saveable
        uint32_t NSSPEC : 1;    // Bit 1 - NS Specific
        uint32_t CHANG : 1;     // Bit 2 - Changeable
        uint32_t Reserved : 29; // Bits 31:3 - Reserved
    } bits;
    uint32_t raw;
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(GetFeaturesCDW10) == sizeof(uint32_t), "CDW10 size mismatch");
static_assert(sizeof(GetFeaturesCDW14) == sizeof(uint32_t), "CDW14 size mismatch");
static_assert(sizeof(GetFeaturesCQEDW0Capabilities) == sizeof(uint32_t), "CQEDW0 size mismatch");

} // namespace nvme

#endif // NVME_GET_FEATURES_H
