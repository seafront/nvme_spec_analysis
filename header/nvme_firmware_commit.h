#ifndef NVME_FIRMWARE_COMMIT_H
#define NVME_FIRMWARE_COMMIT_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Command Dword Structures (Figure 182)
//=============================================================================

/**
 * Commit Action (CA) Values for Firmware Commit Command
 * Figure 182: Firmware Commit - Command Dword 10
 */
enum class FirmwareCommitAction : uint8_t {
    ReplaceNotActivate          = 0b000, // Downloaded image replaces existing, not activated
    ReplaceActivateOnReset      = 0b001, // Downloaded image replaces existing, activate at next Controller Level Reset
    ActivateExistingOnReset     = 0b010, // Existing image in slot activated at next Controller Level Reset
    ReplaceActivateImmediate    = 0b011, // Downloaded image replaces existing and activate immediately
    // 0b100 to 0b101: Reserved
    ReplaceBootPartition        = 0b110, // Downloaded image replaces Boot Partition specified by BPID
    MarkBootPartitionActive     = 0b111, // Mark Boot Partition specified in BPID as active
};

/**
 * Firmware Commit - Command Dword 10
 * Figure 182
 */
union FirmwareCommitCDW10 {
    struct {
        uint32_t FS : 3;        // Bits 02:00 - Firmware Slot (FS)
        uint32_t CA : 3;        // Bits 05:03 - Commit Action (CA)
        uint32_t Reserved : 25; // Bits 30:06 - Reserved
        uint32_t BPID : 1;      // Bit 31 - Boot Partition ID (BPID)
    } bits;
    uint32_t raw;
};

//=============================================================================
// Completion Queue Entry Structures (Figure 183)
//=============================================================================

/**
 * Firmware Commit - Completion Queue Entry Dword 0
 * Figure 183
 */
union FirmwareCommitCQEDW0 {
    struct {
        uint32_t ASQFWO : 1;    // Bit 0 - Admin Submission Queue FW Overlap
        uint32_t MEFWO : 1;     // Bit 1 - Management Endpoint FW Overlap
        uint32_t Reserved : 30; // Bits 31:02 - Reserved
    } bits;
    uint32_t raw;
};

//=============================================================================
// Command Specific Status Values (Figure 184)
//=============================================================================

/**
 * Firmware Commit - Command Specific Status Values
 * Figure 184 (SCT field set to 1h)
 */
enum class FirmwareCommitStatusCode : uint8_t {
    InvalidFirmwareSlot                      = 0x06, // Invalid Firmware Slot
    InvalidFirmwareImage                     = 0x07, // Invalid Firmware Image
    FirmwareActivationRequiresConventionalReset = 0x0B, // Firmware Activation Requires Conventional Reset
    FirmwareActivationRequiresNVMSubsystemReset = 0x10, // Firmware Activation Requires NVM Subsystem Reset
    FirmwareActivationRequiresControllerLevelReset = 0x11, // Firmware Activation Requires Controller Level Reset
    FirmwareActivationRequiresMaxTimeViolation = 0x12, // Firmware Activation Requires Maximum Time Violation
    FirmwareActivationProhibited             = 0x13, // Firmware Activation Prohibited
    OverlappingRange                         = 0x14, // Overlapping Range
    BootPartitionWriteProhibited             = 0x1E, // Boot Partition Write Prohibited
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(FirmwareCommitCDW10) == sizeof(uint32_t), "CDW10 size mismatch");
static_assert(sizeof(FirmwareCommitCQEDW0) == sizeof(uint32_t), "CQEDW0 size mismatch");

} // namespace nvme

#endif // NVME_FIRMWARE_COMMIT_H
