#ifndef NVME_PCIE_ADMIN_COMMANDS_H
#define NVME_PCIE_ADMIN_COMMANDS_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Create I/O Completion Queue (Figure 475, 476)
//=============================================================================

/**
 * Create I/O Completion Queue - Command Dword 10
 * Figure 475
 */
union CreateIOCompletionQueueCDW10 {
    struct {
        uint32_t QID : 16;      // Bits 15:00 - Queue Identifier
        uint32_t QSIZE : 16;    // Bits 31:16 - Queue Size (0's based)
    } bits;
    uint32_t raw;
};

/**
 * Create I/O Completion Queue - Command Dword 11
 * Figure 476
 */
union CreateIOCompletionQueueCDW11 {
    struct {
        uint32_t PC : 1;        // Bit 00 - Physically Contiguous
        uint32_t IEN : 1;       // Bit 01 - Interrupts Enabled
        uint32_t Reserved : 14; // Bits 15:02 - Reserved
        uint32_t IV : 16;       // Bits 31:16 - Interrupt Vector
    } bits;
    uint32_t raw;
};

/**
 * Create I/O Completion Queue - Status Values
 * Figure 477
 */
enum class CreateIOCQStatusCode : uint8_t {
    InvalidQueueIdentifier  = 0x01, // Invalid Queue Identifier
    InvalidQueueSize        = 0x02, // Invalid Queue Size
    InvalidInterruptVector  = 0x08, // Invalid Interrupt Vector
};

//=============================================================================
// Create I/O Submission Queue (Figure 479, 480, 481)
//=============================================================================

/**
 * Queue Priority Values
 * Figure 480
 */
enum class QueuePriority : uint8_t {
    Urgent = 0b00,
    High   = 0b01,
    Medium = 0b10,
    Low    = 0b11,
};

/**
 * Create I/O Submission Queue - Command Dword 10
 * Figure 479
 */
union CreateIOSubmissionQueueCDW10 {
    struct {
        uint32_t QID : 16;      // Bits 15:00 - Queue Identifier
        uint32_t QSIZE : 16;    // Bits 31:16 - Queue Size (0's based)
    } bits;
    uint32_t raw;
};

/**
 * Create I/O Submission Queue - Command Dword 11
 * Figure 480
 */
union CreateIOSubmissionQueueCDW11 {
    struct {
        uint32_t PC : 1;        // Bit 00 - Physically Contiguous
        uint32_t QPRIO : 2;     // Bits 02:01 - Queue Priority
        uint32_t Reserved : 13; // Bits 15:03 - Reserved
        uint32_t CQID : 16;     // Bits 31:16 - Completion Queue Identifier
    } bits;
    uint32_t raw;
};

/**
 * Create I/O Submission Queue - Command Dword 12
 * Figure 481
 */
union CreateIOSubmissionQueueCDW12 {
    struct {
        uint32_t NVMSETID : 16; // Bits 15:00 - NVM Set Identifier
        uint32_t Reserved : 16; // Bits 31:16 - Reserved
    } bits;
    uint32_t raw;
};

/**
 * Create I/O Submission Queue - Status Values
 * Figure 482
 */
enum class CreateIOSQStatusCode : uint8_t {
    CompletionQueueInvalid  = 0x00, // Completion Queue Invalid
    InvalidQueueIdentifier  = 0x01, // Invalid Queue Identifier
    InvalidQueueSize        = 0x02, // Invalid Queue Size
};

//=============================================================================
// Delete I/O Completion Queue (Figure 483)
//=============================================================================

/**
 * Delete I/O Completion Queue - Command Dword 10
 * Figure 483
 */
union DeleteIOCompletionQueueCDW10 {
    struct {
        uint32_t QID : 16;      // Bits 15:00 - Queue Identifier
        uint32_t Reserved : 16; // Bits 31:16 - Reserved
    } bits;
    uint32_t raw;
};

/**
 * Delete I/O Completion Queue - Status Values
 * Figure 484
 */
enum class DeleteIOCQStatusCode : uint8_t {
    InvalidQueueIdentifier = 0x01, // Invalid Queue Identifier
    InvalidQueueDeletion   = 0x0C, // Invalid Queue Deletion
};

//=============================================================================
// Delete I/O Submission Queue (Figure 485)
//=============================================================================

/**
 * Delete I/O Submission Queue - Command Dword 10
 * Figure 485
 */
union DeleteIOSubmissionQueueCDW10 {
    struct {
        uint32_t QID : 16;      // Bits 15:00 - Queue Identifier
        uint32_t Reserved : 16; // Bits 31:16 - Reserved
    } bits;
    uint32_t raw;
};

/**
 * Delete I/O Submission Queue - Status Values
 * Figure 486
 */
enum class DeleteIOSQStatusCode : uint8_t {
    InvalidQueueIdentifier = 0x01, // Invalid Queue Identifier
};

//=============================================================================
// Virtualization Management (Figure 490, 491)
//=============================================================================

/**
 * Resource Type Values
 * Figure 490
 */
enum class VirtualizationResourceType : uint8_t {
    VQResources = 0b000, // VQ Resources
    VIResources = 0b001, // VI Resources
    // 0b010 to 0b111: Reserved
};

/**
 * Action Values
 * Figure 490
 */
enum class VirtualizationAction : uint8_t {
    // 0x0: Reserved
    PrimaryControllerFlexibleAllocation = 0x1, // Primary Controller Flexible Allocation
    // 0x2 to 0x6: Reserved
    SecondaryControllerOffline          = 0x7, // Secondary Controller Offline
    SecondaryControllerAssign           = 0x8, // Secondary Controller Assign
    SecondaryControllerOnline           = 0x9, // Secondary Controller Online
    // 0xA to 0xF: Reserved
};

/**
 * Virtualization Management - Command Dword 10
 * Figure 490
 */
union VirtualizationManagementCDW10 {
    struct {
        uint32_t ACT : 4;       // Bits 03:00 - Action
        uint32_t Reserved1 : 4; // Bits 07:04 - Reserved
        uint32_t RT : 3;        // Bits 10:08 - Resource Type
        uint32_t Reserved2 : 5; // Bits 15:11 - Reserved
        uint32_t CNTLID : 16;   // Bits 31:16 - Controller Identifier
    } bits;
    uint32_t raw;
};

/**
 * Virtualization Management - Command Dword 11
 * Figure 491
 */
union VirtualizationManagementCDW11 {
    struct {
        uint32_t NR : 16;       // Bits 15:00 - Number of Controller Resources
        uint32_t Reserved : 16; // Bits 31:16 - Reserved
    } bits;
    uint32_t raw;
};

/**
 * Virtualization Management - Completion Queue Entry Dword 0
 * Figure 493
 */
union VirtualizationManagementCQEDW0 {
    struct {
        uint32_t NRM : 16;      // Bits 15:00 - Number of Controller Resources Modified
        uint32_t Reserved : 16; // Bits 31:16 - Reserved
    } bits;
    uint32_t raw;
};

/**
 * Virtualization Management - Status Values
 * Figure 492
 */
enum class VirtualizationManagementStatusCode : uint8_t {
    InvalidControllerIdentifier     = 0x1F, // Invalid Controller Identifier
    InvalidSecondaryControllerState = 0x20, // Invalid Secondary Controller State
    InvalidNumberOfControllerResources = 0x21, // Invalid Number of Controller Resources
    InvalidResourceIdentifier       = 0x22, // Invalid Resource Identifier
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(CreateIOCompletionQueueCDW10) == sizeof(uint32_t), "CreateIOCQ CDW10 size mismatch");
static_assert(sizeof(CreateIOCompletionQueueCDW11) == sizeof(uint32_t), "CreateIOCQ CDW11 size mismatch");
static_assert(sizeof(CreateIOSubmissionQueueCDW10) == sizeof(uint32_t), "CreateIOSQ CDW10 size mismatch");
static_assert(sizeof(CreateIOSubmissionQueueCDW11) == sizeof(uint32_t), "CreateIOSQ CDW11 size mismatch");
static_assert(sizeof(CreateIOSubmissionQueueCDW12) == sizeof(uint32_t), "CreateIOSQ CDW12 size mismatch");
static_assert(sizeof(DeleteIOCompletionQueueCDW10) == sizeof(uint32_t), "DeleteIOCQ CDW10 size mismatch");
static_assert(sizeof(DeleteIOSubmissionQueueCDW10) == sizeof(uint32_t), "DeleteIOSQ CDW10 size mismatch");
static_assert(sizeof(VirtualizationManagementCDW10) == sizeof(uint32_t), "Virtualization CDW10 size mismatch");
static_assert(sizeof(VirtualizationManagementCDW11) == sizeof(uint32_t), "Virtualization CDW11 size mismatch");
static_assert(sizeof(VirtualizationManagementCQEDW0) == sizeof(uint32_t), "Virtualization CQEDW0 size mismatch");

} // namespace nvme

#endif // NVME_PCIE_ADMIN_COMMANDS_H
