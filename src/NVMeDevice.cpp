#include "nvme.h"
#include <cstring>

namespace nvme {

//=============================================================================
// NVMeDevice::Impl - Private Implementation
//=============================================================================

class NVMeDevice::Impl {
public:
    Impl() : m_isOpen(false), m_lastStatusCode(0) {}
    ~Impl() { Close(); }

    bool Open(const std::string& devicePath) {
        // TODO: Implement device open
        m_devicePath = devicePath;
        m_isOpen = true;
        return true;
    }

    void Close() {
        // TODO: Implement device close
        m_isOpen = false;
        m_devicePath.clear();
    }

    bool IsOpen() const { return m_isOpen; }

    std::string GetLastErrorString() const { return m_lastError; }
    uint32_t GetLastStatusCode() const { return m_lastStatusCode; }

    void SetError(const std::string& error, uint32_t statusCode) {
        m_lastError = error;
        m_lastStatusCode = statusCode;
    }

    void ClearError() {
        m_lastError.clear();
        m_lastStatusCode = 0;
    }

private:
    bool m_isOpen;
    std::string m_devicePath;
    std::string m_lastError;
    uint32_t m_lastStatusCode;
};

//=============================================================================
// NVMeDevice - Constructor / Destructor
//=============================================================================

NVMeDevice::NVMeDevice()
    : pImpl(std::make_unique<Impl>()) {
}

NVMeDevice::NVMeDevice(const std::string& devicePath)
    : pImpl(std::make_unique<Impl>()) {
    Open(devicePath);
}

NVMeDevice::~NVMeDevice() = default;

NVMeDevice::NVMeDevice(NVMeDevice&&) noexcept = default;
NVMeDevice& NVMeDevice::operator=(NVMeDevice&&) noexcept = default;

//=============================================================================
// Device Management
//=============================================================================

bool NVMeDevice::Open(const std::string& devicePath) {
    return pImpl->Open(devicePath);
}

void NVMeDevice::Close() {
    pImpl->Close();
}

bool NVMeDevice::IsOpen() const {
    return pImpl->IsOpen();
}

//=============================================================================
// Internal Command Submission
//=============================================================================

bool NVMeDevice::SubmitAdminCommand(const SubmissionQueueEntry& cmd, CompletionQueueEntry& cqe) {
    // TODO: Implement admin command submission
    (void)cmd;
    std::memset(&cqe, 0, sizeof(cqe));
    return true;
}

bool NVMeDevice::SubmitAdminCommand(const SubmissionQueueEntry& cmd, CompletionQueueEntry& cqe,
                                    void* data, size_t dataSize, bool isWrite) {
    // TODO: Implement admin command submission with data transfer
    (void)cmd;
    (void)data;
    (void)dataSize;
    (void)isWrite;
    std::memset(&cqe, 0, sizeof(cqe));
    return true;
}

//=============================================================================
// Identify Commands
//=============================================================================

bool NVMeDevice::Identify(IdentifyCNS cns, uint32_t nsid, void* data, size_t dataSize) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::Identify);
    cmd.NSID = nsid;

    IdentifyCDW10 cdw10{};
    cdw10.bits.CNS = static_cast<uint8_t>(cns);
    cmd.CDW10 = cdw10.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe, data, dataSize, false);
}

bool NVMeDevice::IdentifyController(void* data, size_t dataSize) {
    return Identify(IdentifyCNS::IdentifyController, 0, data, dataSize);
}

bool NVMeDevice::IdentifyNamespace(uint32_t nsid, void* data, size_t dataSize) {
    return Identify(IdentifyCNS::IdentifyNamespace, nsid, data, dataSize);
}

//=============================================================================
// Get/Set Features
//=============================================================================

bool NVMeDevice::GetFeatures(uint8_t fid, uint32_t* result) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::GetFeatures);

    GetFeaturesCDW10 cdw10{};
    cdw10.bits.FID = fid;
    cdw10.bits.SEL = static_cast<uint8_t>(GetFeaturesSelect::Current);
    cmd.CDW10 = cdw10.raw;

    CompletionQueueEntry cqe{};
    bool success = SubmitAdminCommand(cmd, cqe);
    if (success && result) {
        *result = cqe.DW0;
    }
    return success;
}

bool NVMeDevice::SetFeatures(uint8_t fid, uint32_t value, bool save) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::SetFeatures);

    SetFeaturesCDW10 cdw10{};
    cdw10.bits.FID = fid;
    cdw10.bits.SV = save ? 1 : 0;
    cmd.CDW10 = cdw10.raw;
    cmd.CDW11 = value;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe);
}

//=============================================================================
// Get Log Page
//=============================================================================

bool NVMeDevice::GetLogPage(uint8_t lid, uint32_t nsid, void* data, size_t dataSize) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::GetLogPage);
    cmd.NSID = nsid;

    uint32_t numDwords = static_cast<uint32_t>((dataSize + 3) / 4) - 1;

    GetLogPageCDW10 cdw10{};
    cdw10.bits.LID = lid;
    cdw10.bits.NUMDL = numDwords & 0xFFFF;
    cmd.CDW10 = cdw10.raw;

    GetLogPageCDW11 cdw11{};
    cdw11.bits.NUMDU = (numDwords >> 16) & 0xFFFF;
    cmd.CDW11 = cdw11.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe, data, dataSize, false);
}

bool NVMeDevice::GetSmartLog(void* data, size_t dataSize) {
    return GetLogPage(0x02, 0xFFFFFFFF, data, dataSize);
}

bool NVMeDevice::GetErrorLog(void* data, size_t dataSize) {
    return GetLogPage(0x01, 0, data, dataSize);
}

bool NVMeDevice::GetFirmwareSlotInfo(void* data, size_t dataSize) {
    return GetLogPage(0x03, 0, data, dataSize);
}

//=============================================================================
// Firmware Operations
//=============================================================================

bool NVMeDevice::FirmwareImageDownload(const void* data, size_t dataSize, uint32_t offset) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    if (data == nullptr) {
        pImpl->SetError("Invalid data pointer", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::FirmwareImageDownload);

    FirmwareImageDownloadCDW10 cdw10{};
    cdw10.bits.NUMD = static_cast<uint32_t>((dataSize + 3) / 4) - 1;
    cmd.CDW10 = cdw10.raw;

    FirmwareImageDownloadCDW11 cdw11{};
    cdw11.bits.OFST = offset / 4;
    cmd.CDW11 = cdw11.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe, const_cast<void*>(data), dataSize, true);
}

bool NVMeDevice::FirmwareCommit(FirmwareCommitAction action, uint8_t slot) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::FirmwareCommit);

    FirmwareCommitCDW10 cdw10{};
    cdw10.bits.FS = slot;
    cdw10.bits.CA = static_cast<uint8_t>(action);
    cmd.CDW10 = cdw10.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe);
}

//=============================================================================
// Namespace Management
//=============================================================================

bool NVMeDevice::NamespaceCreate(uint32_t* nsid, const void* data, size_t dataSize) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::NamespaceManagement);

    NamespaceManagementCDW10 cdw10{};
    cdw10.bits.SEL = static_cast<uint8_t>(NamespaceManagementSelect::Create);
    cmd.CDW10 = cdw10.raw;

    CompletionQueueEntry cqe{};
    bool success = SubmitAdminCommand(cmd, cqe, const_cast<void*>(data), dataSize, true);
    if (success && nsid) {
        *nsid = cqe.DW0;
    }
    return success;
}

bool NVMeDevice::NamespaceDelete(uint32_t nsid) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::NamespaceManagement);
    cmd.NSID = nsid;

    NamespaceManagementCDW10 cdw10{};
    cdw10.bits.SEL = static_cast<uint8_t>(NamespaceManagementSelect::Delete);
    cmd.CDW10 = cdw10.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe);
}

bool NVMeDevice::NamespaceAttach(uint32_t nsid, const std::vector<uint16_t>& controllers) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    // TODO: Build controller list structure
    (void)controllers;

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::NamespaceAttachment);
    cmd.NSID = nsid;

    NamespaceAttachmentCDW10 cdw10{};
    cdw10.bits.SEL = static_cast<uint8_t>(NamespaceAttachmentSelect::ControllerAttach);
    cmd.CDW10 = cdw10.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe);
}

bool NVMeDevice::NamespaceDetach(uint32_t nsid, const std::vector<uint16_t>& controllers) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    // TODO: Build controller list structure
    (void)controllers;

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::NamespaceAttachment);
    cmd.NSID = nsid;

    NamespaceAttachmentCDW10 cdw10{};
    cdw10.bits.SEL = static_cast<uint8_t>(NamespaceAttachmentSelect::ControllerDetach);
    cmd.CDW10 = cdw10.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe);
}

//=============================================================================
// Format NVM
//=============================================================================

bool NVMeDevice::FormatNVM(uint32_t nsid, uint8_t lbaf, FormatNVMSecureErase ses) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::FormatNVM);
    cmd.NSID = nsid;

    FormatNVMCDW10 cdw10{};
    cdw10.bits.LBAFL = lbaf & 0x0F;
    cdw10.bits.SES = static_cast<uint8_t>(ses);
    cmd.CDW10 = cdw10.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe);
}

//=============================================================================
// Sanitize
//=============================================================================

bool NVMeDevice::Sanitize(SanitizeAction action, uint32_t overwritePattern) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::Sanitize);

    SanitizeCDW10 cdw10{};
    cdw10.bits.SANACT = static_cast<uint8_t>(action);
    cmd.CDW10 = cdw10.raw;

    SanitizeCDW11 cdw11{};
    cdw11.bits.OVRPAT = overwritePattern;
    cmd.CDW11 = cdw11.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe);
}

//=============================================================================
// Device Self-test
//=============================================================================

bool NVMeDevice::DeviceSelfTest(DeviceSelfTestCode stc) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::DeviceSelfTest);

    DeviceSelfTestCDW10 cdw10{};
    cdw10.bits.STC = static_cast<uint8_t>(stc);
    cmd.CDW10 = cdw10.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe);
}

//=============================================================================
// Security Commands
//=============================================================================

bool NVMeDevice::SecuritySend(uint8_t protocol, uint16_t spSpecific, const void* data, size_t dataSize) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::SecuritySend);

    SecuritySendCDW10 cdw10{};
    cdw10.bits.SECP = protocol;
    cdw10.bits.SPSP0 = spSpecific & 0xFF;
    cdw10.bits.SPSP1 = (spSpecific >> 8) & 0xFF;
    cmd.CDW10 = cdw10.raw;

    SecuritySendCDW11 cdw11{};
    cdw11.bits.TL = static_cast<uint32_t>(dataSize);
    cmd.CDW11 = cdw11.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe, const_cast<void*>(data), dataSize, true);
}

bool NVMeDevice::SecurityReceive(uint8_t protocol, uint16_t spSpecific, void* data, size_t dataSize) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::SecurityReceive);

    SecurityReceiveCDW10 cdw10{};
    cdw10.bits.SECP = protocol;
    cdw10.bits.SPSP0 = spSpecific & 0xFF;
    cdw10.bits.SPSP1 = (spSpecific >> 8) & 0xFF;
    cmd.CDW10 = cdw10.raw;

    SecurityReceiveCDW11 cdw11{};
    cdw11.bits.AL = static_cast<uint32_t>(dataSize);
    cmd.CDW11 = cdw11.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe, data, dataSize, false);
}

//=============================================================================
// Keep Alive
//=============================================================================

bool NVMeDevice::KeepAlive() {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::KeepAlive);

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe);
}

//=============================================================================
// Abort
//=============================================================================

bool NVMeDevice::Abort(uint16_t sqid, uint16_t cid) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::Abort);

    AbortCDW10 cdw10{};
    cdw10.bits.SQID = sqid;
    cdw10.bits.CID = cid;
    cmd.CDW10 = cdw10.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe);
}

//=============================================================================
// Lockdown
//=============================================================================

bool NVMeDevice::Lockdown(LockdownScope scope, uint8_t ofi, bool prohibit,
                          LockdownInterface ifc, uint8_t uuidIndex) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::Lockdown);

    LockdownCDW10 cdw10{};
    cdw10.bits.SCP = static_cast<uint8_t>(scope);
    cdw10.bits.PRHBT = prohibit ? 1 : 0;
    cdw10.bits.IFC = static_cast<uint8_t>(ifc);
    cdw10.bits.OFI = ofi;
    cmd.CDW10 = cdw10.raw;

    LockdownCDW14 cdw14{};
    cdw14.bits.UIDX = uuidIndex;
    cmd.CDW14 = cdw14.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe);
}

//=============================================================================
// Virtualization Management
//=============================================================================

bool NVMeDevice::VirtualizationManagement(VirtualizationAction action, uint16_t controllerId,
                                          VirtualizationResourceType resourceType,
                                          uint16_t numResources,
                                          uint16_t* numResourcesModified) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::VirtualizationManagement);

    VirtualizationManagementCDW10 cdw10{};
    cdw10.bits.ACT = static_cast<uint8_t>(action);
    cdw10.bits.RT = static_cast<uint8_t>(resourceType);
    cdw10.bits.CNTLID = controllerId;
    cmd.CDW10 = cdw10.raw;

    VirtualizationManagementCDW11 cdw11{};
    cdw11.bits.NR = numResources;
    cmd.CDW11 = cdw11.raw;

    CompletionQueueEntry cqe{};
    bool result = SubmitAdminCommand(cmd, cqe);

    if (result && numResourcesModified != nullptr) {
        VirtualizationManagementCQEDW0 dw0{};
        dw0.raw = cqe.DW0;
        *numResourcesModified = dw0.bits.NRM;
    }

    return result;
}

//=============================================================================
// Queue Management (PCIe Transport)
//=============================================================================

bool NVMeDevice::CreateIOCompletionQueue(uint16_t qid, uint16_t qsize, uint64_t prp,
                                         uint16_t iv, bool ien, bool pc) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::CreateIOCompletionQueue);
    cmd.PRP1 = prp;

    CreateIOCompletionQueueCDW10 cdw10{};
    cdw10.bits.QID = qid;
    cdw10.bits.QSIZE = qsize;
    cmd.CDW10 = cdw10.raw;

    CreateIOCompletionQueueCDW11 cdw11{};
    cdw11.bits.PC = pc ? 1 : 0;
    cdw11.bits.IEN = ien ? 1 : 0;
    cdw11.bits.IV = iv;
    cmd.CDW11 = cdw11.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe);
}

bool NVMeDevice::CreateIOSubmissionQueue(uint16_t qid, uint16_t qsize, uint64_t prp,
                                         uint16_t cqid, QueuePriority priority, bool pc) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::CreateIOSubmissionQueue);
    cmd.PRP1 = prp;

    CreateIOSubmissionQueueCDW10 cdw10{};
    cdw10.bits.QID = qid;
    cdw10.bits.QSIZE = qsize;
    cmd.CDW10 = cdw10.raw;

    CreateIOSubmissionQueueCDW11 cdw11{};
    cdw11.bits.PC = pc ? 1 : 0;
    cdw11.bits.QPRIO = static_cast<uint8_t>(priority);
    cdw11.bits.CQID = cqid;
    cmd.CDW11 = cdw11.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe);
}

bool NVMeDevice::DeleteIOCompletionQueue(uint16_t qid) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::DeleteIOCompletionQueue);

    DeleteIOCompletionQueueCDW10 cdw10{};
    cdw10.bits.QID = qid;
    cmd.CDW10 = cdw10.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe);
}

bool NVMeDevice::DeleteIOSubmissionQueue(uint16_t qid) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(AdminOpcode::DeleteIOSubmissionQueue);

    DeleteIOSubmissionQueueCDW10 cdw10{};
    cdw10.bits.QID = qid;
    cmd.CDW10 = cdw10.raw;

    CompletionQueueEntry cqe{};
    return SubmitAdminCommand(cmd, cqe);
}

//=============================================================================
// Internal I/O Command Submission
//=============================================================================

bool NVMeDevice::SubmitIOCommand(const SubmissionQueueEntry& cmd, CompletionQueueEntry& cqe,
                                  void* data, size_t dataSize, bool isWrite) {
    // TODO: Implement I/O command submission
    (void)cmd;
    (void)data;
    (void)dataSize;
    (void)isWrite;
    std::memset(&cqe, 0, sizeof(cqe));
    return true;
}

//=============================================================================
// NVM Commands (I/O Commands)
//=============================================================================

bool NVMeDevice::Read(uint32_t nsid, uint64_t slba, uint16_t nlb, void* data, size_t dataSize,
                      bool fua, bool lr) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    if (data == nullptr) {
        pImpl->SetError("Invalid data pointer", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(NvmOpcode::Read);
    cmd.NSID = nsid;

    ReadCDW10 cdw10{};
    cdw10.bits.SLBA_Lower = static_cast<uint32_t>(slba & 0xFFFFFFFF);
    cmd.CDW10 = cdw10.raw;

    ReadCDW11 cdw11{};
    cdw11.bits.SLBA_Upper = static_cast<uint32_t>((slba >> 32) & 0xFFFFFFFF);
    cmd.CDW11 = cdw11.raw;

    ReadCDW12 cdw12{};
    cdw12.bits.NLB = nlb;
    cdw12.bits.FUA = fua ? 1 : 0;
    cdw12.bits.LR = lr ? 1 : 0;
    cmd.CDW12 = cdw12.raw;

    CompletionQueueEntry cqe{};
    return SubmitIOCommand(cmd, cqe, data, dataSize, false);
}

bool NVMeDevice::Write(uint32_t nsid, uint64_t slba, uint16_t nlb, const void* data, size_t dataSize,
                       bool fua, bool lr) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    if (data == nullptr) {
        pImpl->SetError("Invalid data pointer", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(NvmOpcode::Write);
    cmd.NSID = nsid;

    WriteCDW10 cdw10{};
    cdw10.bits.SLBA_Lower = static_cast<uint32_t>(slba & 0xFFFFFFFF);
    cmd.CDW10 = cdw10.raw;

    WriteCDW11 cdw11{};
    cdw11.bits.SLBA_Upper = static_cast<uint32_t>((slba >> 32) & 0xFFFFFFFF);
    cmd.CDW11 = cdw11.raw;

    WriteCDW12 cdw12{};
    cdw12.bits.NLB = nlb;
    cdw12.bits.FUA = fua ? 1 : 0;
    cdw12.bits.LR = lr ? 1 : 0;
    cmd.CDW12 = cdw12.raw;

    CompletionQueueEntry cqe{};
    return SubmitIOCommand(cmd, cqe, const_cast<void*>(data), dataSize, true);
}

bool NVMeDevice::DatasetManagement(uint32_t nsid, const DatasetManagementRange* ranges, uint8_t numRanges,
                                   bool deallocate, bool integralRead, bool integralWrite) {
    if (!IsOpen()) {
        pImpl->SetError("Device not open", 0);
        return false;
    }

    if (ranges == nullptr || numRanges == 0) {
        pImpl->SetError("Invalid range data", 0);
        return false;
    }

    SubmissionQueueEntry cmd{};
    cmd.CDW0 = static_cast<uint32_t>(NvmOpcode::DatasetManagement);
    cmd.NSID = nsid;

    DatasetManagementCDW10 cdw10{};
    cdw10.bits.NR = numRanges - 1;  // 0's based value
    cmd.CDW10 = cdw10.raw;

    DatasetManagementCDW11 cdw11{};
    cdw11.bits.IDR = integralRead ? 1 : 0;
    cdw11.bits.IDW = integralWrite ? 1 : 0;
    cdw11.bits.AD = deallocate ? 1 : 0;
    cmd.CDW11 = cdw11.raw;

    size_t dataSize = static_cast<size_t>(numRanges) * sizeof(DatasetManagementRange);

    CompletionQueueEntry cqe{};
    return SubmitIOCommand(cmd, cqe, const_cast<DatasetManagementRange*>(ranges), dataSize, true);
}

//=============================================================================
// Utility Methods
//=============================================================================

std::string NVMeDevice::GetLastErrorString() const {
    return pImpl->GetLastErrorString();
}

uint32_t NVMeDevice::GetLastStatusCode() const {
    return pImpl->GetLastStatusCode();
}

} // namespace nvme
