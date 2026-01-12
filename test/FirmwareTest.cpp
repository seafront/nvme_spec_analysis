#include <gtest/gtest.h>
#include "nvme.h"

//=============================================================================
// Firmware Image Download - Structure Validation Tests
//=============================================================================

// TC-FID-001: CDW10 구조체 크기 검증
TEST(FirmwareImageDownloadStructureTest, TC_FID_001_CDW10_SizeVerification) {
    EXPECT_EQ(sizeof(nvme::FirmwareImageDownloadCDW10), 4u);
    static_assert(sizeof(nvme::FirmwareImageDownloadCDW10) == 4, "CDW10 must be 4 bytes");
}

// TC-FID-002: CDW11 구조체 크기 검증
TEST(FirmwareImageDownloadStructureTest, TC_FID_002_CDW11_SizeVerification) {
    EXPECT_EQ(sizeof(nvme::FirmwareImageDownloadCDW11), 4u);
    static_assert(sizeof(nvme::FirmwareImageDownloadCDW11) == 4, "CDW11 must be 4 bytes");
}

// TC-FID-003: CDW10 raw와 bits 필드 동기화
TEST(FirmwareImageDownloadStructureTest, TC_FID_003_CDW10_RawToBits_Sync) {
    nvme::FirmwareImageDownloadCDW10 cdw10{};
    cdw10.raw = 0x12345678;

    EXPECT_EQ(cdw10.bits.NUMD, 0x12345678u);
}

// TC-FID-004: CDW10 bits와 raw 필드 역방향 동기화
TEST(FirmwareImageDownloadStructureTest, TC_FID_004_CDW10_BitsToRaw_Sync) {
    nvme::FirmwareImageDownloadCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.NUMD = 0xABCDEF00;

    EXPECT_EQ(cdw10.raw, 0xABCDEF00u);
}

// TC-FID-005: CDW11 raw와 bits 필드 동기화
TEST(FirmwareImageDownloadStructureTest, TC_FID_005_CDW11_RawToBits_Sync) {
    nvme::FirmwareImageDownloadCDW11 cdw11{};
    cdw11.raw = 0xABCDEF00;

    EXPECT_EQ(cdw11.bits.OFST, 0xABCDEF00u);
}

// TC-FID-006: CDW11 bits와 raw 필드 역방향 동기화
TEST(FirmwareImageDownloadStructureTest, TC_FID_006_CDW11_BitsToRaw_Sync) {
    nvme::FirmwareImageDownloadCDW11 cdw11{};
    cdw11.raw = 0;
    cdw11.bits.OFST = 0x87654321;

    EXPECT_EQ(cdw11.raw, 0x87654321u);
}

// TC-FID-007: Status Code Enum 값 검증
TEST(FirmwareImageDownloadStructureTest, TC_FID_007_StatusCode_EnumValues) {
    EXPECT_EQ(static_cast<uint8_t>(nvme::FirmwareImageDownloadStatusCode::OverlappingRange), 0x14);
}

// TC-FID-008: Opcode 값 검증
TEST(FirmwareImageDownloadStructureTest, TC_FID_008_Opcode_Verification) {
    EXPECT_EQ(static_cast<uint8_t>(nvme::AdminOpcode::FirmwareImageDownload), 0x11);
}

//=============================================================================
// Firmware Image Download - Basic Command Tests
//=============================================================================

// TC-FID-009: Data Pointer 설정 (기존 테스트 개선)
TEST(FirmwareImageDownloadBasicTest, TC_FID_009_DataPointer_Setting) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};

    // Verify data pointer is used correctly (implicit test through successful execution)
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 0));
}

// TC-FID-010: NUMD 필드 설정 - 기본값 (4KB = 1024 dwords = NUMD 1023)
TEST(FirmwareImageDownloadBasicTest, TC_FID_010_NUMD_Field_BasicValue) {
    nvme::FirmwareImageDownloadCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.NUMD = 1023;

    // 0's based: NUMD=1023 means 1024 dwords = 4096 bytes
    EXPECT_EQ(cdw10.bits.NUMD + 1, 1024u);
    EXPECT_EQ((cdw10.bits.NUMD + 1) * 4, 4096u);
}

// TC-FID-011: OFST 필드 설정 - 기본값 (1024 dwords = 4KB offset)
TEST(FirmwareImageDownloadBasicTest, TC_FID_011_OFST_Field_BasicValue) {
    nvme::FirmwareImageDownloadCDW11 cdw11{};
    cdw11.raw = 0;
    cdw11.bits.OFST = 1024;

    // OFST in dwords: 1024 dwords = 4096 bytes
    EXPECT_EQ(cdw11.bits.OFST * 4, 4096u);
}

// TC-FID-012: 첫 번째 조각 OFST=0 설정
TEST(FirmwareImageDownloadBasicTest, TC_FID_012_FirstFragment_OFST_Zero) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};

    // First fragment should start at offset 0
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 0));
}

//=============================================================================
// Firmware Image Download - Boundary Value Tests
//=============================================================================

// TC-FID-013: NUMD 최소값 (0) - 1 dword 전송
TEST(FirmwareImageDownloadBoundaryTest, TC_FID_013_NUMD_MinValue) {
    nvme::FirmwareImageDownloadCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.NUMD = 0;

    // 0's based: NUMD=0 means 1 dword
    EXPECT_EQ(cdw10.bits.NUMD + 1, 1u);
}

// TC-FID-014: NUMD 최대값 (0xFFFFFFFF)
TEST(FirmwareImageDownloadBoundaryTest, TC_FID_014_NUMD_MaxValue) {
    nvme::FirmwareImageDownloadCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.NUMD = 0xFFFFFFFF;

    EXPECT_EQ(cdw10.bits.NUMD, 0xFFFFFFFFu);
}

// TC-FID-015: OFST 최소값 (0)
TEST(FirmwareImageDownloadBoundaryTest, TC_FID_015_OFST_MinValue) {
    nvme::FirmwareImageDownloadCDW11 cdw11{};
    cdw11.raw = 0;
    cdw11.bits.OFST = 0;

    EXPECT_EQ(cdw11.bits.OFST, 0u);
}

// TC-FID-016: OFST 최대값 (0xFFFFFFFF)
TEST(FirmwareImageDownloadBoundaryTest, TC_FID_016_OFST_MaxValue) {
    nvme::FirmwareImageDownloadCDW11 cdw11{};
    cdw11.raw = 0;
    cdw11.bits.OFST = 0xFFFFFFFF;

    EXPECT_EQ(cdw11.bits.OFST, 0xFFFFFFFFu);
}

//=============================================================================
// Firmware Image Download - FWUG Related Tests
//=============================================================================

// TC-FID-017: NUMD가 FWUG 요구사항 충족 - 성공 (4KB aligned)
TEST(FirmwareImageDownloadFWUGTest, TC_FID_017_NUMD_FWUG_Requirement_Met) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};  // 4KB = 1024 dwords

    // NUMD = 1023 (0's based, means 1024 dwords = 4KB)
    // Assuming FWUG = 4KB (common value)
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 0));
}

// TC-FID-021: OFST가 FWUG alignment 충족 - 성공
TEST(FirmwareImageDownloadFWUGTest, TC_FID_021_OFST_FWUG_Alignment_Met) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};

    // OFST = 1024 dwords (4KB aligned)
    uint32_t offset_dwords = 1024;
    uint32_t offset_bytes = offset_dwords * 4;

    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), offset_bytes));
}

// TC-FID-020: NUMD가 FWUG 배수 - 성공 (8KB = 2 * 4KB)
TEST(FirmwareImageDownloadFWUGTest, TC_FID_020_NUMD_FWUG_Multiple) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[8192] = {0};  // 8KB = 2048 dwords

    // NUMD = 2047 (0's based, means 2048 dwords = 8KB)
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 0));
}

// TC-FID-023: OFST alignment 경계값 - 성공 (8KB aligned)
TEST(FirmwareImageDownloadFWUGTest, TC_FID_023_OFST_Alignment_Boundary) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};

    // OFST = 2048 dwords (8KB aligned)
    uint32_t offset_dwords = 2048;
    uint32_t offset_bytes = offset_dwords * 4;

    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), offset_bytes));
}

//=============================================================================
// Firmware Image Download - Overlapping Range Tests
//=============================================================================

// TC-FID-024: 겹치지 않는 범위 - 성공
TEST(FirmwareImageDownloadOverlapTest, TC_FID_024_NonOverlapping_Range_Success) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer1[4096] = {0};
    uint8_t buffer2[4096] = {0};

    // First fragment: offset 0, size 4KB
    EXPECT_TRUE(device.FirmwareImageDownload(buffer1, sizeof(buffer1), 0));

    // Second fragment: offset 4KB (1024 dwords * 4), size 4KB (non-overlapping)
    EXPECT_TRUE(device.FirmwareImageDownload(buffer2, sizeof(buffer2), 4096));
}

//=============================================================================
// Firmware Image Download - Multi-Fragment Tests
//=============================================================================

// TC-FID-027: 4개 조각 순차 다운로드
TEST(FirmwareImageDownloadMultiFragmentTest, TC_FID_027_FourFragments_Sequential) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};  // 4KB per fragment

    // Fragment 1: OFST = 0
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 0));

    // Fragment 2: OFST = 1024 dwords (4KB)
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 4096));

    // Fragment 3: OFST = 2048 dwords (8KB)
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 8192));

    // Fragment 4: OFST = 3072 dwords (12KB)
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 12288));
}

// TC-FID-028: 비순차적 조각 다운로드
TEST(FirmwareImageDownloadMultiFragmentTest, TC_FID_028_FourFragments_NonSequential) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[4096] = {0};

    // Download in order: 3, 1, 4, 2
    // Fragment 3: OFST = 2048 dwords (8KB)
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 8192));

    // Fragment 1: OFST = 0
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 0));

    // Fragment 4: OFST = 3072 dwords (12KB)
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 12288));

    // Fragment 2: OFST = 1024 dwords (4KB)
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 4096));
}

//=============================================================================
// Firmware Image Download - Error Handling Tests
//=============================================================================

// TC-FID-041: 무효한 Data Pointer (nullptr)
TEST(FirmwareImageDownloadErrorTest, TC_FID_041_InvalidDataPointer_Nullptr) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Passing nullptr should fail
    EXPECT_FALSE(device.FirmwareImageDownload(nullptr, 4096, 0));
}

//=============================================================================
// Firmware Image Download - Parameter Combination Tests
//=============================================================================

// TC-FID-044: 작은 크기 조각 다운로드 (1KB)
TEST(FirmwareImageDownloadParameterTest, TC_FID_044_SmallFragment_1KB) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[1024] = {0};  // 1KB = 256 dwords

    // NUMD = 255 (0's based, means 256 dwords = 1KB)
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 0));
}

// TC-FID-045: 큰 크기 조각 다운로드 (32KB)
TEST(FirmwareImageDownloadParameterTest, TC_FID_045_LargeFragment_32KB) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[32768] = {0};  // 32KB = 8192 dwords

    // NUMD = 8191 (0's based, means 8192 dwords = 32KB)
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 0));
}

//=============================================================================
// Firmware Image Download - Device Not Open Tests
//=============================================================================

TEST(FirmwareImageDownloadDeviceTest, FailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;
    uint8_t buffer[512] = {0};

    EXPECT_FALSE(device.FirmwareImageDownload(buffer, sizeof(buffer), 0));
}

TEST(FirmwareImageDownloadDeviceTest, SucceedsWhenDeviceOpen) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[512] = {0};

    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 0));
}

TEST(FirmwareImageDownloadDeviceTest, WithVariousOffsets) {
    nvme::NVMeDevice device("/dev/nvme0");
    uint8_t buffer[512] = {0};

    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 0));
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 512));
    EXPECT_TRUE(device.FirmwareImageDownload(buffer, sizeof(buffer), 1024));
}

TEST(FirmwareImageDownloadDeviceTest, LargeBuffer) {
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
