#ifndef NVME_H
#define NVME_H

#include <cstdint>
#include <vector>
#include <memory>
#include <string>

// Include all NVMe command headers
#include "nvme_abort.h"
#include "nvme_async_event_request.h"
#include "nvme_capacity_management.h"
#include "nvme_controller_data_queue.h"
#include "nvme_device_self_test.h"
#include "nvme_directive_receive.h"
#include "nvme_directive_send.h"
#include "nvme_firmware_commit.h"
#include "nvme_firmware_image_download.h"
#include "nvme_format_nvm.h"
#include "nvme_get_features.h"
#include "nvme_get_log_page.h"
#include "nvme_identify.h"
#include "nvme_keep_alive.h"
#include "nvme_lockdown.h"
#include "nvme_migration_receive.h"
#include "nvme_migration_send.h"
#include "nvme_namespace_attachment.h"
#include "nvme_namespace_management.h"
#include "nvme_sanitize.h"
#include "nvme_security.h"
#include "nvme_set_features.h"
#include "nvme_pcie_admin_commands.h"
#include "nvme_nvm_commands.h"

namespace nvme {

//=============================================================================
// NVMe Command Opcodes
//=============================================================================

/**
 * Admin Command Opcodes
 */
enum class AdminOpcode : uint8_t {
    DeleteIOSubmissionQueue = 0x00,
    CreateIOSubmissionQueue = 0x01,
    GetLogPage              = 0x02,
    DeleteIOCompletionQueue = 0x04,
    CreateIOCompletionQueue = 0x05,
    Identify                = 0x06,
    Abort                   = 0x08,
    SetFeatures             = 0x09,
    GetFeatures             = 0x0A,
    AsyncEventRequest       = 0x0C,
    NamespaceManagement     = 0x0D,
    FirmwareCommit          = 0x10,
    FirmwareImageDownload   = 0x11,
    DeviceSelfTest          = 0x14,
    NamespaceAttachment     = 0x15,
    KeepAlive               = 0x18,
    DirectiveSend           = 0x19,
    Lockdown                = 0x24,
    DirectiveReceive        = 0x1A,
    VirtualizationManagement = 0x1C,
    NVMeMISend              = 0x1D,
    NVMeMIReceive           = 0x1E,
    CapacityManagement      = 0x20,
    DoorbellBufferConfig    = 0x7C,
    FormatNVM               = 0x80,
    SecuritySend            = 0x81,
    SecurityReceive         = 0x82,
    Sanitize                = 0x84,
    GetLBAStatus            = 0x86,
};

/**
 * NVM Command Opcodes (I/O Commands)
 */
enum class NvmOpcode : uint8_t {
    Flush             = 0x00,
    Write             = 0x01,
    Read              = 0x02,
    WriteUncorrectable = 0x04,
    Compare           = 0x05,
    WriteZeroes       = 0x08,
    DatasetManagement = 0x09,
    Verify            = 0x0C,
    ReservationRegister = 0x0D,
    ReservationReport   = 0x0E,
    ReservationAcquire  = 0x11,
    ReservationRelease  = 0x15,
    Copy              = 0x19,
};

//=============================================================================
// Common NVMe Structures
//=============================================================================

/**
 * NVMe Submission Queue Entry (64 bytes)
 */
struct SubmissionQueueEntry {
    uint32_t CDW0;          // Command Dword 0
    uint32_t NSID;          // Namespace Identifier
    uint32_t CDW2;          // Command Dword 2
    uint32_t CDW3;          // Command Dword 3
    uint64_t MPTR;          // Metadata Pointer
    uint64_t PRP1;          // PRP Entry 1
    uint64_t PRP2;          // PRP Entry 2
    uint32_t CDW10;         // Command Dword 10
    uint32_t CDW11;         // Command Dword 11
    uint32_t CDW12;         // Command Dword 12
    uint32_t CDW13;         // Command Dword 13
    uint32_t CDW14;         // Command Dword 14
    uint32_t CDW15;         // Command Dword 15
};

/**
 * NVMe Completion Queue Entry (16 bytes)
 */
struct CompletionQueueEntry {
    uint32_t DW0;           // Command Specific
    uint32_t DW1;           // Reserved
    uint16_t SQHD;          // SQ Head Pointer
    uint16_t SQID;          // SQ Identifier
    uint16_t CID;           // Command Identifier
    uint16_t Status;        // Status Field
};

/**
 * NVMe Status Code Type
 */
enum class StatusCodeType : uint8_t {
    Generic             = 0x0,
    CommandSpecific     = 0x1,
    MediaDataIntegrity  = 0x2,
    PathRelated         = 0x3,
    VendorSpecific      = 0x7,
};

//=============================================================================
// NVMeDevice Class Declaration
//=============================================================================

/**
 * NVMeDevice - Main class for NVMe device operations
 */
class NVMeDevice {
public:
    // Constructor / Destructor
    NVMeDevice();
    explicit NVMeDevice(const std::string& devicePath);
    ~NVMeDevice();

    // Disable copy
    NVMeDevice(const NVMeDevice&) = delete;
    NVMeDevice& operator=(const NVMeDevice&) = delete;

    // Enable move
    NVMeDevice(NVMeDevice&&) noexcept;
    NVMeDevice& operator=(NVMeDevice&&) noexcept;

    //=========================================================================
    // Device Management
    //=========================================================================
    bool Open(const std::string& devicePath);
    void Close();
    bool IsOpen() const;

    //=========================================================================
    // Admin Commands
    //=========================================================================

    // Identify
    bool Identify(IdentifyCNS cns, uint32_t nsid, void* data, size_t dataSize);
    bool IdentifyController(void* data, size_t dataSize);
    bool IdentifyNamespace(uint32_t nsid, void* data, size_t dataSize);

    // Get/Set Features
    bool GetFeatures(uint8_t fid, uint32_t* result);
    bool SetFeatures(uint8_t fid, uint32_t value, bool save = false);

    // Get Log Page
    bool GetLogPage(uint8_t lid, uint32_t nsid, void* data, size_t dataSize);
    bool GetSmartLog(void* data, size_t dataSize);
    bool GetErrorLog(void* data, size_t dataSize);
    bool GetFirmwareSlotInfo(void* data, size_t dataSize);

    // Firmware Operations
    bool FirmwareImageDownload(const void* data, size_t dataSize, uint32_t offset);
    bool FirmwareCommit(FirmwareCommitAction action, uint8_t slot);

    // Namespace Management
    bool NamespaceCreate(uint32_t* nsid, const void* data, size_t dataSize);
    bool NamespaceDelete(uint32_t nsid);
    bool NamespaceAttach(uint32_t nsid, const std::vector<uint16_t>& controllers);
    bool NamespaceDetach(uint32_t nsid, const std::vector<uint16_t>& controllers);

    // Format NVM
    bool FormatNVM(uint32_t nsid, uint8_t lbaf, FormatNVMSecureErase ses = FormatNVMSecureErase::NoSecureErase);

    // Sanitize
    bool Sanitize(SanitizeAction action, uint32_t overwritePattern = 0);

    // Device Self-test
    bool DeviceSelfTest(DeviceSelfTestCode stc);

    // Security
    bool SecuritySend(uint8_t protocol, uint16_t spSpecific, const void* data, size_t dataSize);
    bool SecurityReceive(uint8_t protocol, uint16_t spSpecific, void* data, size_t dataSize);

    // Keep Alive
    bool KeepAlive();

    // Abort
    bool Abort(uint16_t sqid, uint16_t cid);

    // Lockdown
    bool Lockdown(LockdownScope scope, uint8_t ofi, bool prohibit,
                  LockdownInterface ifc = LockdownInterface::AdminSubmissionQueue,
                  uint8_t uuidIndex = 0);

    // Virtualization Management
    bool VirtualizationManagement(VirtualizationAction action, uint16_t controllerId,
                                  VirtualizationResourceType resourceType = VirtualizationResourceType::VQResources,
                                  uint16_t numResources = 0,
                                  uint16_t* numResourcesModified = nullptr);

    //=========================================================================
    // Queue Management (PCIe Transport)
    //=========================================================================
    bool CreateIOCompletionQueue(uint16_t qid, uint16_t qsize, uint64_t prp, uint16_t iv, bool ien, bool pc);
    bool CreateIOSubmissionQueue(uint16_t qid, uint16_t qsize, uint64_t prp, uint16_t cqid, QueuePriority priority, bool pc);
    bool DeleteIOCompletionQueue(uint16_t qid);
    bool DeleteIOSubmissionQueue(uint16_t qid);

    //=========================================================================
    // NVM Commands (I/O Commands)
    //=========================================================================

    // Read
    bool Read(uint32_t nsid, uint64_t slba, uint16_t nlb, void* data, size_t dataSize,
              bool fua = false, bool lr = false);

    // Write
    bool Write(uint32_t nsid, uint64_t slba, uint16_t nlb, const void* data, size_t dataSize,
               bool fua = false, bool lr = false);

    // Dataset Management (DSM / TRIM)
    bool DatasetManagement(uint32_t nsid, const DatasetManagementRange* ranges, uint8_t numRanges,
                           bool deallocate = true, bool integralRead = false, bool integralWrite = false);

    //=========================================================================
    // Utility Methods
    //=========================================================================
    std::string GetLastErrorString() const;
    uint32_t GetLastStatusCode() const;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;

    // Internal command submission
    bool SubmitAdminCommand(const SubmissionQueueEntry& cmd, CompletionQueueEntry& cqe);
    bool SubmitAdminCommand(const SubmissionQueueEntry& cmd, CompletionQueueEntry& cqe,
                           void* data, size_t dataSize, bool isWrite);
    bool SubmitIOCommand(const SubmissionQueueEntry& cmd, CompletionQueueEntry& cqe,
                         void* data, size_t dataSize, bool isWrite);
};

//=============================================================================
// Static Assertions
//=============================================================================

static_assert(sizeof(SubmissionQueueEntry) == 64, "SQE size mismatch");
static_assert(sizeof(CompletionQueueEntry) == 16, "CQE size mismatch");

} // namespace nvme

#endif // NVME_H
