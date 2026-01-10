#include <gtest/gtest.h>
#include "nvme.h"

//=============================================================================
// Firmware Image Download Tests
//=============================================================================

TEST(FirmwareTest, FirmwareImageDownloadFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;
    uint8_t buffer[512] = {0};

    EXPECT_FALSE(device.FirmwareImageDownload(buffer, sizeof(buffer), 0));
}

TEST(FirmwareTest, FirmwareImageDownloadSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[512] = {0};

    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 0));
}

TEST(FirmwareTest, FirmwareImageDownloadWithOffset) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[512] = {0};

    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 0));
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 512));
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 1024));
}

TEST(FirmwareTest, FirmwareImageDownloadLargeBuffer) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};

    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 0));
}

//=============================================================================
// Firmware Commit Tests
//=============================================================================

TEST(FirmwareTest, FirmwareCommitFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.FirmwareCommit(nvme::FirmwareCommitAction::ReplaceActivateOnReset, 1));
}

TEST(FirmwareTest, FirmwareCommitSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.FirmwareCommit(nvme::FirmwareCommitAction::ReplaceActivateOnReset, 1));
}

TEST(FirmwareTest, FirmwareCommitVariousActions) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.FirmwareCommit(nvme::FirmwareCommitAction::ReplaceNotActivate, 1));
    EXPECT_TRUE(device.FirmwareCommit(nvme::FirmwareCommitAction::ReplaceActivateOnReset, 2));
    EXPECT_TRUE(device.FirmwareCommit(nvme::FirmwareCommitAction::ActivateExistingOnReset, 1));
    EXPECT_TRUE(device.FirmwareCommit(nvme::FirmwareCommitAction::ReplaceActivateImmediate, 1));
}

TEST(FirmwareTest, FirmwareCommitVariousSlots) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.FirmwareCommit(nvme::FirmwareCommitAction::ReplaceActivateOnReset, 1));
    EXPECT_TRUE(device.FirmwareCommit(nvme::FirmwareCommitAction::ReplaceActivateOnReset, 2));
    EXPECT_TRUE(device.FirmwareCommit(nvme::FirmwareCommitAction::ReplaceActivateOnReset, 3));
    EXPECT_TRUE(device.FirmwareCommit(nvme::FirmwareCommitAction::ReplaceActivateOnReset, 7));
}
