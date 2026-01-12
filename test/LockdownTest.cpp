#include <gtest/gtest.h>
#include "nvme.h"

//=============================================================================
// Lockdown Tests
//=============================================================================

TEST(LockdownTest, LockdownFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.Lockdown(nvme::LockdownScope::AdminCommandOpcode, 0x00, true));
}

TEST(LockdownTest, LockdownProhibitAdminCommandSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Prohibit an admin command opcode
    EXPECT_TRUE(device.Lockdown(nvme::LockdownScope::AdminCommandOpcode, 0x06, true));
}

TEST(LockdownTest, LockdownAllowAdminCommandSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Allow an admin command opcode
    EXPECT_TRUE(device.Lockdown(nvme::LockdownScope::AdminCommandOpcode, 0x06, false));
}

TEST(LockdownTest, LockdownProhibitSetFeaturesSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Prohibit a Set Features Feature Identifier
    EXPECT_TRUE(device.Lockdown(nvme::LockdownScope::SetFeaturesFeatureId, 0x01, true));
}

TEST(LockdownTest, LockdownWithDifferentInterfaces) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Admin Submission Queue only
    EXPECT_TRUE(device.Lockdown(nvme::LockdownScope::AdminCommandOpcode, 0x06, true,
                                nvme::LockdownInterface::AdminSubmissionQueue));

    // Admin SQ and Management Endpoint
    EXPECT_TRUE(device.Lockdown(nvme::LockdownScope::AdminCommandOpcode, 0x06, true,
                                nvme::LockdownInterface::AdminSQAndManagementEndpoint));

    // Management Endpoint only
    EXPECT_TRUE(device.Lockdown(nvme::LockdownScope::AdminCommandOpcode, 0x06, true,
                                nvme::LockdownInterface::ManagementEndpoint));
}

TEST(LockdownTest, LockdownWithUUIDIndex) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Lockdown with UUID Index for vendor-specific Feature Identifier
    EXPECT_TRUE(device.Lockdown(nvme::LockdownScope::SetFeaturesFeatureId, 0xC0, true,
                                nvme::LockdownInterface::AdminSubmissionQueue, 0x01));
}

TEST(LockdownTest, LockdownVariousScopes) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Admin command opcode
    EXPECT_TRUE(device.Lockdown(nvme::LockdownScope::AdminCommandOpcode, 0x06, true));

    // Set Features Feature Identifier
    EXPECT_TRUE(device.Lockdown(nvme::LockdownScope::SetFeaturesFeatureId, 0x01, true));

    // Management Interface Command Set opcode
    EXPECT_TRUE(device.Lockdown(nvme::LockdownScope::ManagementInterfaceOpcode, 0x00, true));

    // PCIe Command Set opcode
    EXPECT_TRUE(device.Lockdown(nvme::LockdownScope::PCIeCommandSetOpcode, 0x00, true));
}

TEST(LockdownTest, LockdownDefaultParameters) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Using default interface and UUID index
    EXPECT_TRUE(device.Lockdown(nvme::LockdownScope::AdminCommandOpcode, 0x06, true));
}
