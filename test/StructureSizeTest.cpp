#include <gtest/gtest.h>
#include "nvme.h"

//=============================================================================
// Queue Entry Structure Size Tests
//=============================================================================

TEST(StructureSizeTest, SubmissionQueueEntry) {
    EXPECT_EQ(sizeof(nvme::SubmissionQueueEntry), 64);
}

TEST(StructureSizeTest, CompletionQueueEntry) {
    EXPECT_EQ(sizeof(nvme::CompletionQueueEntry), 16);
}

//=============================================================================
// Command DWord Union Size Tests
//=============================================================================

TEST(StructureSizeTest, IdentifyCDWs) {
    EXPECT_EQ(sizeof(nvme::IdentifyCDW10), 4);
    EXPECT_EQ(sizeof(nvme::IdentifyCDW11), 4);
}

TEST(StructureSizeTest, GetLogPageCDWs) {
    EXPECT_EQ(sizeof(nvme::GetLogPageCDW10), 4);
    EXPECT_EQ(sizeof(nvme::GetLogPageCDW11), 4);
    EXPECT_EQ(sizeof(nvme::GetLogPageCDW12), 4);
    EXPECT_EQ(sizeof(nvme::GetLogPageCDW13), 4);
}

TEST(StructureSizeTest, FeaturesCDWs) {
    EXPECT_EQ(sizeof(nvme::GetFeaturesCDW10), 4);
    EXPECT_EQ(sizeof(nvme::SetFeaturesCDW10), 4);
}

TEST(StructureSizeTest, FirmwareCDWs) {
    EXPECT_EQ(sizeof(nvme::FirmwareCommitCDW10), 4);
    EXPECT_EQ(sizeof(nvme::FirmwareImageDownloadCDW10), 4);
    EXPECT_EQ(sizeof(nvme::FirmwareImageDownloadCDW11), 4);
}

TEST(StructureSizeTest, NamespaceManagementCDWs) {
    EXPECT_EQ(sizeof(nvme::NamespaceManagementCDW10), 4);
    EXPECT_EQ(sizeof(nvme::NamespaceAttachmentCDW10), 4);
}

TEST(StructureSizeTest, FormatNVMCDWs) {
    EXPECT_EQ(sizeof(nvme::FormatNVMCDW10), 4);
}

TEST(StructureSizeTest, SanitizeCDWs) {
    EXPECT_EQ(sizeof(nvme::SanitizeCDW10), 4);
    EXPECT_EQ(sizeof(nvme::SanitizeCDW11), 4);
}

TEST(StructureSizeTest, SecurityCDWs) {
    EXPECT_EQ(sizeof(nvme::SecuritySendCDW10), 4);
    EXPECT_EQ(sizeof(nvme::SecuritySendCDW11), 4);
    EXPECT_EQ(sizeof(nvme::SecurityReceiveCDW10), 4);
    EXPECT_EQ(sizeof(nvme::SecurityReceiveCDW11), 4);
}

TEST(StructureSizeTest, QueueManagementCDWs) {
    EXPECT_EQ(sizeof(nvme::CreateIOCompletionQueueCDW10), 4);
    EXPECT_EQ(sizeof(nvme::CreateIOCompletionQueueCDW11), 4);
    EXPECT_EQ(sizeof(nvme::CreateIOSubmissionQueueCDW10), 4);
    EXPECT_EQ(sizeof(nvme::CreateIOSubmissionQueueCDW11), 4);
    EXPECT_EQ(sizeof(nvme::DeleteIOCompletionQueueCDW10), 4);
    EXPECT_EQ(sizeof(nvme::DeleteIOSubmissionQueueCDW10), 4);
}

TEST(StructureSizeTest, DeviceSelfTestCDWs) {
    EXPECT_EQ(sizeof(nvme::DeviceSelfTestCDW10), 4);
}

TEST(StructureSizeTest, AbortCDWs) {
    EXPECT_EQ(sizeof(nvme::AbortCDW10), 4);
}
