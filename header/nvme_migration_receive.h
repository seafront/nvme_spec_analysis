#ifndef NVME_MIGRATION_RECEIVE_H
#define NVME_MIGRATION_RECEIVE_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Command Dword Structures (Figure 341-345)
//=============================================================================

/**
 * Select Values for Migration Receive Command
 * Figure 341
 */
enum class MigrationReceiveSelect : uint8_t {
    GetControllerState = 0x00, // Get Controller State
    // 0x01 to 0xFF: Reserved
};

/**
 * Migration Receive - Command Dword 10
 * Figure 341
 */
union MigrationReceiveCDW10 {
    struct {
        uint32_t SEL : 8;       // Bits 07:00 - Select
        uint32_t Reserved : 8;  // Bits 15:08 - Reserved
        uint32_t MOS : 16;      // Bits 31:16 - Management Operation Specific
    } bits;
    uint32_t raw;
};

/**
 * Migration Receive - Command Dword 12
 * Figure 342
 */
union MigrationReceiveCDW12 {
    struct {
        uint32_t OL : 32;       // Bits 31:00 - Offset Lower
    } bits;
    uint32_t raw;
};

/**
 * Migration Receive - Command Dword 13
 * Figure 343
 */
union MigrationReceiveCDW13 {
    struct {
        uint32_t OU : 32;       // Bits 31:00 - Offset Upper
    } bits;
    uint32_t raw;
};

/**
 * Migration Receive - Command Dword 15
 * Figure 345
 */
union MigrationReceiveCDW15 {
    struct {
        uint32_t NUMDL : 32;    // Bits 31:00 - Number of Dwords (0's based)
    } bits;
    uint32_t raw;
};

//=============================================================================
// Command Specific Status Values (Figure 349)
//=============================================================================

/**
 * Migration Receive - Command Specific Status Values
 * Figure 349 (SCT field set to 1h)
 */
enum class MigrationReceiveStatusCode : uint8_t {
    InvalidControllerIdentifier = 0x1F, // Invalid Controller Identifier
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(MigrationReceiveCDW10) == sizeof(uint32_t), "CDW10 size mismatch");
static_assert(sizeof(MigrationReceiveCDW12) == sizeof(uint32_t), "CDW12 size mismatch");
static_assert(sizeof(MigrationReceiveCDW13) == sizeof(uint32_t), "CDW13 size mismatch");
static_assert(sizeof(MigrationReceiveCDW15) == sizeof(uint32_t), "CDW15 size mismatch");

} // namespace nvme

#endif // NVME_MIGRATION_RECEIVE_H
