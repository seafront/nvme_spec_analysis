#include <gtest/gtest.h>
#include "nvme.h"

//=============================================================================
// Virtualization Management Tests
//=============================================================================

TEST(VirtualizationManagementTest, FailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.VirtualizationManagement(
        nvme::VirtualizationAction::PrimaryControllerFlexibleAllocation,
        0, nvme::VirtualizationResourceType::VQResources, 0));
}

TEST(VirtualizationManagementTest, PrimaryControllerFlexibleAllocationSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.VirtualizationManagement(
        nvme::VirtualizationAction::PrimaryControllerFlexibleAllocation,
        0, nvme::VirtualizationResourceType::VQResources, 16));
}

TEST(VirtualizationManagementTest, SecondaryControllerOfflineSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.VirtualizationManagement(
        nvme::VirtualizationAction::SecondaryControllerOffline,
        1, nvme::VirtualizationResourceType::VQResources, 0));
}

TEST(VirtualizationManagementTest, SecondaryControllerAssignSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.VirtualizationManagement(
        nvme::VirtualizationAction::SecondaryControllerAssign,
        1, nvme::VirtualizationResourceType::VQResources, 8));
}

TEST(VirtualizationManagementTest, SecondaryControllerOnlineSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.VirtualizationManagement(
        nvme::VirtualizationAction::SecondaryControllerOnline,
        1, nvme::VirtualizationResourceType::VQResources, 0));
}

TEST(VirtualizationManagementTest, DifferentResourceTypes) {
    nvme::NVMeDevice device("/dev/nvme0");

    // VQ Resources
    EXPECT_TRUE(device.VirtualizationManagement(
        nvme::VirtualizationAction::PrimaryControllerFlexibleAllocation,
        0, nvme::VirtualizationResourceType::VQResources, 8));

    // VI Resources
    EXPECT_TRUE(device.VirtualizationManagement(
        nvme::VirtualizationAction::PrimaryControllerFlexibleAllocation,
        0, nvme::VirtualizationResourceType::VIResources, 8));
}

TEST(VirtualizationManagementTest, VariousControllerIds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.VirtualizationManagement(
        nvme::VirtualizationAction::SecondaryControllerOffline,
        0x0000, nvme::VirtualizationResourceType::VQResources, 0));

    EXPECT_TRUE(device.VirtualizationManagement(
        nvme::VirtualizationAction::SecondaryControllerOffline,
        0x0001, nvme::VirtualizationResourceType::VQResources, 0));

    EXPECT_TRUE(device.VirtualizationManagement(
        nvme::VirtualizationAction::SecondaryControllerOffline,
        0xFFFF, nvme::VirtualizationResourceType::VQResources, 0));
}

TEST(VirtualizationManagementTest, VariousResourceCounts) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.VirtualizationManagement(
        nvme::VirtualizationAction::PrimaryControllerFlexibleAllocation,
        0, nvme::VirtualizationResourceType::VQResources, 0));

    EXPECT_TRUE(device.VirtualizationManagement(
        nvme::VirtualizationAction::PrimaryControllerFlexibleAllocation,
        0, nvme::VirtualizationResourceType::VQResources, 1));

    EXPECT_TRUE(device.VirtualizationManagement(
        nvme::VirtualizationAction::PrimaryControllerFlexibleAllocation,
        0, nvme::VirtualizationResourceType::VQResources, 0xFFFF));
}

TEST(VirtualizationManagementTest, DefaultParameters) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Using default resource type and count
    EXPECT_TRUE(device.VirtualizationManagement(
        nvme::VirtualizationAction::SecondaryControllerOffline, 1));
}
