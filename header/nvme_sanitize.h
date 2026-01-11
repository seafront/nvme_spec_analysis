#ifndef NVME_SANITIZE_H
#define NVME_SANITIZE_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Command Dword Structures (Figure 373, 374)
//=============================================================================

/**
 * Sanitize Action Values
 * Figure 373
 */
enum class SanitizeAction : uint8_t {
    // 0b000: Reserved
    ExitFailureMode         = 0b001, // Exit Failure Mode
    BlockErase              = 0b010, // Start a Block Erase sanitize operation
    Overwrite               = 0b011, // Start an Overwrite sanitize operation
    CryptoErase             = 0b100, // Start a Crypto Erase sanitize operation
    ExitMediaVerification   = 0b101, // Exit Media Verification State
    // 0b110 to 0b111: Reserved
};

/**
 * Sanitize - Command Dword 10
 * Figure 373
 */
union SanitizeCDW10 {
    struct {
        uint32_t SANACT : 3;    // Bits 02:00 - Sanitize Action
        uint32_t AUSE : 1;      // Bit 03 - Allow Unrestricted Sanitize Exit
        uint32_t OWPASS : 4;    // Bits 07:04 - Overwrite Pass Count
        uint32_t OIPBP : 1;     // Bit 08 - Overwrite Invert Pattern Between Passes
        uint32_t NDAS : 1;      // Bit 09 - No-Deallocate After Sanitize
        uint32_t EMVS : 1;      // Bit 10 - Enter Media Verification State
        uint32_t Reserved : 21; // Bits 31:11 - Reserved
    } bits;
    uint32_t raw;
};

/**
 * Sanitize - Command Dword 11
 * Figure 374
 */
union SanitizeCDW11 {
    struct {
        uint32_t OVRPAT : 32;   // Bits 31:00 - Overwrite Pattern
    } bits;
    uint32_t raw;
};


typedef struct _SanitizeCmdParameter{
    uint32_t nsid;

    SanitizeCDW10 cdw10;
    SanitizeCDW11 cdw11;
} SanitizeCmdParameter;

//=============================================================================
// Command Specific Status Values (Figure 375)
//=============================================================5================

/**
 * Sanitize - Command Specific Status Values
 * Figure 375 (SCT field set to 1h)
 */
enum class SanitizeStatusCode : uint8_t {
    FirmwareActivationRequiresConventionalReset = 0x0B, // Firmware Activation Requires Conventional Reset
    FirmwareActivationRequiresNVMSubsystemReset = 0x10, // Firmware Activation Requires NVM Subsystem Reset
    FirmwareActivationRequiresControllerLevelReset = 0x11, // Firmware Activation Requires Controller Level Reset
    SanitizeProhibitedWhilePMREnabled = 0x23, // Sanitize Prohibited While Persistent Memory Region is Enabled
    ControllerSuspended = 0x39, // Controller Suspended
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(SanitizeCDW10) == sizeof(uint32_t), "CDW10 size mismatch");
static_assert(sizeof(SanitizeCDW11) == sizeof(uint32_t), "CDW11 size mismatch");

} // namespace nvme

#endif // NVME_SANITIZE_H
