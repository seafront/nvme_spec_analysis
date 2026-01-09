#include <gtest/gtest.h>
#include "nvme.h"
#include <vector>

//=============================================================================
// NVMeDevice Lifecycle Tests
//=============================================================================

TEST(NVMeDeviceTest, DefaultConstructor) {
    nvme::NVMeDevice device;
    EXPECT_FALSE(device.IsOpen());
}

TEST(NVMeDeviceTest, ConstructorWithPath) {
    nvme::NVMeDevice device("/dev/nvme0");
    EXPECT_TRUE(device.IsOpen());
}

TEST(NVMeDeviceTest, OpenAndClose) {
    nvme::NVMeDevice device;
    EXPECT_FALSE(device.IsOpen());

    EXPECT_TRUE(device.Open("/dev/nvme0"));
    EXPECT_TRUE(device.IsOpen());

    device.Close();
    EXPECT_FALSE(device.IsOpen());
}

TEST(NVMeDeviceTest, MultipleOpenClose) {
    nvme::NVMeDevice device;

    EXPECT_TRUE(device.Open("/dev/nvme0"));
    EXPECT_TRUE(device.IsOpen());
    device.Close();
    EXPECT_FALSE(device.IsOpen());

    EXPECT_TRUE(device.Open("/dev/nvme1"));
    EXPECT_TRUE(device.IsOpen());
    device.Close();
    EXPECT_FALSE(device.IsOpen());
}

//=============================================================================
// NVMeDevice Move Semantics Tests
//=============================================================================

TEST(NVMeDeviceTest, MoveConstructor) {
    nvme::NVMeDevice device1("/dev/nvme0");
    EXPECT_TRUE(device1.IsOpen());

    nvme::NVMeDevice device2(std::move(device1));
    EXPECT_TRUE(device2.IsOpen());
}

TEST(NVMeDeviceTest, MoveAssignment) {
    nvme::NVMeDevice device1("/dev/nvme0");
    nvme::NVMeDevice device2;

    EXPECT_TRUE(device1.IsOpen());
    EXPECT_FALSE(device2.IsOpen());

    device2 = std::move(device1);
    EXPECT_TRUE(device2.IsOpen());
}

//=============================================================================
// NVMeDevice Commands - Device Not Open
//=============================================================================

TEST(NVMeDeviceTest, IdentifyFailsWhenNotOpen) {
    nvme::NVMeDevice device;
    uint8_t buffer[4096] = {0};

    EXPECT_FALSE(device.IdentifyController(buffer, sizeof(buffer)));
    EXPECT_FALSE(device.IdentifyNamespace(1, buffer, sizeof(buffer)));
}

TEST(NVMeDeviceTest, GetFeaturesFailsWhenNotOpen) {
    nvme::NVMeDevice device;
    uint32_t result = 0;

    EXPECT_FALSE(device.GetFeatures(0x01, &result));
}

TEST(NVMeDeviceTest, SetFeaturesFailsWhenNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.SetFeatures(0x01, 0x1234));
}

TEST(NVMeDeviceTest, GetLogPageFailsWhenNotOpen) {
    nvme::NVMeDevice device;
    uint8_t buffer[512] = {0};

    EXPECT_FALSE(device.GetSmartLog(buffer, sizeof(buffer)));
    EXPECT_FALSE(device.GetErrorLog(buffer, sizeof(buffer)));
    EXPECT_FALSE(device.GetFirmwareSlotInfo(buffer, sizeof(buffer)));
}

TEST(NVMeDeviceTest, FirmwareOpsFailWhenNotOpen) {
    nvme::NVMeDevice device;
    uint8_t buffer[512] = {0};

    EXPECT_FALSE(device.FirmwareImageDownload(buffer, sizeof(buffer), 0));
    EXPECT_FALSE(device.FirmwareCommit(nvme::FirmwareCommitAction::ReplaceActivateOnReset, 1));
}

TEST(NVMeDeviceTest, NamespaceOpsFailWhenNotOpen) {
    nvme::NVMeDevice device;
    uint8_t buffer[4096] = {0};
    uint32_t nsid = 0;
    std::vector<uint16_t> controllers = {0, 1};

    EXPECT_FALSE(device.NamespaceCreate(&nsid, buffer, sizeof(buffer)));
    EXPECT_FALSE(device.NamespaceDelete(1));
    EXPECT_FALSE(device.NamespaceAttach(1, controllers));
    EXPECT_FALSE(device.NamespaceDetach(1, controllers));
}

TEST(NVMeDeviceTest, FormatAndSanitizeFailWhenNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.FormatNVM(1, 0));
    EXPECT_FALSE(device.Sanitize(nvme::SanitizeAction::BlockErase));
}

TEST(NVMeDeviceTest, MiscCommandsFailWhenNotOpen) {
    nvme::NVMeDevice device;
    uint8_t buffer[512] = {0};

    EXPECT_FALSE(device.DeviceSelfTest(nvme::DeviceSelfTestCode::ShortDeviceSelfTest));
    EXPECT_FALSE(device.SecuritySend(0x00, 0x0000, buffer, sizeof(buffer)));
    EXPECT_FALSE(device.SecurityReceive(0x00, 0x0000, buffer, sizeof(buffer)));
    EXPECT_FALSE(device.KeepAlive());
    EXPECT_FALSE(device.Abort(0, 0));
}

TEST(NVMeDeviceTest, QueueOpsFailWhenNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.CreateIOCompletionQueue(1, 64, 0x1000, 0, true, true));
    EXPECT_FALSE(device.CreateIOSubmissionQueue(1, 64, 0x2000, 1, nvme::QueuePriority::Medium, true));
    EXPECT_FALSE(device.DeleteIOCompletionQueue(1));
    EXPECT_FALSE(device.DeleteIOSubmissionQueue(1));
}

//=============================================================================
// NVMeDevice Commands - Device Open (stub returns success)
//=============================================================================

TEST(NVMeDeviceTest, IdentifySucceedsWhenOpen) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};

    EXPECT_TRUE(device.IdentifyController(buffer, sizeof(buffer)));
    EXPECT_TRUE(device.IdentifyNamespace(1, buffer, sizeof(buffer)));
}

TEST(NVMeDeviceTest, GetSetFeaturesSucceedWhenOpen) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint32_t result = 0;

    EXPECT_TRUE(device.GetFeatures(0x01, &result));
    EXPECT_TRUE(device.SetFeatures(0x01, 0x1234));
    EXPECT_TRUE(device.SetFeatures(0x01, 0x1234, true));  // with save
}

TEST(NVMeDeviceTest, GetLogPageSucceedsWhenOpen) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[512] = {0};

    EXPECT_TRUE(device.GetSmartLog(buffer, sizeof(buffer)));
    EXPECT_TRUE(device.GetErrorLog(buffer, sizeof(buffer)));
    EXPECT_TRUE(device.GetFirmwareSlotInfo(buffer, sizeof(buffer)));
    EXPECT_TRUE(device.GetLogPage(0x05, 0, buffer, sizeof(buffer)));
}

TEST(NVMeDeviceTest, FirmwareOpsSucceedWhenOpen) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[512] = {0};

    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 0));
    EXPECT_TRUE(device.FirmwareCommit(nvme::FirmwareCommitAction::ReplaceActivateOnReset, 1));
}

TEST(NVMeDeviceTest, NamespaceOpsSucceedWhenOpen) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};
    uint32_t nsid = 0;
    std::vector<uint16_t> controllers = {0, 1};

    EXPECT_TRUE(device.NamespaceCreate(&nsid, buffer, sizeof(buffer)));
    EXPECT_TRUE(device.NamespaceDelete(1));
    EXPECT_TRUE(device.NamespaceAttach(1, controllers));
    EXPECT_TRUE(device.NamespaceDetach(1, controllers));
}

TEST(NVMeDeviceTest, FormatAndSanitizeSucceedWhenOpen) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.FormatNVM(1, 0));
    EXPECT_TRUE(device.FormatNVM(1, 0, nvme::FormatNVMSecureErase::UserDataErase));
    EXPECT_TRUE(device.Sanitize(nvme::SanitizeAction::BlockErase));
    EXPECT_TRUE(device.Sanitize(nvme::SanitizeAction::Overwrite, 0xDEADBEEF));
}

TEST(NVMeDeviceTest, MiscCommandsSucceedWhenOpen) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[512] = {0};

    EXPECT_TRUE(device.DeviceSelfTest(nvme::DeviceSelfTestCode::ShortDeviceSelfTest));
    EXPECT_TRUE(device.DeviceSelfTest(nvme::DeviceSelfTestCode::ExtendedDeviceSelfTest));
    EXPECT_TRUE(device.SecuritySend(0x00, 0x0000, buffer, sizeof(buffer)));
    EXPECT_TRUE(device.SecurityReceive(0x00, 0x0000, buffer, sizeof(buffer)));
    EXPECT_TRUE(device.KeepAlive());
    EXPECT_TRUE(device.Abort(0, 0));
}

TEST(NVMeDeviceTest, QueueOpsSucceedWhenOpen) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.CreateIOCompletionQueue(1, 64, 0x1000, 0, true, true));
    EXPECT_TRUE(device.CreateIOSubmissionQueue(1, 64, 0x2000, 1, nvme::QueuePriority::Medium, true));
    EXPECT_TRUE(device.DeleteIOSubmissionQueue(1));
    EXPECT_TRUE(device.DeleteIOCompletionQueue(1));
}

//=============================================================================
// Structure Size Tests
//=============================================================================

TEST(NVMeStructSizeTest, SubmissionQueueEntry) {
    EXPECT_EQ(sizeof(nvme::SubmissionQueueEntry), 64);
}

TEST(NVMeStructSizeTest, CompletionQueueEntry) {
    EXPECT_EQ(sizeof(nvme::CompletionQueueEntry), 16);
}

TEST(NVMeStructSizeTest, CommandDWordUnions) {
    EXPECT_EQ(sizeof(nvme::IdentifyCDW10), 4);
    EXPECT_EQ(sizeof(nvme::IdentifyCDW11), 4);
    EXPECT_EQ(sizeof(nvme::GetLogPageCDW10), 4);
    EXPECT_EQ(sizeof(nvme::SetFeaturesCDW10), 4);
    EXPECT_EQ(sizeof(nvme::FirmwareCommitCDW10), 4);
}
