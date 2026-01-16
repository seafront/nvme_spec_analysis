#ifndef NVME_NVM_COMMANDS_H
#define NVME_NVM_COMMANDS_H

#include <cstdint>

namespace nvme {

//=============================================================================
// NVM Read Command (Opcode 0x02)
// NVMe Specification Section 6.x
//=============================================================================

/**
 * Read Command - Command Dword 10
 * Starting LBA (lower 32 bits)
 */
union ReadCDW10 {
    struct {
        uint32_t SLBA_Lower;    // Bits 31:00 - Starting LBA (lower 32 bits)
    } bits;
    uint32_t raw;
};

/**
 * Read Command - Command Dword 11
 * Starting LBA (upper 32 bits)
 */
union ReadCDW11 {
    struct {
        uint32_t SLBA_Upper;    // Bits 31:00 - Starting LBA (upper 32 bits)
    } bits;
    uint32_t raw;
};

/**
 * Read Command - Command Dword 12
 */
union ReadCDW12 {
    struct {
        uint32_t NLB       : 16;  // Bits 15:00 - Number of Logical Blocks (0's based)
        uint32_t Reserved1 : 4;   // Bits 19:16 - Reserved
        uint32_t DTYPE     : 4;   // Bits 23:20 - Directive Type
        uint32_t Reserved2 : 1;   // Bit 24 - Reserved
        uint32_t STC       : 1;   // Bit 25 - Storage Tag Check
        uint32_t PRINFO    : 4;   // Bits 29:26 - Protection Information Field
        uint32_t FUA       : 1;   // Bit 30 - Force Unit Access
        uint32_t LR        : 1;   // Bit 31 - Limited Retry
    } bits;
    uint32_t raw;
};

/**
 * Read Command - Command Dword 13
 */
union ReadCDW13 {
    struct {
        uint32_t DSM;             // Dataset Management
    } bits;
    uint32_t raw;
};

/**
 * Read Command - Command Dword 14
 */
union ReadCDW14 {
    struct {
        uint32_t EILBRT;          // Expected Initial Logical Block Reference Tag
    } bits;
    uint32_t raw;
};

/**
 * Read Command - Command Dword 15
 */
union ReadCDW15 {
    struct {
        uint32_t ELBAT  : 16;     // Bits 15:00 - Expected Logical Block Application Tag
        uint32_t ELBATM : 16;     // Bits 31:16 - Expected Logical Block Application Tag Mask
    } bits;
    uint32_t raw;
};

//=============================================================================
// NVM Write Command (Opcode 0x01)
// NVMe Specification Section 6.x
//=============================================================================

/**
 * Write Command - Command Dword 10
 * Starting LBA (lower 32 bits)
 */
union WriteCDW10 {
    struct {
        uint32_t SLBA_Lower;    // Bits 31:00 - Starting LBA (lower 32 bits)
    } bits;
    uint32_t raw;
};

/**
 * Write Command - Command Dword 11
 * Starting LBA (upper 32 bits)
 */
union WriteCDW11 {
    struct {
        uint32_t SLBA_Upper;    // Bits 31:00 - Starting LBA (upper 32 bits)
    } bits;
    uint32_t raw;
};

/**
 * Write Command - Command Dword 12
 */
union WriteCDW12 {
    struct {
        uint32_t NLB       : 16;  // Bits 15:00 - Number of Logical Blocks (0's based)
        uint32_t Reserved1 : 4;   // Bits 19:16 - Reserved
        uint32_t DTYPE     : 4;   // Bits 23:20 - Directive Type
        uint32_t Reserved2 : 1;   // Bit 24 - Reserved
        uint32_t STC       : 1;   // Bit 25 - Storage Tag Check
        uint32_t PRINFO    : 4;   // Bits 29:26 - Protection Information Field
        uint32_t FUA       : 1;   // Bit 30 - Force Unit Access
        uint32_t LR        : 1;   // Bit 31 - Limited Retry
    } bits;
    uint32_t raw;
};

/**
 * Write Command - Command Dword 13
 */
union WriteCDW13 {
    struct {
        uint32_t DSM;             // Dataset Management
    } bits;
    uint32_t raw;
};

/**
 * Write Command - Command Dword 14
 */
union WriteCDW14 {
    struct {
        uint32_t ILBRT;           // Initial Logical Block Reference Tag
    } bits;
    uint32_t raw;
};

/**
 * Write Command - Command Dword 15
 */
union WriteCDW15 {
    struct {
        uint32_t LBAT  : 16;      // Bits 15:00 - Logical Block Application Tag
        uint32_t LBATM : 16;      // Bits 31:16 - Logical Block Application Tag Mask
    } bits;
    uint32_t raw;
};

//=============================================================================
// Dataset Management (DSM) Command (Opcode 0x09)
// NVMe Specification Section 6.x
//=============================================================================

/**
 * Dataset Management Command - Command Dword 10
 */
union DatasetManagementCDW10 {
    struct {
        uint32_t NR : 8;          // Bits 07:00 - Number of Ranges (0's based)
        uint32_t Reserved : 24;   // Bits 31:08 - Reserved
    } bits;
    uint32_t raw;
};

/**
 * Dataset Management Command - Command Dword 11
 */
union DatasetManagementCDW11 {
    struct {
        uint32_t IDR       : 1;   // Bit 00 - Integral Dataset for Read
        uint32_t IDW       : 1;   // Bit 01 - Integral Dataset for Write
        uint32_t AD        : 1;   // Bit 02 - Deallocate (TRIM)
        uint32_t Reserved  : 29;  // Bits 31:03 - Reserved
    } bits;
    uint32_t raw;
};

/**
 * Dataset Management Range Definition
 * Each range is 16 bytes
 */
struct DatasetManagementRange {
    uint32_t ContextAttributes;   // Dword 0 - Context Attributes
    uint32_t LengthInLBs;         // Dword 1 - Length in Logical Blocks
    uint64_t StartingLBA;         // Dwords 2-3 - Starting LBA
};

/**
 * Dataset Management Context Attributes (Dword 0 of Range)
 */
union DSMContextAttributes {
    struct {
        uint32_t AF             : 4;   // Bits 03:00 - Access Frequency
        uint32_t AL             : 2;   // Bits 05:04 - Access Latency
        uint32_t Reserved1      : 2;   // Bits 07:06 - Reserved
        uint32_t SR             : 1;   // Bit 08 - Sequential Read Range
        uint32_t SW             : 1;   // Bit 09 - Sequential Write Range
        uint32_t WP             : 1;   // Bit 10 - Write Prepare
        uint32_t Reserved2      : 13;  // Bits 23:11 - Reserved
        uint32_t CommandAccess  : 8;   // Bits 31:24 - Command Access Size
    } bits;
    uint32_t raw;
};

/**
 * Access Frequency values for DSM Context Attributes
 */
enum class DSMAccessFrequency : uint8_t {
    NoFrequencyInfo     = 0x0,    // No frequency information provided
    Typical             = 0x1,    // Typical number of reads and writes
    InfrequentWrites    = 0x2,    // Infrequent writes and frequent reads
    InfrequentReads     = 0x3,    // Infrequent writes and infrequent reads
    InfrequentRW        = 0x4,    // Frequent writes and infrequent reads
    FrequentWrites      = 0x5,    // Frequent writes and frequent reads
    OneTimeRead         = 0x6,    // One time read
    Speculative         = 0x7,    // Speculative read
    WillBeOverwritten   = 0x8,    // Data will be overwritten soon
};

/**
 * Access Latency values for DSM Context Attributes
 */
enum class DSMAccessLatency : uint8_t {
    None    = 0x0,    // None (no latency information)
    Idle    = 0x1,    // Idle (longer latency acceptable)
    Normal  = 0x2,    // Normal (typical latency)
    Low     = 0x3,    // Low (smallest possible latency)
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(ReadCDW10) == sizeof(uint32_t), "ReadCDW10 size mismatch");
static_assert(sizeof(ReadCDW11) == sizeof(uint32_t), "ReadCDW11 size mismatch");
static_assert(sizeof(ReadCDW12) == sizeof(uint32_t), "ReadCDW12 size mismatch");
static_assert(sizeof(ReadCDW13) == sizeof(uint32_t), "ReadCDW13 size mismatch");
static_assert(sizeof(ReadCDW14) == sizeof(uint32_t), "ReadCDW14 size mismatch");
static_assert(sizeof(ReadCDW15) == sizeof(uint32_t), "ReadCDW15 size mismatch");

static_assert(sizeof(WriteCDW10) == sizeof(uint32_t), "WriteCDW10 size mismatch");
static_assert(sizeof(WriteCDW11) == sizeof(uint32_t), "WriteCDW11 size mismatch");
static_assert(sizeof(WriteCDW12) == sizeof(uint32_t), "WriteCDW12 size mismatch");
static_assert(sizeof(WriteCDW13) == sizeof(uint32_t), "WriteCDW13 size mismatch");
static_assert(sizeof(WriteCDW14) == sizeof(uint32_t), "WriteCDW14 size mismatch");
static_assert(sizeof(WriteCDW15) == sizeof(uint32_t), "WriteCDW15 size mismatch");

static_assert(sizeof(DatasetManagementCDW10) == sizeof(uint32_t), "DatasetManagementCDW10 size mismatch");
static_assert(sizeof(DatasetManagementCDW11) == sizeof(uint32_t), "DatasetManagementCDW11 size mismatch");
static_assert(sizeof(DatasetManagementRange) == 16, "DatasetManagementRange size mismatch");
static_assert(sizeof(DSMContextAttributes) == sizeof(uint32_t), "DSMContextAttributes size mismatch");

} // namespace nvme

#endif // NVME_NVM_COMMANDS_H
