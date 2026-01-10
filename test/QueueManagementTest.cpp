#include <gtest/gtest.h>
#include "nvme.h"

//=============================================================================
// Create I/O Completion Queue Tests
//=============================================================================

TEST(QueueManagementTest, CreateIOCQFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.CreateIOCompletionQueue(1, 64, 0x1000, 0, true, true));
}

TEST(QueueManagementTest, CreateIOCQSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.CreateIOCompletionQueue(1, 64, 0x1000, 0, true, true));
}

TEST(QueueManagementTest, CreateIOCQVariousParameters) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Different queue IDs
    EXPECT_TRUE(device.CreateIOCompletionQueue(1, 64, 0x1000, 0, true, true));
    EXPECT_TRUE(device.CreateIOCompletionQueue(2, 128, 0x2000, 1, true, true));
    EXPECT_TRUE(device.CreateIOCompletionQueue(3, 256, 0x3000, 2, false, true));
}

TEST(QueueManagementTest, CreateIOCQInterruptSettings) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Interrupts enabled
    EXPECT_TRUE(device.CreateIOCompletionQueue(1, 64, 0x1000, 0, true, true));
    // Interrupts disabled
    EXPECT_TRUE(device.CreateIOCompletionQueue(2, 64, 0x2000, 0, false, true));
}

//=============================================================================
// Create I/O Submission Queue Tests
//=============================================================================

TEST(QueueManagementTest, CreateIOSQFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.CreateIOSubmissionQueue(1, 64, 0x2000, 1, nvme::QueuePriority::Medium, true));
}

TEST(QueueManagementTest, CreateIOSQSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.CreateIOSubmissionQueue(1, 64, 0x2000, 1, nvme::QueuePriority::Medium, true));
}

TEST(QueueManagementTest, CreateIOSQVariousPriorities) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.CreateIOSubmissionQueue(1, 64, 0x2000, 1, nvme::QueuePriority::Urgent, true));
    EXPECT_TRUE(device.CreateIOSubmissionQueue(2, 64, 0x3000, 1, nvme::QueuePriority::High, true));
    EXPECT_TRUE(device.CreateIOSubmissionQueue(3, 64, 0x4000, 1, nvme::QueuePriority::Medium, true));
    EXPECT_TRUE(device.CreateIOSubmissionQueue(4, 64, 0x5000, 1, nvme::QueuePriority::Low, true));
}

//=============================================================================
// Delete I/O Completion Queue Tests
//=============================================================================

TEST(QueueManagementTest, DeleteIOCQFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.DeleteIOCompletionQueue(1));
}

TEST(QueueManagementTest, DeleteIOCQSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.DeleteIOCompletionQueue(1));
}

TEST(QueueManagementTest, DeleteIOCQVariousQIDs) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.DeleteIOCompletionQueue(1));
    EXPECT_TRUE(device.DeleteIOCompletionQueue(2));
    EXPECT_TRUE(device.DeleteIOCompletionQueue(65535));
}

//=============================================================================
// Delete I/O Submission Queue Tests
//=============================================================================

TEST(QueueManagementTest, DeleteIOSQFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.DeleteIOSubmissionQueue(1));
}

TEST(QueueManagementTest, DeleteIOSQSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.DeleteIOSubmissionQueue(1));
}

TEST(QueueManagementTest, DeleteIOSQVariousQIDs) {
    nvme::NVMeDevice device("/dev/nvme0");

    EXPECT_TRUE(device.DeleteIOSubmissionQueue(1));
    EXPECT_TRUE(device.DeleteIOSubmissionQueue(2));
    EXPECT_TRUE(device.DeleteIOSubmissionQueue(65535));
}

//=============================================================================
// Queue Lifecycle Tests
//=============================================================================

TEST(QueueManagementTest, CreateAndDeleteQueues) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Create CQ first, then SQ
    EXPECT_TRUE(device.CreateIOCompletionQueue(1, 64, 0x1000, 0, true, true));
    EXPECT_TRUE(device.CreateIOSubmissionQueue(1, 64, 0x2000, 1, nvme::QueuePriority::Medium, true));

    // Delete SQ first, then CQ
    EXPECT_TRUE(device.DeleteIOSubmissionQueue(1));
    EXPECT_TRUE(device.DeleteIOCompletionQueue(1));
}
