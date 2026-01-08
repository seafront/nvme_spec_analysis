#ifndef NVME_CONTROLLER_DATA_QUEUE_H
#define NVME_CONTROLLER_DATA_QUEUE_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Command Dword Structures (Figure 162, 164, 165, 168)
//=============================================================================

/**
 * Select Values for Controller Data Queue Command
 * Figure 162
 */
enum class ControllerDataQueueSelect : uint8_t {
    CreateControllerDataQueue = 0x00, // Create Controller Data Queue
    DeleteControllerDataQueue = 0x01, // Delete Controller Data Queue
    // 0x02 to 0xBF: Reserved
    // 0xC0 to 0xFF: Vendor Specific
};

/**
 * Queue Type Values
 * Figure 166
 */
enum class ControllerDataQueueType : uint8_t {
    UserDataMigrationQueue = 0x00, // User Data Migration Queue
    // 0x01 to 0xBF: Reserved
    // 0xC0 to 0xFF: Vendor Specific
};

/**
 * Controller Data Queue - Command Dword 10
 * Figure 162
 */
union ControllerDataQueueCDW10 {
    struct {
        uint32_t SEL : 8;       // Bits 07:00 - Select
        uint32_t Reserved : 8;  // Bits 15:08 - Reserved
        uint32_t MOS : 16;      // Bits 31:16 - Management Operation Specific
    } bits;
    uint32_t raw;
};

/**
 * Create Controller Data Queue - Command Dword 11
 * Figure 164
 */
union CreateControllerDataQueueCDW11 {
    struct {
        uint32_t PC : 1;        // Bit 00 - Physically Contiguous
        uint32_t Reserved : 15; // Bits 15:01 - Reserved
        uint32_t CQS : 16;      // Bits 31:16 - Create Queue Specific
    } bits;
    uint32_t raw;
};

/**
 * Create Controller Data Queue - Command Dword 12
 * Figure 165
 */
union CreateControllerDataQueueCDW12 {
    struct {
        uint32_t CDQSIZE : 32;  // Bits 31:00 - Controller Data Queue Size (in dwords)
    } bits;
    uint32_t raw;
};

/**
 * Delete Controller Data Queue - Command Dword 11
 * Figure 168
 */
union DeleteControllerDataQueueCDW11 {
    struct {
        uint32_t CDQID : 16;    // Bits 15:00 - Controller Data Queue Identifier
        uint32_t Reserved : 16; // Bits 31:16 - Reserved
    } bits;
    uint32_t raw;
};

//=============================================================================
// Completion Queue Entry Structures (Figure 169)
//=============================================================================

/**
 * Controller Data Queue - Completion Queue Entry Dword 0
 * Figure 169
 */
union ControllerDataQueueCQEDW0 {
    struct {
        uint32_t CDQID : 16;    // Bits 15:00 - Controller Data Queue Identifier
        uint32_t Reserved : 16; // Bits 31:16 - Reserved
    } bits;
    uint32_t raw;
};

//=============================================================================
// Command Specific Status Values (Figure 170)
//=============================================================================

/**
 * Controller Data Queue - Command Specific Status Values
 * Figure 170 (SCT field set to 1h)
 */
enum class ControllerDataQueueStatusCode : uint8_t {
    InvalidControllerIdentifier  = 0x1F, // Invalid Controller Identifier
    InvalidControllerDataQueue   = 0x37, // Invalid Controller Data Queue
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(ControllerDataQueueCDW10) == sizeof(uint32_t), "CDW10 size mismatch");
static_assert(sizeof(CreateControllerDataQueueCDW11) == sizeof(uint32_t), "Create CDW11 size mismatch");
static_assert(sizeof(CreateControllerDataQueueCDW12) == sizeof(uint32_t), "Create CDW12 size mismatch");
static_assert(sizeof(DeleteControllerDataQueueCDW11) == sizeof(uint32_t), "Delete CDW11 size mismatch");
static_assert(sizeof(ControllerDataQueueCQEDW0) == sizeof(uint32_t), "CQEDW0 size mismatch");

} // namespace nvme

#endif // NVME_CONTROLLER_DATA_QUEUE_H
