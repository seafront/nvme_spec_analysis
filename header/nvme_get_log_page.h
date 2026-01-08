#ifndef NVME_GET_LOG_PAGE_H
#define NVME_GET_LOG_PAGE_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Command Dword Structures (Figure 198-202)
//=============================================================================

/**
 * Get Log Page - Command Dword 10
 * Figure 198
 */
union GetLogPageCDW10 {
    struct {
        uint32_t LID : 8;       // Bits 07:00 - Log Page Identifier
        uint32_t LSP : 7;       // Bits 14:08 - Log Specific Parameter
        uint32_t RAE : 1;       // Bit 15 - Retain Asynchronous Event
        uint32_t NUMDL : 16;    // Bits 31:16 - Number of Dwords Lower
    } bits;
    uint32_t raw;
};

/**
 * Get Log Page - Command Dword 11
 * Figure 199
 */
union GetLogPageCDW11 {
    struct {
        uint32_t NUMDU : 16;    // Bits 15:00 - Number of Dwords Upper
        uint32_t LSI : 16;      // Bits 31:16 - Log Specific Identifier
    } bits;
    uint32_t raw;
};

/**
 * Get Log Page - Command Dword 12
 * Figure 200
 */
union GetLogPageCDW12 {
    struct {
        uint32_t LPOL : 32;     // Bits 31:00 - Log Page Offset Lower
    } bits;
    uint32_t raw;
};

/**
 * Get Log Page - Command Dword 13
 * Figure 201
 */
union GetLogPageCDW13 {
    struct {
        uint32_t LPOU : 32;     // Bits 31:00 - Log Page Offset Upper
    } bits;
    uint32_t raw;
};

/**
 * Get Log Page - Command Dword 14
 * Figure 202
 */
union GetLogPageCDW14 {
    struct {
        uint32_t UIDX : 7;      // Bits 06:00 - UUID Index
        uint32_t Reserved : 16; // Bits 22:07 - Reserved
        uint32_t OT : 1;        // Bit 23 - Offset Type
        uint32_t CSI : 8;       // Bits 31:24 - Command Set Identifier
    } bits;
    uint32_t raw;
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(GetLogPageCDW10) == sizeof(uint32_t), "CDW10 size mismatch");
static_assert(sizeof(GetLogPageCDW11) == sizeof(uint32_t), "CDW11 size mismatch");
static_assert(sizeof(GetLogPageCDW12) == sizeof(uint32_t), "CDW12 size mismatch");
static_assert(sizeof(GetLogPageCDW13) == sizeof(uint32_t), "CDW13 size mismatch");
static_assert(sizeof(GetLogPageCDW14) == sizeof(uint32_t), "CDW14 size mismatch");

} // namespace nvme

#endif // NVME_GET_LOG_PAGE_H
