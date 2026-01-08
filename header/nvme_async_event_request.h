#ifndef NVME_ASYNC_EVENT_REQUEST_H
#define NVME_ASYNC_EVENT_REQUEST_H

#include <cstdint>

namespace nvme {

//=============================================================================
// Asynchronous Event Type (Figure 148)
//=============================================================================

/**
 * Asynchronous Event Type Values
 * Figure 148
 */
enum class AsyncEventType : uint8_t {
    ErrorStatus           = 0b000, // Error status
    SmartHealthStatus     = 0b001, // SMART / Health status
    Notice                = 0b010, // Notice
    Immediate             = 0b011, // Immediate
    OneShot               = 0b100, // One-Shot
    // 0b101: Reserved
    IOCommandSpecific     = 0b110, // I/O Command specific status
    VendorSpecific        = 0b111, // Vendor specific
};

//=============================================================================
// Completion Queue Entry Structures (Figure 148, 149)
//=============================================================================

/**
 * Asynchronous Event Request - Completion Queue Entry Dword 0
 * Figure 148
 */
union AsyncEventRequestCQEDW0 {
    struct {
        uint32_t AET : 3;           // Bits 02:00 - Asynchronous Event Type
        uint32_t Reserved1 : 5;     // Bits 07:03 - Reserved
        uint32_t AEI : 8;           // Bits 15:08 - Asynchronous Event Information
        uint32_t LID : 8;           // Bits 23:16 - Log Page Identifier
        uint32_t Reserved2 : 8;     // Bits 31:24 - Reserved
    } bits;
    uint32_t raw;
};

/**
 * Asynchronous Event Request - Completion Queue Entry Dword 1
 * Figure 149
 */
union AsyncEventRequestCQEDW1 {
    struct {
        uint32_t EVNTSP : 32;       // Bits 31:00 - Event Specific Parameter
    } bits;
    uint32_t raw;
};

//=============================================================================
// Command Specific Status Values (Figure 147)
//=============================================================================

/**
 * Asynchronous Event Request - Command Specific Status Values
 * Figure 147 (SCT field set to 1h)
 */
enum class AsyncEventRequestStatusCode : uint8_t {
    AsyncEventRequestLimitExceeded = 0x05,  // Asynchronous Event Request Limit Exceeded
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(AsyncEventRequestCQEDW0) == sizeof(uint32_t), "CQEDW0 size mismatch");
static_assert(sizeof(AsyncEventRequestCQEDW1) == sizeof(uint32_t), "CQEDW1 size mismatch");

} // namespace nvme

#endif // NVME_ASYNC_EVENT_REQUEST_H
