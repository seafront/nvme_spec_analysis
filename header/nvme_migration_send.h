#ifndef NVME_MIGRATION_SEND_H
#define NVME_MIGRATION_SEND_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Command Dword Structures (Figure 350)
//=============================================================================

/**
 * Select Values for Migration Send Command
 * Figure 350
 */
enum class MigrationSendSelect : uint8_t {
    Suspend            = 0x00, // Suspend
    Resume             = 0x01, // Resume
    SetControllerState = 0x02, // Set Controller State
    // 0x03 to 0xFF: Reserved
};

/**
 * Migration Send - Command Dword 10
 * Figure 350
 */
union MigrationSendCDW10 {
    struct {
        uint32_t SEL : 8;       // Bits 07:00 - Select
        uint32_t Reserved : 8;  // Bits 15:08 - Reserved
        uint32_t MOS : 16;      // Bits 31:16 - Management Operation Specific
    } bits;
    uint32_t raw;
};

//=============================================================================
// Command Specific Status Values (Figure 363)
//=============================================================================

/**
 * Migration Send - Command Specific Status Values
 * Figure 363 (SCT field set to 1h)
 */
enum class MigrationSendStatusCode : uint8_t {
    InvalidControllerIdentifier  = 0x1F, // Invalid Controller Identifier
    InvalidControllerDataQueue   = 0x37, // Invalid Controller Data Queue
    NotEnoughResources           = 0x38, // Not Enough Resources
    ControllerNotSuspended       = 0x3A, // Controller Not Suspended
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(MigrationSendCDW10) == sizeof(uint32_t), "CDW10 size mismatch");

} // namespace nvme

#endif // NVME_MIGRATION_SEND_H
