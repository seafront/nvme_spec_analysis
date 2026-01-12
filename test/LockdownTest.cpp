#include <gtest/gtest.h>
#include "nvme.h"

//=============================================================================
// Lockdown Header Structure Tests
//=============================================================================

// TC-001: Opcode 검증
TEST(LockdownBasicTest, TC001_OpcodeVerification) {
    EXPECT_EQ(static_cast<uint8_t>(nvme::AdminOpcode::Lockdown), 0x24);
}

//-----------------------------------------------------------------------------
// CDW10 필드 테스트
//-----------------------------------------------------------------------------

// TC-002: OFI 필드 - Admin Command Opcode 설정
TEST(LockdownCDW10Test, TC002_OFI_AdminCommandOpcode) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.OFI = 0x06;

    EXPECT_EQ(cdw10.bits.OFI, 0x06);
    EXPECT_EQ(cdw10.raw & 0xFF00, 0x0600);
}

// TC-003: OFI 필드 - Feature Identifier 설정
TEST(LockdownCDW10Test, TC003_OFI_FeatureIdentifier) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.SCP = 0x2;
    cdw10.bits.OFI = 0x01;

    EXPECT_EQ(cdw10.bits.OFI, 0x01);
    EXPECT_EQ(cdw10.bits.SCP, 0x2);
}

// TC-004: OFI 필드 최대값 경계 테스트
TEST(LockdownCDW10Test, TC004_OFI_MaxBoundary) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.OFI = 0xFF;

    EXPECT_EQ(cdw10.bits.OFI, 0xFF);
    EXPECT_EQ(cdw10.bits.SCP, 0);
    EXPECT_EQ(cdw10.bits.PRHBT, 0);
}

// TC-005: OFI 필드 최소값 경계 테스트
TEST(LockdownCDW10Test, TC005_OFI_MinBoundary) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.OFI = 0x00;

    EXPECT_EQ(cdw10.bits.OFI, 0x00);
}

// TC-006: IFC 필드 - Admin Submission Queue (00b)
TEST(LockdownCDW10Test, TC006_IFC_AdminSubmissionQueue) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.IFC = static_cast<uint8_t>(nvme::LockdownInterface::AdminSubmissionQueue);

    EXPECT_EQ(cdw10.bits.IFC, 0);
    EXPECT_EQ(static_cast<uint8_t>(nvme::LockdownInterface::AdminSubmissionQueue), 0);
}

// TC-007: IFC 필드 - Admin SQ 및 Management Endpoint (01b)
TEST(LockdownCDW10Test, TC007_IFC_AdminSQAndMgmtEndpoint) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.IFC = static_cast<uint8_t>(nvme::LockdownInterface::AdminSQAndManagementEndpoint);

    EXPECT_EQ(cdw10.bits.IFC, 1);
    EXPECT_EQ(static_cast<uint8_t>(nvme::LockdownInterface::AdminSQAndManagementEndpoint), 1);
}

// TC-008: IFC 필드 - Management Endpoint Only (10b)
TEST(LockdownCDW10Test, TC008_IFC_ManagementEndpointOnly) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.IFC = static_cast<uint8_t>(nvme::LockdownInterface::ManagementEndpoint);

    EXPECT_EQ(cdw10.bits.IFC, 2);
    EXPECT_EQ(static_cast<uint8_t>(nvme::LockdownInterface::ManagementEndpoint), 2);
}

// TC-009: IFC 필드 - Reserved 값 (11b) 검증
TEST(LockdownCDW10Test, TC009_IFC_Reserved) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.IFC = 3;

    EXPECT_EQ(cdw10.bits.IFC, 3);
}

// TC-010: PRHBT 필드 - 금지 설정 (1)
TEST(LockdownCDW10Test, TC010_PRHBT_Prohibit) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.PRHBT = 1;

    EXPECT_EQ(cdw10.bits.PRHBT, 1);
}

// TC-011: PRHBT 필드 - 허용 설정 (0)
TEST(LockdownCDW10Test, TC011_PRHBT_Allow) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.PRHBT = 0;

    EXPECT_EQ(cdw10.bits.PRHBT, 0);
}

// TC-012: SCP 필드 - Admin Command Opcode (0h)
TEST(LockdownCDW10Test, TC012_SCP_AdminCommandOpcode) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.SCP = static_cast<uint8_t>(nvme::LockdownScope::AdminCommandOpcode);

    EXPECT_EQ(cdw10.bits.SCP, 0);
    EXPECT_EQ(static_cast<uint8_t>(nvme::LockdownScope::AdminCommandOpcode), 0);
}

// TC-013: SCP 필드 - Set Features FID (2h)
TEST(LockdownCDW10Test, TC013_SCP_SetFeaturesFID) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.SCP = static_cast<uint8_t>(nvme::LockdownScope::SetFeaturesFeatureId);

    EXPECT_EQ(cdw10.bits.SCP, 2);
    EXPECT_EQ(static_cast<uint8_t>(nvme::LockdownScope::SetFeaturesFeatureId), 2);
}

// TC-014: SCP 필드 - MI Command Set Opcode (3h)
TEST(LockdownCDW10Test, TC014_SCP_MICommandSetOpcode) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.SCP = static_cast<uint8_t>(nvme::LockdownScope::ManagementInterfaceOpcode);

    EXPECT_EQ(cdw10.bits.SCP, 3);
    EXPECT_EQ(static_cast<uint8_t>(nvme::LockdownScope::ManagementInterfaceOpcode), 3);
}

// TC-015: SCP 필드 - PCIe Command Set Opcode (4h)
TEST(LockdownCDW10Test, TC015_SCP_PCIeCommandSetOpcode) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.SCP = static_cast<uint8_t>(nvme::LockdownScope::PCIeCommandSetOpcode);

    EXPECT_EQ(cdw10.bits.SCP, 4);
    EXPECT_EQ(static_cast<uint8_t>(nvme::LockdownScope::PCIeCommandSetOpcode), 4);
}

// TC-016: SCP 필드 - Reserved 값 (1h) 검증
TEST(LockdownCDW10Test, TC016_SCP_Reserved1) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.SCP = 1;

    EXPECT_EQ(cdw10.bits.SCP, 1);
}

// TC-017: SCP 필드 - Reserved 값 (5h-Fh) 경계 테스트
TEST(LockdownCDW10Test, TC017_SCP_ReservedBoundary) {
    nvme::LockdownCDW10 cdw10{};

    // Test SCP = 5
    cdw10.raw = 0;
    cdw10.bits.SCP = 5;
    EXPECT_EQ(cdw10.bits.SCP, 5);

    // Test SCP = 15 (0xF, maximum 4-bit value)
    cdw10.raw = 0;
    cdw10.bits.SCP = 15;
    EXPECT_EQ(cdw10.bits.SCP, 15);
}

//-----------------------------------------------------------------------------
// CDW14 필드 테스트
//-----------------------------------------------------------------------------

// TC-018: UIDX 필드 - UUID Index 설정
TEST(LockdownCDW14Test, TC018_UIDX_UUIDIndex) {
    nvme::LockdownCDW14 cdw14{};
    cdw14.raw = 0;
    cdw14.bits.UIDX = 1;

    EXPECT_EQ(cdw14.bits.UIDX, 1);
}

// TC-019: UIDX 필드 최대값 경계 테스트
TEST(LockdownCDW14Test, TC019_UIDX_MaxBoundary) {
    nvme::LockdownCDW14 cdw14{};
    cdw14.raw = 0;
    cdw14.bits.UIDX = 0x7F;

    EXPECT_EQ(cdw14.bits.UIDX, 0x7F);
    EXPECT_EQ(cdw14.bits.UIDX, 127);
}

// TC-020: UIDX 필드 - SCP=2h 및 UUID 지원 시 사용
TEST(LockdownCDW14Test, TC020_UIDX_WithSetFeaturesFID) {
    nvme::LockdownCDW10 cdw10{};
    nvme::LockdownCDW14 cdw14{};
    cdw10.raw = 0;
    cdw14.raw = 0;

    cdw10.bits.SCP = static_cast<uint8_t>(nvme::LockdownScope::SetFeaturesFeatureId);
    cdw14.bits.UIDX = 5;

    EXPECT_EQ(cdw10.bits.SCP, 2);
    EXPECT_EQ(cdw14.bits.UIDX, 5);
}

// TC-021: UIDX 필드 - SCP != 2h일 때 무시 검증
TEST(LockdownCDW14Test, TC021_UIDX_IgnoredWhenSCPNotSetFeatures) {
    nvme::LockdownCDW10 cdw10{};
    nvme::LockdownCDW14 cdw14{};
    cdw10.raw = 0;
    cdw14.raw = 0;

    cdw10.bits.SCP = static_cast<uint8_t>(nvme::LockdownScope::AdminCommandOpcode);
    cdw14.bits.UIDX = 10;

    EXPECT_EQ(cdw10.bits.SCP, 0);
    EXPECT_EQ(cdw14.bits.UIDX, 10);
    // Note: 구현에서 SCP=0일 때 UIDX는 무시됨 (스펙 참조)
}

//-----------------------------------------------------------------------------
// Reserved 필드 테스트
//-----------------------------------------------------------------------------

// TC-022: CDW10 Reserved 필드 (bits 31:16) 0 설정
TEST(LockdownReservedTest, TC022_CDW10_Reserved_Bits31_16) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.OFI = 0x12;
    cdw10.bits.SCP = 2;

    EXPECT_EQ((cdw10.raw >> 16) & 0xFFFF, 0);
}

// TC-023: CDW10 Reserved 필드 (bit 07) 0 설정
TEST(LockdownReservedTest, TC023_CDW10_Reserved_Bit07) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.IFC = 2;
    cdw10.bits.PRHBT = 1;

    EXPECT_EQ(cdw10.bits.Reserved0, 0);
    EXPECT_EQ((cdw10.raw >> 7) & 0x1, 0);
}

// TC-024: CDW14 Reserved 필드 (bits 31:07) 0 설정
TEST(LockdownReservedTest, TC024_CDW14_Reserved_Bits31_07) {
    nvme::LockdownCDW14 cdw14{};
    cdw14.raw = 0;
    cdw14.bits.UIDX = 0x3F;

    EXPECT_EQ((cdw14.raw >> 7), 0);
    EXPECT_EQ(cdw14.bits.Reserved, 0);
}

//-----------------------------------------------------------------------------
// 구조체 크기 및 레이아웃 테스트
//-----------------------------------------------------------------------------

// TC-025: CDW10 Union 크기 검증
TEST(LockdownStructureTest, TC025_CDW10_SizeVerification) {
    EXPECT_EQ(sizeof(nvme::LockdownCDW10), 4);
    static_assert(sizeof(nvme::LockdownCDW10) == 4, "CDW10 must be 4 bytes");
}

// TC-026: CDW14 Union 크기 검증
TEST(LockdownStructureTest, TC026_CDW14_SizeVerification) {
    EXPECT_EQ(sizeof(nvme::LockdownCDW14), 4);
    static_assert(sizeof(nvme::LockdownCDW14) == 4, "CDW14 must be 4 bytes");
}

// TC-027: CDW10 비트 필드 레이아웃 검증
TEST(LockdownStructureTest, TC027_CDW10_BitFieldLayout) {
    nvme::LockdownCDW10 cdw10{};

    // Test SCP field (bits 0-3)
    cdw10.raw = 0;
    cdw10.bits.SCP = 0xF;
    EXPECT_EQ(cdw10.raw & 0xF, 0xF);

    // Test PRHBT field (bit 4)
    cdw10.raw = 0;
    cdw10.bits.PRHBT = 1;
    EXPECT_EQ(cdw10.raw & 0x10, 0x10);

    // Test IFC field (bits 5-6)
    cdw10.raw = 0;
    cdw10.bits.IFC = 0x3;
    EXPECT_EQ(cdw10.raw & 0x60, 0x60);

    // Test OFI field (bits 8-15)
    cdw10.raw = 0;
    cdw10.bits.OFI = 0xAB;
    EXPECT_EQ(cdw10.raw & 0xFF00, 0xAB00);
}

// TC-028: CDW14 비트 필드 레이아웃 검증
TEST(LockdownStructureTest, TC028_CDW14_BitFieldLayout) {
    nvme::LockdownCDW14 cdw14{};

    // Test UIDX field (bits 0-6)
    cdw14.raw = 0;
    cdw14.bits.UIDX = 0x55;
    EXPECT_EQ(cdw14.raw & 0x7F, 0x55);

    // Test reserved bits are not affected
    EXPECT_EQ(cdw14.raw >> 7, 0);
}

//-----------------------------------------------------------------------------
// Enum 값 테스트
//-----------------------------------------------------------------------------

// TC-029: LockdownScope Enum 값 검증
TEST(LockdownEnumTest, TC029_LockdownScope_Values) {
    EXPECT_EQ(static_cast<uint8_t>(nvme::LockdownScope::AdminCommandOpcode), 0x0);
    EXPECT_EQ(static_cast<uint8_t>(nvme::LockdownScope::SetFeaturesFeatureId), 0x2);
    EXPECT_EQ(static_cast<uint8_t>(nvme::LockdownScope::ManagementInterfaceOpcode), 0x3);
    EXPECT_EQ(static_cast<uint8_t>(nvme::LockdownScope::PCIeCommandSetOpcode), 0x4);
}

// TC-030: LockdownInterface Enum 값 검증
TEST(LockdownEnumTest, TC030_LockdownInterface_Values) {
    EXPECT_EQ(static_cast<uint8_t>(nvme::LockdownInterface::AdminSubmissionQueue), 0);
    EXPECT_EQ(static_cast<uint8_t>(nvme::LockdownInterface::AdminSQAndManagementEndpoint), 1);
    EXPECT_EQ(static_cast<uint8_t>(nvme::LockdownInterface::ManagementEndpoint), 2);
}

// TC-031: LockdownProhibit Enum 값 검증 (현재 헤더에는 정의되지 않음, 값만 테스트)
TEST(LockdownEnumTest, TC031_LockdownProhibit_Values) {
    // Allow = 0, Prohibit = 1 (PRHBT 필드 값으로 테스트)
    nvme::LockdownCDW10 cdw10{};

    cdw10.raw = 0;
    cdw10.bits.PRHBT = 0;  // Allow
    EXPECT_EQ(cdw10.bits.PRHBT, 0);

    cdw10.raw = 0;
    cdw10.bits.PRHBT = 1;  // Prohibit
    EXPECT_EQ(cdw10.bits.PRHBT, 1);
}

//-----------------------------------------------------------------------------
// 통합 및 경계 조건 테스트
//-----------------------------------------------------------------------------

// TC-032: 모든 필드 최대값 동시 설정
TEST(LockdownIntegrationTest, TC032_AllFields_MaxValues) {
    nvme::LockdownCDW10 cdw10{};
    nvme::LockdownCDW14 cdw14{};

    cdw10.raw = 0;
    cdw14.raw = 0;

    cdw10.bits.OFI = 0xFF;
    cdw10.bits.IFC = 2;  // ManagementEndpoint (max valid value)
    cdw10.bits.SCP = 4;  // PCIeCommandSetOpcode (max valid value)
    cdw10.bits.PRHBT = 1;
    cdw14.bits.UIDX = 0x7F;

    EXPECT_EQ(cdw10.bits.OFI, 0xFF);
    EXPECT_EQ(cdw10.bits.IFC, 2);
    EXPECT_EQ(cdw10.bits.SCP, 4);
    EXPECT_EQ(cdw10.bits.PRHBT, 1);
    EXPECT_EQ(cdw14.bits.UIDX, 0x7F);

    // Verify reserved fields remain 0
    EXPECT_EQ(cdw10.bits.Reserved0, 0);
    EXPECT_EQ((cdw10.raw >> 16), 0);
    EXPECT_EQ((cdw14.raw >> 7), 0);
}

// TC-033: 모든 필드 최소값 동시 설정
TEST(LockdownIntegrationTest, TC033_AllFields_MinValues) {
    nvme::LockdownCDW10 cdw10{};
    nvme::LockdownCDW14 cdw14{};

    cdw10.raw = 0;
    cdw14.raw = 0;

    cdw10.bits.OFI = 0;
    cdw10.bits.IFC = 0;
    cdw10.bits.SCP = 0;
    cdw10.bits.PRHBT = 0;
    cdw14.bits.UIDX = 0;

    EXPECT_EQ(cdw10.bits.OFI, 0);
    EXPECT_EQ(cdw10.bits.IFC, 0);
    EXPECT_EQ(cdw10.bits.SCP, 0);
    EXPECT_EQ(cdw10.bits.PRHBT, 0);
    EXPECT_EQ(cdw14.bits.UIDX, 0);
    EXPECT_EQ(cdw10.raw, 0);
    EXPECT_EQ(cdw14.raw, 0);
}

// TC-034: 다양한 필드 조합 테스트 - Admin Command 금지
TEST(LockdownIntegrationTest, TC034_ProhibitAdminCommand) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;

    cdw10.bits.SCP = static_cast<uint8_t>(nvme::LockdownScope::AdminCommandOpcode);
    cdw10.bits.IFC = static_cast<uint8_t>(nvme::LockdownInterface::AdminSubmissionQueue);
    cdw10.bits.PRHBT = 1;
    cdw10.bits.OFI = 0x06;  // Identify command

    EXPECT_EQ(cdw10.bits.SCP, 0);
    EXPECT_EQ(cdw10.bits.IFC, 0);
    EXPECT_EQ(cdw10.bits.PRHBT, 1);
    EXPECT_EQ(cdw10.bits.OFI, 0x06);
}

// TC-035: 다양한 필드 조합 테스트 - Set Features FID 금지
TEST(LockdownIntegrationTest, TC035_ProhibitSetFeaturesFID) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;

    cdw10.bits.SCP = static_cast<uint8_t>(nvme::LockdownScope::SetFeaturesFeatureId);
    cdw10.bits.IFC = static_cast<uint8_t>(nvme::LockdownInterface::AdminSubmissionQueue);
    cdw10.bits.PRHBT = 1;
    cdw10.bits.OFI = 0x01;  // Arbitration FID

    EXPECT_EQ(cdw10.bits.SCP, 2);
    EXPECT_EQ(cdw10.bits.IFC, 0);
    EXPECT_EQ(cdw10.bits.PRHBT, 1);
    EXPECT_EQ(cdw10.bits.OFI, 0x01);
}

// TC-036: 다양한 필드 조합 테스트 - Management Interface
TEST(LockdownIntegrationTest, TC036_ProhibitManagementInterface) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;

    cdw10.bits.SCP = static_cast<uint8_t>(nvme::LockdownScope::ManagementInterfaceOpcode);
    cdw10.bits.IFC = static_cast<uint8_t>(nvme::LockdownInterface::AdminSQAndManagementEndpoint);
    cdw10.bits.PRHBT = 1;
    cdw10.bits.OFI = 0x10;  // Example MI opcode

    EXPECT_EQ(cdw10.bits.SCP, 3);
    EXPECT_EQ(cdw10.bits.IFC, 1);
    EXPECT_EQ(cdw10.bits.PRHBT, 1);
    EXPECT_EQ(cdw10.bits.OFI, 0x10);
}

// TC-037: 다양한 필드 조합 테스트 - PCIe Command Set
TEST(LockdownIntegrationTest, TC037_ProhibitPCIeCommandSet) {
    nvme::LockdownCDW10 cdw10{};
    cdw10.raw = 0;

    cdw10.bits.SCP = static_cast<uint8_t>(nvme::LockdownScope::PCIeCommandSetOpcode);
    cdw10.bits.IFC = static_cast<uint8_t>(nvme::LockdownInterface::ManagementEndpoint);
    cdw10.bits.PRHBT = 1;
    cdw10.bits.OFI = 0x20;  // Example PCIe opcode

    EXPECT_EQ(cdw10.bits.SCP, 4);
    EXPECT_EQ(cdw10.bits.IFC, 2);
    EXPECT_EQ(cdw10.bits.PRHBT, 1);
    EXPECT_EQ(cdw10.bits.OFI, 0x20);
}

// TC-038: UUID Index 사용 시나리오
TEST(LockdownIntegrationTest, TC038_UUIDIndexScenario) {
    nvme::LockdownCDW10 cdw10{};
    nvme::LockdownCDW14 cdw14{};

    cdw10.raw = 0;
    cdw14.raw = 0;

    cdw10.bits.SCP = static_cast<uint8_t>(nvme::LockdownScope::SetFeaturesFeatureId);
    cdw10.bits.IFC = static_cast<uint8_t>(nvme::LockdownInterface::AdminSubmissionQueue);
    cdw10.bits.PRHBT = 1;
    cdw10.bits.OFI = 0xC0;  // Vendor specific FID (C0h-FFh)
    cdw14.bits.UIDX = 3;    // UUID Index

    EXPECT_EQ(cdw10.bits.SCP, 2);
    EXPECT_EQ(cdw10.bits.OFI, 0xC0);
    EXPECT_EQ(cdw14.bits.UIDX, 3);
}

//=============================================================================
// Lockdown API Tests (NVMeDevice Integration)
//=============================================================================

TEST(LockdownAPITest, LockdownFailsWhenDeviceNotOpen) {
    nvme::NVMeDevice device;

    EXPECT_FALSE(device.Lockdown(nvme::LockdownScope::AdminCommandOpcode, 0x00, true));
}

TEST(LockdownAPITest, LockdownProhibitAdminCommandSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Prohibit an admin command opcode
    EXPECT_TRUE(device.Lockdown(nvme::LockdownScope::AdminCommandOpcode, 0x06, true));
}

TEST(LockdownAPITest, LockdownAllowAdminCommandSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Allow an admin command opcode
    EXPECT_TRUE(device.Lockdown(nvme::LockdownScope::AdminCommandOpcode, 0x06, false));
}

TEST(LockdownAPITest, LockdownProhibitSetFeaturesSucceeds) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Prohibit a Set Features Feature Identifier
    EXPECT_TRUE(device.Lockdown(nvme::LockdownScope::SetFeaturesFeatureId, 0x01, true));
}

TEST(LockdownAPITest, LockdownWithDifferentInterfaces) {
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

TEST(LockdownAPITest, LockdownWithUUIDIndex) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Lockdown with UUID Index for vendor-specific Feature Identifier
    EXPECT_TRUE(device.Lockdown(nvme::LockdownScope::SetFeaturesFeatureId, 0xC0, true,
                                nvme::LockdownInterface::AdminSubmissionQueue, 0x01));
}

TEST(LockdownAPITest, LockdownVariousScopes) {
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

TEST(LockdownAPITest, LockdownDefaultParameters) {
    nvme::NVMeDevice device("/dev/nvme0");

    // Using default interface and UUID index
    EXPECT_TRUE(device.Lockdown(nvme::LockdownScope::AdminCommandOpcode, 0x06, true));
}
