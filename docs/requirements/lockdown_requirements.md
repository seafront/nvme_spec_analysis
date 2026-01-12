# Lockdown Command 요구사항 명세서

## 1. 기본 정보

| 항목 | 값 |
|------|-----|
| **Command Name** | Lockdown |
| **Opcode** | 0x24 (Admin Command) |
| **Spec Section** | 5.1.15 |
| **Spec Version** | NVM Express Base Specification, Revision 2.2 |
| **Figure Numbers** | Figure 338, Figure 339, Figure 340 |

### 1.1 명령 개요

Lockdown 명령은 Command and Feature Lockdown 기능(섹션 8.1.5 참조)을 제어하는 데 사용됩니다. 이 명령은 특정 명령의 실행 또는 특정 Feature Identifier를 대상으로 하는 Set Features 명령의 실행을 금지하거나 허용하도록 구성합니다.

Lockdown 명령이 성공적으로 완료되어 명령 또는 Feature Identifier를 금지하면, NVM 서브시스템의 모든 컨트롤러(해당되는 경우) 및 모든 관리 엔드포인트(해당되는 경우)가 섹션 8.1.5에 설명된 대로 동작합니다.

---

## 2. Command Dword 구조

### 2.1 Command Dword 10 (CDW10) - Figure 338

| Bits | Field | Description |
|------|-------|-------------|
| 31:16 | Reserved | 예약됨 |
| 15:08 | **OFI** (Opcode or Feature Identifier) | Scope 필드에 의해 식별되는 명령 opcode 또는 Set Features Feature Identifier를 지정 |
| 07 | Reserved | 예약됨 |
| 06:05 | **IFC** (Interface) | 이 명령의 영향을 받는 인터페이스를 식별. 지정된 인터페이스에서 명령이 수신될 때 이 명령의 동작이 적용됨 |
| 04 | **PRHBT** (Prohibit) | 이 명령에서 지정한 명령 opcode 또는 Set Features Feature Identifier의 금지 또는 허용 여부를 지정 |
| 03:00 | **SCP** (Scope) | Opcode or Feature Identifier 필드의 내용을 지정 |

### 2.2 Command Dword 14 (CDW14) - Figure 339

| Bits | Field | Description |
|------|-------|-------------|
| 31:07 | Reserved | 예약됨 |
| 06:00 | **UIDX** (UUID Index) | UUID Index 값을 지정 (Figure 659 참조) |

---

## 3. Enum 정의

### 3.1 Interface (IFC) 값 - CDW10 Bits 06:05

| Value | Name | Description |
|-------|------|-------------|
| 00b (0) | ADMIN_SUBMISSION_QUEUE | Admin Submission Queue |
| 01b (1) | ADMIN_SQ_AND_MGMT_ENDPOINT | Admin Submission Queue 및 Management Endpoint의 out-of-band |
| 10b (2) | MGMT_ENDPOINT_ONLY | Management Endpoint의 out-of-band만 |
| 11b (3) | RESERVED | 예약됨 |

### 3.2 Prohibit (PRHBT) 값 - CDW10 Bit 04

| Value | Name | Description |
|-------|------|-------------|
| 0 | ALLOW | 명령 실행 허용 |
| 1 | PROHIBIT | 명령 실행 금지 |

### 3.3 Scope (SCP) 값 - CDW10 Bits 03:00

| Value | Name | Description |
|-------|------|-------------|
| 0h | ADMIN_COMMAND_OPCODE | Admin command opcode |
| 1h | RESERVED_1 | 예약됨 |
| 2h | SET_FEATURES_FID | Set Features Feature Identifier |
| 3h | MI_COMMAND_SET_OPCODE | Management Interface Command Set opcode (NVM Express Management Interface Specification 참조) |
| 4h | PCIE_COMMAND_SET_OPCODE | PCIe Command Set opcode (NVM Express Management Interface Specification 참조) |
| 5h-Fh | RESERVED | 예약됨 |

---

## 4. Status Codes

### 4.1 Command Specific Status Values - Figure 340

| Value | Name | Description |
|-------|------|-------------|
| 28h | PROHIBITION_NOT_SUPPORTED | 지정된 opcode 또는 Feature Identifier가 명령에 의한 실행 금지를 지원하지 않아 명령이 중단됨 |

### 4.2 Generic Status Codes (적용 가능)

| Value | Name | Condition |
|-------|------|-----------|
| 00h | SUCCESS | 명령이 성공적으로 완료됨 |
| 02h | INVALID_FIELD | Interface 필드가 01b 또는 10b이고 NVM 서브시스템에 Management Endpoint가 없는 경우, 또는 Interface 필드가 00b 또는 01b이고 Scope 필드가 4h인 경우 |

---

## 5. 기능 요구사항

| ID | 요구사항 | 우선순위 | 스펙 참조 |
|----|----------|----------|-----------|
| REQ-LKD-001 | Lockdown 명령은 Admin Command opcode 0x24를 사용해야 한다 | P1 | Section 5.1.15 |
| REQ-LKD-002 | CDW10의 OFI 필드(bits 15:08)는 금지/허용할 명령 opcode 또는 Feature Identifier를 지정해야 한다 | P1 | Figure 338 |
| REQ-LKD-003 | CDW10의 IFC 필드(bits 06:05)는 영향받는 인터페이스를 지정해야 한다 | P1 | Figure 338 |
| REQ-LKD-004 | CDW10의 PRHBT 필드(bit 04)가 1이면 명령 실행을 금지하고, 0이면 허용해야 한다 | P1 | Figure 338 |
| REQ-LKD-005 | CDW10의 SCP 필드(bits 03:00)는 OFI 필드의 내용 유형을 지정해야 한다 | P1 | Figure 338 |
| REQ-LKD-006 | SCP 값 0h는 Admin command opcode를 의미해야 한다 | P1 | Figure 338 |
| REQ-LKD-007 | SCP 값 2h는 Set Features Feature Identifier를 의미해야 한다 | P1 | Figure 338 |
| REQ-LKD-008 | SCP 값 3h는 Management Interface Command Set opcode를 의미해야 한다 | P2 | Figure 338 |
| REQ-LKD-009 | SCP 값 4h는 PCIe Command Set opcode를 의미해야 한다 | P2 | Figure 338 |
| REQ-LKD-010 | IFC 값 00b는 Admin Submission Queue만 영향받아야 한다 | P1 | Figure 338 |
| REQ-LKD-011 | IFC 값 01b는 Admin Submission Queue와 Management Endpoint 모두 영향받아야 한다 | P2 | Figure 338 |
| REQ-LKD-012 | IFC 값 10b는 Management Endpoint만 영향받아야 한다 | P2 | Figure 338 |
| REQ-LKD-013 | CDW14의 UIDX 필드(bits 06:00)는 UUID Index 값을 지정해야 한다 | P2 | Figure 339 |
| REQ-LKD-014 | 컨트롤러가 UUID 선택을 지원하고 SCP가 2h(vendor specific FID)인 경우에만 CDW14의 UUID Index가 사용되어야 한다 | P2 | Section 5.1.15 |
| REQ-LKD-015 | SCP 필드가 2h가 아닌 경우 UUID Index 필드는 무시되어야 한다 | P2 | Section 5.1.15 |
| REQ-LKD-016 | 금지 불가능한 opcode 또는 Feature Identifier가 지정되면 status code 28h(Prohibition of Command Execution Not Supported)로 명령이 중단되어야 한다 | P1 | Figure 340 |
| REQ-LKD-017 | IFC 필드가 01b 또는 10b이고 NVM 서브시스템에 Management Endpoint가 없으면 Invalid Field in Command 상태 코드로 명령이 중단되어야 한다 | P1 | Section 5.1.15 |
| REQ-LKD-018 | IFC 필드가 00b 또는 01b이고 SCP 필드가 4h이면 Invalid Field in Command 상태 코드로 명령이 중단되어야 한다 | P1 | Section 5.1.15 |
| REQ-LKD-019 | 이미 금지된 명령/FID를 다시 금지하거나 이미 허용된 명령/FID를 다시 허용하는 것은 오류가 아니어야 한다 | P1 | Section 5.1.15 |
| REQ-LKD-020 | Lockdown 명령 완료 시 컨트롤러는 Admin Completion Queue에 completion queue entry를 post해야 한다 | P1 | Section 5.1.15 |
| REQ-LKD-021 | CDW10의 bits 31:16, bit 07은 Reserved이며 0으로 설정되어야 한다 | P2 | Figure 338 |
| REQ-LKD-022 | CDW14의 bits 31:07은 Reserved이며 0으로 설정되어야 한다 | P2 | Figure 339 |
| REQ-LKD-023 | IFC 값 11b는 Reserved이며 사용하면 안 된다 | P2 | Figure 338 |
| REQ-LKD-024 | SCP 값 1h, 5h-Fh는 Reserved이며 사용하면 안 된다 | P2 | Figure 338 |

---

## 6. 구현 참고사항

### 6.1 C++ 구조체 설계

```cpp
// CDW10 Union 구조
union LockdownCDW10 {
    struct {
        uint32_t SCP   : 4;   // Bits 03:00 - Scope
        uint32_t PRHBT : 1;   // Bit 04 - Prohibit
        uint32_t IFC   : 2;   // Bits 06:05 - Interface
        uint32_t Rsvd7 : 1;   // Bit 07 - Reserved
        uint32_t OFI   : 8;   // Bits 15:08 - Opcode or Feature Identifier
        uint32_t Rsvd  : 16;  // Bits 31:16 - Reserved
    } bits;
    uint32_t raw;
};

// CDW14 Union 구조
union LockdownCDW14 {
    struct {
        uint32_t UIDX : 7;    // Bits 06:00 - UUID Index
        uint32_t Rsvd : 25;   // Bits 31:07 - Reserved
    } bits;
    uint32_t raw;
};
```

### 6.2 Enum 정의

```cpp
enum class LockdownScope : uint8_t {
    AdminCommandOpcode = 0x0,
    Reserved1 = 0x1,
    SetFeaturesFID = 0x2,
    MICommandSetOpcode = 0x3,
    PCIeCommandSetOpcode = 0x4,
    // 0x5 - 0xF: Reserved
};

enum class LockdownInterface : uint8_t {
    AdminSubmissionQueue = 0x0,
    AdminSQAndMgmtEndpoint = 0x1,
    MgmtEndpointOnly = 0x2,
    Reserved = 0x3,
};

enum class LockdownProhibit : uint8_t {
    Allow = 0,
    Prohibit = 1,
};
```

---

## 7. 변경 이력

| 버전 | 날짜 | 작성자 | 변경 내용 |
|------|------|--------|-----------|
| 1.0 | 2026-01-12 | Claude | 초기 작성 |
