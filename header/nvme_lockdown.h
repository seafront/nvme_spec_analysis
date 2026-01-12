#ifndef NVME_LOCKDOWN_H
#define NVME_LOCKDOWN_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Lockdown Command Enums (Figure 338)
//=============================================================================

/**
 * Lockdown Scope Values
 * Figure 338 - SCP field
 */
enum class LockdownScope : uint8_t {
    AdminCommandOpcode          = 0x0,  // Admin command opcode
    // 0x1 Reserved
    SetFeaturesFeatureId        = 0x2,  // A Set Features Feature Identifier
    ManagementInterfaceOpcode   = 0x3,  // Management Interface Command Set opcode
    PCIeCommandSetOpcode        = 0x4,  // PCIe Command Set opcode
    // 0x5-0xF Reserved
};

/**
 * Lockdown Interface Values
 * Figure 338 - IFC field
 */
enum class LockdownInterface : uint8_t {
    AdminSubmissionQueue        = 0b00, // Admin Submission Queue
    AdminSQAndManagementEndpoint = 0b01, // Admin Submission Queue and out-of-band on a Management Endpoint
    ManagementEndpoint          = 0b10, // Out-of-band on a Management Endpoint
    // 0b11 Reserved
};

//=============================================================================
// Command Dword Structures (Figure 338, 339)
//=============================================================================

/**
 * Lockdown - Command Dword 10
 * Figure 338
 */
union LockdownCDW10 {
    struct {
        uint32_t SCP : 4;       // Bits 03:00 - Scope
        uint32_t PRHBT : 1;     // Bit 04 - Prohibit
        uint32_t IFC : 2;       // Bits 06:05 - Interface
        uint32_t Reserved0 : 1; // Bit 07 - Reserved
        uint32_t OFI : 8;       // Bits 15:08 - Opcode or Feature Identifier
        uint32_t Reserved1 : 16; // Bits 31:16 - Reserved
    } bits;
    uint32_t raw;
};

/**
 * Lockdown - Command Dword 14
 * Figure 339
 */
union LockdownCDW14 {
    struct {
        uint32_t UIDX : 7;      // Bits 06:00 - UUID Index
        uint32_t Reserved : 25; // Bits 31:07 - Reserved
    } bits;
    uint32_t raw;
};

//=============================================================================
// Command Specific Status Values (Figure 340)
//=============================================================================

/**
 * Lockdown - Command Specific Status Values
 * Figure 340 (SCT field set to 1h)
 */
enum class LockdownStatusCode : uint8_t {
    ProhibitionNotSupported = 0x28, // Prohibition of Command Execution Not Supported
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(LockdownCDW10) == sizeof(uint32_t), "LockdownCDW10 size mismatch");
static_assert(sizeof(LockdownCDW14) == sizeof(uint32_t), "LockdownCDW14 size mismatch");

} // namespace nvme

#endif // NVME_LOCKDOWN_H
