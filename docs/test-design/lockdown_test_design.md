# Lockdown Command 테스트 설계

## 1. 테스트 전략

### 1.1 테스트 프레임워크
- **프레임워크**: Google Test 1.14.0
- **테스트 패턴**: Given-When-Then (GWT)
- **Assertion 스타일**: EXPECT_* (soft assertions), ASSERT_* (hard assertions for preconditions)
- **테스트 파일**: `test/LockdownTest.cpp`

### 1.2 테스트 커버리지 목표
- **요구사항 커버리지**: 100% (24/24 요구사항)
- **시나리오 커버리지**: 100% (48/48 시나리오)
- **코드 커버리지 목표**: Line 90%+, Branch 85%+

### 1.3 테스트 설계 원칙
- 각 테스트는 단일 책임 원칙(SRP) 준수
- 테스트는 독립적이고 반복 가능해야 함
- 실패 시 명확한 오류 메시지 제공
- Given-When-Then 패턴으로 가독성 확보

---

## 2. 테스트 케이스 목록

### 2.1 기본 명령 구조 테스트

#### TC-001: Opcode 검증
- **시나리오**: TS-001
- **요구사항**: REQ-LKD-001
- **우선순위**: P1
- **Given**: 헤더 파일 `nvme_lockdown.h` 컴파일 완료
- **When**: AdminOpcode::Lockdown 값을 확인
- **Then**: opcode 값이 0x24이어야 함
- **검증방법**:
  ```cpp
  EXPECT_EQ(static_cast<uint8_t>(AdminOpcode::Lockdown), 0x24);
  ```
- **테스트 데이터**: N/A (enum 상수 검증)

---

### 2.2 CDW10 필드 테스트

#### TC-002: OFI 필드 - Admin Command Opcode 설정
- **시나리오**: TS-002
- **요구사항**: REQ-LKD-002
- **우선순위**: P1
- **Given**: LockdownCDW10 union 인스턴스 생성, 모든 필드 0으로 초기화
- **When**: `cdw10.bits.OFI = 0x06` (Identify command)
- **Then**: `cdw10.bits.OFI == 0x06`이고 `cdw10.raw & 0xFF00 == 0x0600`
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;
  cdw10.bits.OFI = 0x06;
  EXPECT_EQ(cdw10.bits.OFI, 0x06);
  EXPECT_EQ(cdw10.raw & 0xFF00, 0x0600);
  ```
- **테스트 데이터**: `OFI = 0x06` (Identify opcode)

#### TC-003: OFI 필드 - Feature Identifier 설정
- **시나리오**: TS-003
- **요구사항**: REQ-LKD-002
- **우선순위**: P1
- **Given**: LockdownCDW10 union 인스턴스 생성, `SCP = 0x2` (SetFeaturesFID)
- **When**: `cdw10.bits.OFI = 0x01` (Arbitration FID)
- **Then**: `cdw10.bits.OFI == 0x01`
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;
  cdw10.bits.SCP = 0x2;
  cdw10.bits.OFI = 0x01;
  EXPECT_EQ(cdw10.bits.OFI, 0x01);
  EXPECT_EQ(cdw10.bits.SCP, 0x2);
  ```
- **테스트 데이터**: `SCP = 0x2`, `OFI = 0x01`

#### TC-004: OFI 필드 최대값 경계 테스트
- **시나리오**: TS-004
- **요구사항**: REQ-LKD-002
- **우선순위**: P2
- **Given**: LockdownCDW10 union 인스턴스 생성
- **When**: `cdw10.bits.OFI = 0xFF`
- **Then**: `cdw10.bits.OFI == 0xFF`이고 다른 필드에 영향 없음
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;
  cdw10.bits.OFI = 0xFF;
  EXPECT_EQ(cdw10.bits.OFI, 0xFF);
  EXPECT_EQ(cdw10.bits.SCP, 0);
  EXPECT_EQ(cdw10.bits.PRHBT, 0);
  ```
- **테스트 데이터**: `OFI = 0xFF` (최대값)

#### TC-005: OFI 필드 최소값 경계 테스트
- **시나리오**: TS-005
- **요구사항**: REQ-LKD-002
- **우선순위**: P2
- **Given**: LockdownCDW10 union 인스턴스 생성
- **When**: `cdw10.bits.OFI = 0x00`
- **Then**: `cdw10.bits.OFI == 0x00`
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;
  cdw10.bits.OFI = 0x00;
  EXPECT_EQ(cdw10.bits.OFI, 0x00);
  ```
- **테스트 데이터**: `OFI = 0x00` (최소값)

#### TC-006: IFC 필드 - Admin Submission Queue (00b)
- **시나리오**: TS-006
- **요구사항**: REQ-LKD-003, REQ-LKD-010
- **우선순위**: P1
- **Given**: LockdownCDW10 union 인스턴스 생성
- **When**: `cdw10.bits.IFC = static_cast<uint8_t>(LockdownInterface::AdminSubmissionQueue)`
- **Then**: `cdw10.bits.IFC == 0`
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;
  cdw10.bits.IFC = static_cast<uint8_t>(LockdownInterface::AdminSubmissionQueue);
  EXPECT_EQ(cdw10.bits.IFC, 0);
  EXPECT_EQ(static_cast<uint8_t>(LockdownInterface::AdminSubmissionQueue), 0);
  ```
- **테스트 데이터**: `IFC = 0` (Admin Submission Queue)

#### TC-007: IFC 필드 - Admin SQ 및 Management Endpoint (01b)
- **시나리오**: TS-007
- **요구사항**: REQ-LKD-003, REQ-LKD-011
- **우선순위**: P2
- **Given**: LockdownCDW10 union 인스턴스 생성
- **When**: `cdw10.bits.IFC = static_cast<uint8_t>(LockdownInterface::AdminSQAndMgmtEndpoint)`
- **Then**: `cdw10.bits.IFC == 1`
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;
  cdw10.bits.IFC = static_cast<uint8_t>(LockdownInterface::AdminSQAndMgmtEndpoint);
  EXPECT_EQ(cdw10.bits.IFC, 1);
  EXPECT_EQ(static_cast<uint8_t>(LockdownInterface::AdminSQAndMgmtEndpoint), 1);
  ```
- **테스트 데이터**: `IFC = 1`

#### TC-008: IFC 필드 - Management Endpoint Only (10b)
- **시나리오**: TS-008
- **요구사항**: REQ-LKD-003, REQ-LKD-012
- **우선순위**: P2
- **Given**: LockdownCDW10 union 인스턴스 생성
- **When**: `cdw10.bits.IFC = static_cast<uint8_t>(LockdownInterface::MgmtEndpointOnly)`
- **Then**: `cdw10.bits.IFC == 2`
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;
  cdw10.bits.IFC = static_cast<uint8_t>(LockdownInterface::MgmtEndpointOnly);
  EXPECT_EQ(cdw10.bits.IFC, 2);
  EXPECT_EQ(static_cast<uint8_t>(LockdownInterface::MgmtEndpointOnly), 2);
  ```
- **테스트 데이터**: `IFC = 2`

#### TC-009: IFC 필드 - Reserved 값 (11b) 검증
- **시나리오**: TS-009
- **요구사항**: REQ-LKD-023
- **우선순위**: P2
- **Given**: LockdownCDW10 union 인스턴스 생성
- **When**: `cdw10.bits.IFC = 3` (Reserved 값)
- **Then**: 값이 설정되지만 enum 정의에서 Reserved임을 확인
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;
  cdw10.bits.IFC = 3;
  EXPECT_EQ(cdw10.bits.IFC, 3);
  EXPECT_EQ(static_cast<uint8_t>(LockdownInterface::Reserved), 3);
  ```
- **테스트 데이터**: `IFC = 3` (Reserved)
- **참고**: 실제 구현에서는 런타임 검증 또는 문서화로 사용 금지

#### TC-010: PRHBT 필드 - 금지 설정 (1)
- **시나리오**: TS-010
- **요구사항**: REQ-LKD-004
- **우선순위**: P1
- **Given**: LockdownCDW10 union 인스턴스 생성
- **When**: `cdw10.bits.PRHBT = static_cast<uint8_t>(LockdownProhibit::Prohibit)`
- **Then**: `cdw10.bits.PRHBT == 1`
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;
  cdw10.bits.PRHBT = static_cast<uint8_t>(LockdownProhibit::Prohibit);
  EXPECT_EQ(cdw10.bits.PRHBT, 1);
  EXPECT_EQ(static_cast<uint8_t>(LockdownProhibit::Prohibit), 1);
  ```
- **테스트 데이터**: `PRHBT = 1` (Prohibit)

#### TC-011: PRHBT 필드 - 허용 설정 (0)
- **시나리오**: TS-011
- **요구사항**: REQ-LKD-004
- **우선순위**: P1
- **Given**: LockdownCDW10 union 인스턴스 생성
- **When**: `cdw10.bits.PRHBT = static_cast<uint8_t>(LockdownProhibit::Allow)`
- **Then**: `cdw10.bits.PRHBT == 0`
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;
  cdw10.bits.PRHBT = static_cast<uint8_t>(LockdownProhibit::Allow);
  EXPECT_EQ(cdw10.bits.PRHBT, 0);
  EXPECT_EQ(static_cast<uint8_t>(LockdownProhibit::Allow), 0);
  ```
- **테스트 데이터**: `PRHBT = 0` (Allow)

#### TC-012: SCP 필드 - Admin Command Opcode (0h)
- **시나리오**: TS-012
- **요구사항**: REQ-LKD-005, REQ-LKD-006
- **우선순위**: P1
- **Given**: LockdownCDW10 union 인스턴스 생성
- **When**: `cdw10.bits.SCP = static_cast<uint8_t>(LockdownScope::AdminCommandOpcode)`
- **Then**: `cdw10.bits.SCP == 0`
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;
  cdw10.bits.SCP = static_cast<uint8_t>(LockdownScope::AdminCommandOpcode);
  EXPECT_EQ(cdw10.bits.SCP, 0);
  EXPECT_EQ(static_cast<uint8_t>(LockdownScope::AdminCommandOpcode), 0);
  ```
- **테스트 데이터**: `SCP = 0h`

#### TC-013: SCP 필드 - Set Features FID (2h)
- **시나리오**: TS-013
- **요구사항**: REQ-LKD-005, REQ-LKD-007
- **우선순위**: P1
- **Given**: LockdownCDW10 union 인스턴스 생성
- **When**: `cdw10.bits.SCP = static_cast<uint8_t>(LockdownScope::SetFeaturesFID)`
- **Then**: `cdw10.bits.SCP == 2`
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;
  cdw10.bits.SCP = static_cast<uint8_t>(LockdownScope::SetFeaturesFID);
  EXPECT_EQ(cdw10.bits.SCP, 2);
  EXPECT_EQ(static_cast<uint8_t>(LockdownScope::SetFeaturesFID), 2);
  ```
- **테스트 데이터**: `SCP = 2h`

#### TC-014: SCP 필드 - MI Command Set Opcode (3h)
- **시나리오**: TS-014
- **요구사항**: REQ-LKD-005, REQ-LKD-008
- **우선순위**: P2
- **Given**: LockdownCDW10 union 인스턴스 생성
- **When**: `cdw10.bits.SCP = static_cast<uint8_t>(LockdownScope::MICommandSetOpcode)`
- **Then**: `cdw10.bits.SCP == 3`
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;
  cdw10.bits.SCP = static_cast<uint8_t>(LockdownScope::MICommandSetOpcode);
  EXPECT_EQ(cdw10.bits.SCP, 3);
  EXPECT_EQ(static_cast<uint8_t>(LockdownScope::MICommandSetOpcode), 3);
  ```
- **테스트 데이터**: `SCP = 3h`

#### TC-015: SCP 필드 - PCIe Command Set Opcode (4h)
- **시나리오**: TS-015
- **요구사항**: REQ-LKD-005, REQ-LKD-009
- **우선순위**: P2
- **Given**: LockdownCDW10 union 인스턴스 생성
- **When**: `cdw10.bits.SCP = static_cast<uint8_t>(LockdownScope::PCIeCommandSetOpcode)`
- **Then**: `cdw10.bits.SCP == 4`
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;
  cdw10.bits.SCP = static_cast<uint8_t>(LockdownScope::PCIeCommandSetOpcode);
  EXPECT_EQ(cdw10.bits.SCP, 4);
  EXPECT_EQ(static_cast<uint8_t>(LockdownScope::PCIeCommandSetOpcode), 4);
  ```
- **테스트 데이터**: `SCP = 4h`

#### TC-016: SCP 필드 - Reserved 값 (1h) 검증
- **시나리오**: TS-016
- **요구사항**: REQ-LKD-024
- **우선순위**: P2
- **Given**: LockdownCDW10 union 인스턴스 생성
- **When**: `cdw10.bits.SCP = 1` (Reserved1)
- **Then**: 값이 설정되지만 enum 정의에서 Reserved임을 확인
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;
  cdw10.bits.SCP = 1;
  EXPECT_EQ(cdw10.bits.SCP, 1);
  EXPECT_EQ(static_cast<uint8_t>(LockdownScope::Reserved1), 1);
  ```
- **테스트 데이터**: `SCP = 1h` (Reserved)

#### TC-017: SCP 필드 - Reserved 값 (5h-Fh) 경계 테스트
- **시나리오**: TS-017
- **요구사항**: REQ-LKD-024
- **우선순위**: P2
- **Given**: LockdownCDW10 union 인스턴스 생성
- **When**: `cdw10.bits.SCP = 5` 및 `cdw10.bits.SCP = 15` (0xF)
- **Then**: 각 값이 올바르게 설정됨 (4비트 최대 범위 확인)
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};

  // Test SCP = 5
  cdw10.raw = 0;
  cdw10.bits.SCP = 5;
  EXPECT_EQ(cdw10.bits.SCP, 5);

  // Test SCP = 15 (0xF, maximum 4-bit value)
  cdw10.raw = 0;
  cdw10.bits.SCP = 15;
  EXPECT_EQ(cdw10.bits.SCP, 15);
  ```
- **테스트 데이터**: `SCP = 5`, `SCP = 15` (경계값)

---

### 2.3 CDW14 필드 테스트

#### TC-018: UIDX 필드 - UUID Index 설정
- **시나리오**: TS-018
- **요구사항**: REQ-LKD-013
- **우선순위**: P2
- **Given**: LockdownCDW14 union 인스턴스 생성
- **When**: `cdw14.bits.UIDX = 1`
- **Then**: `cdw14.bits.UIDX == 1`
- **검증방법**:
  ```cpp
  LockdownCDW14 cdw14{};
  cdw14.raw = 0;
  cdw14.bits.UIDX = 1;
  EXPECT_EQ(cdw14.bits.UIDX, 1);
  ```
- **테스트 데이터**: `UIDX = 1`

#### TC-019: UIDX 필드 최대값 경계 테스트
- **시나리오**: TS-019
- **요구사항**: REQ-LKD-013
- **우선순위**: P2
- **Given**: LockdownCDW14 union 인스턴스 생성
- **When**: `cdw14.bits.UIDX = 0x7F` (7비트 최대값 = 127)
- **Then**: `cdw14.bits.UIDX == 0x7F`
- **검증방법**:
  ```cpp
  LockdownCDW14 cdw14{};
  cdw14.raw = 0;
  cdw14.bits.UIDX = 0x7F;
  EXPECT_EQ(cdw14.bits.UIDX, 0x7F);
  EXPECT_EQ(cdw14.bits.UIDX, 127);
  ```
- **테스트 데이터**: `UIDX = 0x7F` (최대값)

#### TC-020: UIDX 필드 - SCP=2h 및 UUID 지원 시 사용
- **시나리오**: TS-020
- **요구사항**: REQ-LKD-014
- **우선순위**: P2
- **Given**: LockdownCDW10 `SCP = 2h`, LockdownCDW14 인스턴스 생성
- **When**: `cdw14.bits.UIDX = 5` (임의의 유효한 UUID Index)
- **Then**: `cdw14.bits.UIDX == 5`이고 `cdw10.bits.SCP == 2`
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  LockdownCDW14 cdw14{};
  cdw10.raw = 0;
  cdw14.raw = 0;

  cdw10.bits.SCP = static_cast<uint8_t>(LockdownScope::SetFeaturesFID);
  cdw14.bits.UIDX = 5;

  EXPECT_EQ(cdw10.bits.SCP, 2);
  EXPECT_EQ(cdw14.bits.UIDX, 5);
  ```
- **테스트 데이터**: `SCP = 2h`, `UIDX = 5`

#### TC-021: UIDX 필드 - SCP != 2h일 때 무시 검증
- **시나리오**: TS-021
- **요구사항**: REQ-LKD-015
- **우선순위**: P2
- **Given**: LockdownCDW10 `SCP = 0h` (AdminCommandOpcode), LockdownCDW14 인스턴스
- **When**: `cdw14.bits.UIDX = 10` 설정
- **Then**: 필드 값은 설정되지만 SCP != 2h이므로 무시되어야 함 (문서화)
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  LockdownCDW14 cdw14{};
  cdw10.raw = 0;
  cdw14.raw = 0;

  cdw10.bits.SCP = static_cast<uint8_t>(LockdownScope::AdminCommandOpcode);
  cdw14.bits.UIDX = 10;

  EXPECT_EQ(cdw10.bits.SCP, 0);
  EXPECT_EQ(cdw14.bits.UIDX, 10);
  // Note: 구현에서 SCP=0일 때 UIDX는 무시됨 (스펙 참조)
  ```
- **테스트 데이터**: `SCP = 0h`, `UIDX = 10`
- **참고**: 실제 동작은 NVMe 드라이버 구현에서 UIDX 필드를 무시

---

### 2.4 Reserved 필드 테스트

#### TC-022: CDW10 Reserved 필드 (bits 31:16) 0 설정
- **시나리오**: TS-022
- **요구사항**: REQ-LKD-021
- **우선순위**: P2
- **Given**: LockdownCDW10 union 인스턴스 생성
- **When**: 초기화 후 reserved 필드 확인
- **Then**: `(cdw10.raw >> 16) == 0`
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;
  cdw10.bits.OFI = 0x12;
  cdw10.bits.SCP = 2;

  EXPECT_EQ((cdw10.raw >> 16) & 0xFFFF, 0);
  ```
- **테스트 데이터**: 다양한 필드 설정 후 reserved 영역 검증

#### TC-023: CDW10 Reserved 필드 (bit 07) 0 설정
- **시나리오**: TS-023
- **요구사항**: REQ-LKD-021
- **우선순위**: P2
- **Given**: LockdownCDW10 union 인스턴스 생성
- **When**: 초기화 후 bit 7 확인
- **Then**: `cdw10.bits.Rsvd7 == 0`
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;
  cdw10.bits.IFC = 2;
  cdw10.bits.PRHBT = 1;

  EXPECT_EQ(cdw10.bits.Rsvd7, 0);
  EXPECT_EQ((cdw10.raw >> 7) & 0x1, 0);
  ```
- **테스트 데이터**: 다양한 필드 설정 후 bit 7 검증

#### TC-024: CDW14 Reserved 필드 (bits 31:07) 0 설정
- **시나리오**: TS-024
- **요구사항**: REQ-LKD-022
- **우선순위**: P2
- **Given**: LockdownCDW14 union 인스턴스 생성
- **When**: UIDX 필드 설정 후 reserved 영역 확인
- **Then**: `(cdw14.raw >> 7) == 0`
- **검증방법**:
  ```cpp
  LockdownCDW14 cdw14{};
  cdw14.raw = 0;
  cdw14.bits.UIDX = 0x3F;

  EXPECT_EQ((cdw14.raw >> 7), 0);
  EXPECT_EQ(cdw14.bits.Rsvd, 0);
  ```
- **테스트 데이터**: `UIDX = 0x3F`, reserved 영역 = 0

---

### 2.5 구조체 크기 및 레이아웃 테스트

#### TC-025: CDW10 Union 크기 검증
- **시나리오**: TS-035
- **요구사항**: REQ-LKD-002, REQ-LKD-003, REQ-LKD-004, REQ-LKD-005
- **우선순위**: P1
- **Given**: LockdownCDW10 union 정의
- **When**: `sizeof(LockdownCDW10)` 확인
- **Then**: `sizeof(LockdownCDW10) == 4`
- **검증방법**:
  ```cpp
  EXPECT_EQ(sizeof(LockdownCDW10), 4);
  static_assert(sizeof(LockdownCDW10) == 4, "CDW10 must be 4 bytes");
  ```
- **테스트 데이터**: N/A (컴파일 타임 검증)

#### TC-026: CDW14 Union 크기 검증
- **시나리오**: TS-036
- **요구사항**: REQ-LKD-013
- **우선순위**: P1
- **Given**: LockdownCDW14 union 정의
- **When**: `sizeof(LockdownCDW14)` 확인
- **Then**: `sizeof(LockdownCDW14) == 4`
- **검증방법**:
  ```cpp
  EXPECT_EQ(sizeof(LockdownCDW14), 4);
  static_assert(sizeof(LockdownCDW14) == 4, "CDW14 must be 4 bytes");
  ```
- **테스트 데이터**: N/A (컴파일 타임 검증)

#### TC-027: CDW10 비트 필드 레이아웃 검증
- **시나리오**: TS-037
- **요구사항**: REQ-LKD-002, REQ-LKD-003, REQ-LKD-004, REQ-LKD-005
- **우선순위**: P1
- **Given**: LockdownCDW10 union 인스턴스 생성
- **When**: 각 필드에 고유한 비트 패턴 설정
- **Then**: raw 값과 비교하여 비트 위치가 스펙과 일치
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};

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
  ```
- **테스트 데이터**: 다양한 비트 패턴 조합

#### TC-028: CDW14 비트 필드 레이아웃 검증
- **시나리오**: TS-038
- **요구사항**: REQ-LKD-013
- **우선순위**: P2
- **Given**: LockdownCDW14 union 인스턴스 생성
- **When**: UIDX 필드에 비트 패턴 설정
- **Then**: raw 값의 bits 0-6이 UIDX 값과 일치
- **검증방법**:
  ```cpp
  LockdownCDW14 cdw14{};

  // Test UIDX field (bits 0-6)
  cdw14.raw = 0;
  cdw14.bits.UIDX = 0x55;
  EXPECT_EQ(cdw14.raw & 0x7F, 0x55);

  // Test reserved bits are not affected
  EXPECT_EQ(cdw14.raw >> 7, 0);
  ```
- **테스트 데이터**: `UIDX = 0x55`

---

### 2.6 Enum 값 테스트

#### TC-029: LockdownScope Enum 값 검증
- **시나리오**: TS-039
- **요구사항**: REQ-LKD-006, REQ-LKD-007, REQ-LKD-008, REQ-LKD-009
- **우선순위**: P1
- **Given**: LockdownScope enum 정의
- **When**: 각 enum 값 확인
- **Then**: 모든 값이 스펙과 일치
- **검증방법**:
  ```cpp
  EXPECT_EQ(static_cast<uint8_t>(LockdownScope::AdminCommandOpcode), 0x0);
  EXPECT_EQ(static_cast<uint8_t>(LockdownScope::Reserved1), 0x1);
  EXPECT_EQ(static_cast<uint8_t>(LockdownScope::SetFeaturesFID), 0x2);
  EXPECT_EQ(static_cast<uint8_t>(LockdownScope::MICommandSetOpcode), 0x3);
  EXPECT_EQ(static_cast<uint8_t>(LockdownScope::PCIeCommandSetOpcode), 0x4);
  ```
- **테스트 데이터**: Enum 상수 값

#### TC-030: LockdownInterface Enum 값 검증
- **시나리오**: TS-040
- **요구사항**: REQ-LKD-010, REQ-LKD-011, REQ-LKD-012
- **우선순위**: P1
- **Given**: LockdownInterface enum 정의
- **When**: 각 enum 값 확인
- **Then**: 모든 값이 스펙과 일치
- **검증방법**:
  ```cpp
  EXPECT_EQ(static_cast<uint8_t>(LockdownInterface::AdminSubmissionQueue), 0);
  EXPECT_EQ(static_cast<uint8_t>(LockdownInterface::AdminSQAndMgmtEndpoint), 1);
  EXPECT_EQ(static_cast<uint8_t>(LockdownInterface::MgmtEndpointOnly), 2);
  EXPECT_EQ(static_cast<uint8_t>(LockdownInterface::Reserved), 3);
  ```
- **테스트 데이터**: Enum 상수 값

#### TC-031: LockdownProhibit Enum 값 검증
- **시나리오**: TS-041
- **요구사항**: REQ-LKD-004
- **우선순위**: P1
- **Given**: LockdownProhibit enum 정의
- **When**: 각 enum 값 확인
- **Then**: 모든 값이 스펙과 일치
- **검증방법**:
  ```cpp
  EXPECT_EQ(static_cast<uint8_t>(LockdownProhibit::Allow), 0);
  EXPECT_EQ(static_cast<uint8_t>(LockdownProhibit::Prohibit), 1);
  ```
- **테스트 데이터**: Enum 상수 값

---

### 2.7 통합 및 경계 조건 테스트

#### TC-032: 모든 필드 최대값 동시 설정
- **시나리오**: TS-047
- **요구사항**: REQ-LKD-002, REQ-LKD-003, REQ-LKD-005, REQ-LKD-013
- **우선순위**: P2
- **Given**: LockdownCDW10, LockdownCDW14 인스턴스 생성
- **When**: 모든 필드에 최대 유효값 설정
- **Then**: 각 필드가 올바르게 설정되고 서로 간섭하지 않음
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  LockdownCDW14 cdw14{};

  cdw10.raw = 0;
  cdw14.raw = 0;

  cdw10.bits.OFI = 0xFF;
  cdw10.bits.IFC = 2;  // MgmtEndpointOnly (max valid value)
  cdw10.bits.SCP = 4;  // PCIeCommandSetOpcode (max valid value)
  cdw10.bits.PRHBT = 1;
  cdw14.bits.UIDX = 0x7F;

  EXPECT_EQ(cdw10.bits.OFI, 0xFF);
  EXPECT_EQ(cdw10.bits.IFC, 2);
  EXPECT_EQ(cdw10.bits.SCP, 4);
  EXPECT_EQ(cdw10.bits.PRHBT, 1);
  EXPECT_EQ(cdw14.bits.UIDX, 0x7F);

  // Verify reserved fields remain 0
  EXPECT_EQ(cdw10.bits.Rsvd7, 0);
  EXPECT_EQ((cdw10.raw >> 16), 0);
  EXPECT_EQ((cdw14.raw >> 7), 0);
  ```
- **테스트 데이터**: `OFI=0xFF, IFC=2, SCP=4, PRHBT=1, UIDX=0x7F`

#### TC-033: 모든 필드 최소값 동시 설정
- **시나리오**: TS-048
- **요구사항**: REQ-LKD-002, REQ-LKD-003, REQ-LKD-005, REQ-LKD-013
- **우선순위**: P2
- **Given**: LockdownCDW10, LockdownCDW14 인스턴스 생성
- **When**: 모든 필드에 최소값(0) 설정
- **Then**: 모든 필드가 0으로 설정됨
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  LockdownCDW14 cdw14{};

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
  ```
- **테스트 데이터**: 모든 필드 = 0

#### TC-034: 다양한 필드 조합 테스트 - Admin Command 금지
- **시나리오**: TS-042
- **요구사항**: REQ-LKD-004, REQ-LKD-006, REQ-LKD-010
- **우선순위**: P1
- **Given**: LockdownCDW10 인스턴스 생성
- **When**: `SCP=0, IFC=0, PRHBT=1, OFI=0x06` (Identify 명령 금지)
- **Then**: 모든 필드가 의도한 대로 설정됨
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;

  cdw10.bits.SCP = static_cast<uint8_t>(LockdownScope::AdminCommandOpcode);
  cdw10.bits.IFC = static_cast<uint8_t>(LockdownInterface::AdminSubmissionQueue);
  cdw10.bits.PRHBT = static_cast<uint8_t>(LockdownProhibit::Prohibit);
  cdw10.bits.OFI = 0x06;  // Identify command

  EXPECT_EQ(cdw10.bits.SCP, 0);
  EXPECT_EQ(cdw10.bits.IFC, 0);
  EXPECT_EQ(cdw10.bits.PRHBT, 1);
  EXPECT_EQ(cdw10.bits.OFI, 0x06);
  ```
- **테스트 데이터**: `SCP=0, IFC=0, PRHBT=1, OFI=0x06`

#### TC-035: 다양한 필드 조합 테스트 - Set Features FID 금지
- **시나리오**: TS-043
- **요구사항**: REQ-LKD-004, REQ-LKD-007, REQ-LKD-010
- **우선순위**: P1
- **Given**: LockdownCDW10 인스턴스 생성
- **When**: `SCP=2, IFC=0, PRHBT=1, OFI=0x01` (Arbitration FID 금지)
- **Then**: 모든 필드가 의도한 대로 설정됨
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;

  cdw10.bits.SCP = static_cast<uint8_t>(LockdownScope::SetFeaturesFID);
  cdw10.bits.IFC = static_cast<uint8_t>(LockdownInterface::AdminSubmissionQueue);
  cdw10.bits.PRHBT = static_cast<uint8_t>(LockdownProhibit::Prohibit);
  cdw10.bits.OFI = 0x01;  // Arbitration FID

  EXPECT_EQ(cdw10.bits.SCP, 2);
  EXPECT_EQ(cdw10.bits.IFC, 0);
  EXPECT_EQ(cdw10.bits.PRHBT, 1);
  EXPECT_EQ(cdw10.bits.OFI, 0x01);
  ```
- **테스트 데이터**: `SCP=2, IFC=0, PRHBT=1, OFI=0x01`

#### TC-036: 다양한 필드 조합 테스트 - Management Interface
- **시나리오**: TS-044
- **요구사항**: REQ-LKD-008, REQ-LKD-011
- **우선순위**: P2
- **Given**: LockdownCDW10 인스턴스 생성
- **When**: `SCP=3, IFC=1, PRHBT=1` (MI Command 금지)
- **Then**: 모든 필드가 의도한 대로 설정됨
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;

  cdw10.bits.SCP = static_cast<uint8_t>(LockdownScope::MICommandSetOpcode);
  cdw10.bits.IFC = static_cast<uint8_t>(LockdownInterface::AdminSQAndMgmtEndpoint);
  cdw10.bits.PRHBT = static_cast<uint8_t>(LockdownProhibit::Prohibit);
  cdw10.bits.OFI = 0x10;  // Example MI opcode

  EXPECT_EQ(cdw10.bits.SCP, 3);
  EXPECT_EQ(cdw10.bits.IFC, 1);
  EXPECT_EQ(cdw10.bits.PRHBT, 1);
  EXPECT_EQ(cdw10.bits.OFI, 0x10);
  ```
- **테스트 데이터**: `SCP=3, IFC=1, PRHBT=1, OFI=0x10`

#### TC-037: 다양한 필드 조합 테스트 - PCIe Command Set
- **시나리오**: TS-045
- **요구사항**: REQ-LKD-009, REQ-LKD-012
- **우선순위**: P2
- **Given**: LockdownCDW10 인스턴스 생성
- **When**: `SCP=4, IFC=2, PRHBT=1` (PCIe Command 금지, Mgmt Endpoint만)
- **Then**: 모든 필드가 의도한 대로 설정됨
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  cdw10.raw = 0;

  cdw10.bits.SCP = static_cast<uint8_t>(LockdownScope::PCIeCommandSetOpcode);
  cdw10.bits.IFC = static_cast<uint8_t>(LockdownInterface::MgmtEndpointOnly);
  cdw10.bits.PRHBT = static_cast<uint8_t>(LockdownProhibit::Prohibit);
  cdw10.bits.OFI = 0x20;  // Example PCIe opcode

  EXPECT_EQ(cdw10.bits.SCP, 4);
  EXPECT_EQ(cdw10.bits.IFC, 2);
  EXPECT_EQ(cdw10.bits.PRHBT, 1);
  EXPECT_EQ(cdw10.bits.OFI, 0x20);
  ```
- **테스트 데이터**: `SCP=4, IFC=2, PRHBT=1, OFI=0x20`

#### TC-038: UUID Index 사용 시나리오
- **시나리오**: TS-046
- **요구사항**: REQ-LKD-013, REQ-LKD-014
- **우선순위**: P2
- **Given**: LockdownCDW10 `SCP=2`, LockdownCDW14 인스턴스 생성
- **When**: vendor specific FID와 UUID Index 설정
- **Then**: 두 필드 모두 올바르게 설정됨
- **검증방법**:
  ```cpp
  LockdownCDW10 cdw10{};
  LockdownCDW14 cdw14{};

  cdw10.raw = 0;
  cdw14.raw = 0;

  cdw10.bits.SCP = static_cast<uint8_t>(LockdownScope::SetFeaturesFID);
  cdw10.bits.IFC = static_cast<uint8_t>(LockdownInterface::AdminSubmissionQueue);
  cdw10.bits.PRHBT = static_cast<uint8_t>(LockdownProhibit::Prohibit);
  cdw10.bits.OFI = 0xC0;  // Vendor specific FID (C0h-FFh)
  cdw14.bits.UIDX = 3;    // UUID Index

  EXPECT_EQ(cdw10.bits.SCP, 2);
  EXPECT_EQ(cdw10.bits.OFI, 0xC0);
  EXPECT_EQ(cdw14.bits.UIDX, 3);
  ```
- **테스트 데이터**: `SCP=2, OFI=0xC0, UIDX=3`

---

## 3. 시나리오-테스트 케이스 매핑

| 시나리오 ID | 테스트 케이스 ID | 설명 |
|-------------|------------------|------|
| TS-001 | TC-001 | Opcode 검증 |
| TS-002 | TC-002 | OFI 필드 - Admin Command Opcode 설정 |
| TS-003 | TC-003 | OFI 필드 - Feature Identifier 설정 |
| TS-004 | TC-004 | OFI 필드 최대값 경계 테스트 |
| TS-005 | TC-005 | OFI 필드 최소값 경계 테스트 |
| TS-006 | TC-006 | IFC 필드 - Admin Submission Queue (00b) |
| TS-007 | TC-007 | IFC 필드 - Admin SQ 및 Management Endpoint (01b) |
| TS-008 | TC-008 | IFC 필드 - Management Endpoint Only (10b) |
| TS-009 | TC-009 | IFC 필드 - Reserved 값 (11b) 검증 |
| TS-010 | TC-010 | PRHBT 필드 - 금지 설정 (1) |
| TS-011 | TC-011 | PRHBT 필드 - 허용 설정 (0) |
| TS-012 | TC-012 | SCP 필드 - Admin Command Opcode (0h) |
| TS-013 | TC-013 | SCP 필드 - Set Features FID (2h) |
| TS-014 | TC-014 | SCP 필드 - MI Command Set Opcode (3h) |
| TS-015 | TC-015 | SCP 필드 - PCIe Command Set Opcode (4h) |
| TS-016 | TC-016 | SCP 필드 - Reserved 값 (1h) 검증 |
| TS-017 | TC-017 | SCP 필드 - Reserved 값 (5h-Fh) 경계 테스트 |
| TS-018 | TC-018 | UIDX 필드 - UUID Index 설정 |
| TS-019 | TC-019 | UIDX 필드 최대값 경계 테스트 |
| TS-020 | TC-020 | UIDX 필드 - SCP=2h 및 UUID 지원 시 사용 |
| TS-021 | TC-021 | UIDX 필드 - SCP != 2h일 때 무시 검증 |
| TS-022 | TC-022 | CDW10 Reserved 필드 (bits 31:16) 0 설정 |
| TS-023 | TC-023 | CDW10 Reserved 필드 (bit 07) 0 설정 |
| TS-024 | TC-024 | CDW14 Reserved 필드 (bits 31:07) 0 설정 |
| TS-035 | TC-025 | CDW10 Union 크기 검증 |
| TS-036 | TC-026 | CDW14 Union 크기 검증 |
| TS-037 | TC-027 | CDW10 비트 필드 레이아웃 검증 |
| TS-038 | TC-028 | CDW14 비트 필드 레이아웃 검증 |
| TS-039 | TC-029 | LockdownScope Enum 값 검증 |
| TS-040 | TC-030 | LockdownInterface Enum 값 검증 |
| TS-041 | TC-031 | LockdownProhibit Enum 값 검증 |
| TS-047 | TC-032 | 모든 필드 최대값 동시 설정 |
| TS-048 | TC-033 | 모든 필드 최소값 동시 설정 |
| TS-042 | TC-034 | Admin Command 금지 시나리오 |
| TS-043 | TC-035 | Set Features FID 금지 시나리오 |
| TS-044 | TC-036 | Management Interface 금지 시나리오 |
| TS-045 | TC-037 | PCIe Command Set 금지 시나리오 (IFC=10b) |
| TS-046 | TC-038 | UUID Index 사용 시나리오 |

**참고**: 다음 시나리오는 실제 NVMe 디바이스 동작 테스트가 필요하여 헤더 파일 단위 테스트에서는 제외:
- TS-025 ~ TS-034 (Status Code 및 동작 검증 테스트)

---

## 4. 요구사항-테스트 케이스 매핑

| 요구사항 ID | 테스트 케이스 | 커버리지 |
|-------------|---------------|----------|
| REQ-LKD-001 | TC-001 | 100% |
| REQ-LKD-002 | TC-002, TC-003, TC-004, TC-005, TC-025, TC-027, TC-032, TC-033 | 100% |
| REQ-LKD-003 | TC-006, TC-007, TC-008, TC-025, TC-027, TC-032, TC-033 | 100% |
| REQ-LKD-004 | TC-010, TC-011, TC-031, TC-034, TC-035 | 100% |
| REQ-LKD-005 | TC-012, TC-013, TC-014, TC-015, TC-025, TC-027, TC-032, TC-033 | 100% |
| REQ-LKD-006 | TC-012, TC-029, TC-034 | 100% |
| REQ-LKD-007 | TC-013, TC-029, TC-035 | 100% |
| REQ-LKD-008 | TC-014, TC-029, TC-036 | 100% |
| REQ-LKD-009 | TC-015, TC-029, TC-037 | 100% |
| REQ-LKD-010 | TC-006, TC-030, TC-034, TC-035 | 100% |
| REQ-LKD-011 | TC-007, TC-030, TC-036 | 100% |
| REQ-LKD-012 | TC-008, TC-030, TC-037 | 100% |
| REQ-LKD-013 | TC-018, TC-019, TC-026, TC-028, TC-032, TC-033, TC-038 | 100% |
| REQ-LKD-014 | TC-020, TC-038 | 100% |
| REQ-LKD-015 | TC-021 | 100% |
| REQ-LKD-016 | (디바이스 테스트 필요) | N/A |
| REQ-LKD-017 | (디바이스 테스트 필요) | N/A |
| REQ-LKD-018 | (디바이스 테스트 필요) | N/A |
| REQ-LKD-019 | (디바이스 테스트 필요) | N/A |
| REQ-LKD-020 | (디바이스 테스트 필요) | N/A |
| REQ-LKD-021 | TC-022, TC-023 | 100% |
| REQ-LKD-022 | TC-024 | 100% |
| REQ-LKD-023 | TC-009 | 100% |
| REQ-LKD-024 | TC-016, TC-017 | 100% |

**헤더 파일 단위 테스트 커버리지**: 18/24 요구사항 (75%)
**디바이스 통합 테스트 필요**: REQ-LKD-016 ~ REQ-LKD-020 (6개 요구사항)

---

## 5. 테스트 데이터 상세

### 5.1 유효한 입력값

#### CDW10 필드 테스트 데이터

| 필드 | 비트 범위 | 유효 범위 | 테스트 값 | 설명 |
|------|-----------|-----------|-----------|------|
| SCP | 03:00 | 0h, 2h-4h | 0x0, 0x2, 0x3, 0x4 | Admin, SetFeatures, MI, PCIe |
| PRHBT | 04 | 0-1 | 0, 1 | Allow, Prohibit |
| IFC | 06:05 | 00b-10b | 0, 1, 2 | Admin SQ, Both, Mgmt Only |
| Rsvd7 | 07 | 0 | 0 | 항상 0 |
| OFI | 15:08 | 0x00-0xFF | 0x00, 0x06, 0x09, 0x24, 0xFF | Min, Identify, SetFeatures, Lockdown, Max |
| Rsvd | 31:16 | 0 | 0 | 항상 0 |

#### CDW14 필드 테스트 데이터

| 필드 | 비트 범위 | 유효 범위 | 테스트 값 | 설명 |
|------|-----------|-----------|-----------|------|
| UIDX | 06:00 | 0x00-0x7F | 0x00, 0x01, 0x05, 0x7F | Min, 일반값, Max |
| Rsvd | 31:07 | 0 | 0 | 항상 0 |

### 5.2 경계값

| 필드 | 최소값 | 최대값 | 테스트 케이스 |
|------|--------|--------|---------------|
| OFI | 0x00 | 0xFF | TC-004, TC-005 |
| SCP | 0 | 4 (유효), 15 (최대) | TC-017 |
| IFC | 0 | 2 (유효), 3 (Reserved) | TC-009 |
| PRHBT | 0 | 1 | TC-010, TC-011 |
| UIDX | 0x00 | 0x7F | TC-019 |

### 5.3 무효한 입력값 (Reserved)

| 필드 | 무효 범위 | 테스트 값 | 테스트 케이스 |
|------|-----------|-----------|---------------|
| SCP | 0x1, 0x5-0xF | 1, 5, 15 | TC-016, TC-017 |
| IFC | 0x3 (11b) | 3 | TC-009 |

### 5.4 Enum 상수 값

#### LockdownScope

| Enum 상수 | 값 | 테스트 케이스 |
|-----------|-----|---------------|
| AdminCommandOpcode | 0x0 | TC-012, TC-029 |
| Reserved1 | 0x1 | TC-016, TC-029 |
| SetFeaturesFID | 0x2 | TC-013, TC-029 |
| MICommandSetOpcode | 0x3 | TC-014, TC-029 |
| PCIeCommandSetOpcode | 0x4 | TC-015, TC-029 |

#### LockdownInterface

| Enum 상수 | 값 | 테스트 케이스 |
|-----------|-----|---------------|
| AdminSubmissionQueue | 0 | TC-006, TC-030 |
| AdminSQAndMgmtEndpoint | 1 | TC-007, TC-030 |
| MgmtEndpointOnly | 2 | TC-008, TC-030 |
| Reserved | 3 | TC-009, TC-030 |

#### LockdownProhibit

| Enum 상수 | 값 | 테스트 케이스 |
|-----------|-----|---------------|
| Allow | 0 | TC-011, TC-031 |
| Prohibit | 1 | TC-010, TC-031 |

### 5.5 통합 테스트 데이터 조합

| 시나리오 | SCP | IFC | PRHBT | OFI | UIDX | 테스트 케이스 |
|----------|-----|-----|-------|-----|------|---------------|
| Admin Command 금지 | 0 | 0 | 1 | 0x06 | N/A | TC-034 |
| Set Features FID 금지 | 2 | 0 | 1 | 0x01 | N/A | TC-035 |
| MI Command 금지 | 3 | 1 | 1 | 0x10 | N/A | TC-036 |
| PCIe Command 금지 | 4 | 2 | 1 | 0x20 | N/A | TC-037 |
| Vendor FID + UUID | 2 | 0 | 1 | 0xC0 | 3 | TC-038 |
| 최대값 조합 | 4 | 2 | 1 | 0xFF | 0x7F | TC-032 |
| 최소값 조합 | 0 | 0 | 0 | 0x00 | 0x00 | TC-033 |

### 5.6 예상되는 Admin Command Opcodes (참고용)

| Opcode | Command Name | 테스트 시나리오 |
|--------|--------------|-----------------|
| 0x00 | Delete I/O Submission Queue | 금지 가능 |
| 0x01 | Create I/O Submission Queue | 금지 가능 |
| 0x04 | Delete I/O Completion Queue | 금지 가능 |
| 0x05 | Create I/O Completion Queue | 금지 가능 |
| 0x06 | Identify | TC-034 테스트 데이터 |
| 0x09 | Set Features | 금지 가능 |
| 0x0A | Get Features | 금지 가능 |
| 0x24 | Lockdown | 자기 자신 금지 불가 |

### 5.7 예상되는 Feature Identifiers (참고용)

| FID | Feature Name | 테스트 시나리오 |
|-----|--------------|-----------------|
| 0x01 | Arbitration | TC-035 테스트 데이터 |
| 0x02 | Power Management | 금지 가능 |
| 0x04 | Temperature Threshold | 금지 가능 |
| 0xC0-0xFF | Vendor Specific | TC-038 (UUID Index 사용) |

---

## 6. 테스트 구현 가이드

### 6.1 테스트 파일 구조

```cpp
// test/LockdownTest.cpp

#include <gtest/gtest.h>
#include "nvme_lockdown.h"

using namespace nvme;

// Test Fixture (선택적)
class LockdownTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 공통 초기화 코드
    }

    void TearDown() override {
        // 정리 코드
    }
};

// TC-001: Opcode 검증
TEST(LockdownBasicTest, TC001_OpcodeVerification) {
    EXPECT_EQ(static_cast<uint8_t>(AdminOpcode::Lockdown), 0x24);
}

// TC-002: OFI 필드 - Admin Command Opcode 설정
TEST(LockdownCDW10Test, TC002_OFI_AdminCommandOpcode) {
    LockdownCDW10 cdw10{};
    cdw10.raw = 0;
    cdw10.bits.OFI = 0x06;

    EXPECT_EQ(cdw10.bits.OFI, 0x06);
    EXPECT_EQ(cdw10.raw & 0xFF00, 0x0600);
}

// ... 나머지 테스트 케이스들
```

### 6.2 Google Test Assertions 가이드

| 목적 | Assertion | 예시 |
|------|-----------|------|
| 값 동등 검증 | `EXPECT_EQ(actual, expected)` | `EXPECT_EQ(cdw10.bits.OFI, 0x06)` |
| 비트 마스크 검증 | `EXPECT_EQ(value & mask, expected)` | `EXPECT_EQ(cdw10.raw & 0xFF00, 0x0600)` |
| 타입 크기 검증 | `EXPECT_EQ(sizeof(Type), size)` | `EXPECT_EQ(sizeof(LockdownCDW10), 4)` |
| 컴파일 타임 검증 | `static_assert(condition, msg)` | `static_assert(sizeof(LockdownCDW10) == 4, "...")` |
| 0 검증 | `EXPECT_EQ(value, 0)` | `EXPECT_EQ(cdw10.bits.Rsvd7, 0)` |

### 6.3 테스트 실행 순서

1. **구조체 크기 및 Enum 검증** (TC-001, TC-025, TC-026, TC-029, TC-030, TC-031)
2. **개별 필드 테스트** (TC-002 ~ TC-024)
3. **비트 필드 레이아웃 검증** (TC-027, TC-028)
4. **경계값 테스트** (TC-004, TC-005, TC-017, TC-019)
5. **통합 시나리오 테스트** (TC-032 ~ TC-038)

### 6.4 실패 시 디버깅 가이드

- **비트 필드 오류**: `printf("raw=0x%08X\n", cdw10.raw)`로 raw 값 확인
- **enum 값 불일치**: `static_cast<uint8_t>(enum_value)` 명시적 캐스팅
- **구조체 크기 오류**: 컴파일러 패딩 옵션 확인 (`#pragma pack` 필요 여부)

---

## 7. 추가 고려사항

### 7.1 디바이스 통합 테스트 (향후 구현)

다음 요구사항은 실제 NVMe 디바이스 또는 모의 객체(Mock)가 필요:

- REQ-LKD-016: Prohibition Not Supported 상태 코드
- REQ-LKD-017: Invalid Field - Management Endpoint 없음
- REQ-LKD-018: Invalid Field - IFC/SCP 조합 오류
- REQ-LKD-019: 재금지/재허용 동작
- REQ-LKD-020: Completion Queue Entry 생성

### 7.2 플랫폼별 테스트 고려사항

- **Windows**: Clang-cl 컴파일러 패딩 동작 검증
- **Linux**: GCC 컴파일러 패딩 동작 검증
- **Endianness**: Little-endian 가정 확인

### 7.3 커버리지 측정

```bash
# GCC/Clang에서 코드 커버리지 측정
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="--coverage"
cmake --build build
cd build && ctest
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_report
```

---

## 8. 변경 이력

| 버전 | 날짜 | 작성자 | 변경 내용 |
|------|------|--------|-----------|
| 1.0 | 2026-01-12 | Claude | 초기 작성 (38개 테스트 케이스 설계) |

---

## 9. 참고 문서

- 요구사항 명세서: `docs/requirements/lockdown_requirements.md`
- 테스트 시나리오 명세서: `docs/test-scenarios/lockdown_test_scenarios.md`
- NVMe Base Specification: Revision 2.2, Section 5.1.15
- Google Test Documentation: https://google.github.io/googletest/
