# Firmware Image Download Command 요구사항 명세서

## 1. 기본 정보

| 항목 | 값 |
|------|-----|
| **Command Name** | Firmware Image Download |
| **Opcode** | 0x11 (Admin Command) |
| **Spec Section** | 5.1.9 |
| **Spec Version** | NVM Express Base Specification, Revision 2.2 |
| **Figure Numbers** | Figure 185, Figure 186, Figure 187, Figure 188 |

### 1.1 명령 개요

Firmware Image Download 명령은 향후 컨트롤러 업데이트를 위한 이미지의 전체 또는 일부를 다운로드하는 데 사용됩니다. 이 명령은 Admin Submission Queue 또는 I/O Submission Queue에서 다른 명령이 처리 중일 때도 제출될 수 있습니다.

이미지는 별도의 Firmware Image Download 명령으로 개별적으로 다운로드되는 여러 조각으로 구성될 수 있습니다. 각 명령에는 dword 범위를 지정하는 Dword Offset과 Number of Dwords가 포함됩니다. 펌웨어 조각은 순서에 관계없이 컨트롤러에 제출될 수 있습니다. 단, Boot Partition을 업데이트할 때는 이미지 조각을 순서대로 제출해야 합니다.

새 펌웨어 이미지는 Firmware Image Download 명령의 일부로 활성화되지 않습니다. 활성화를 위해서는 Firmware Commit 명령을 사용해야 합니다.

---

## 2. Command Dword 구조

### 2.1 Data Pointer (DPTR) - Figure 185

| Bits | Field | Description |
|------|-------|-------------|
| 127:00 | **DPTR** (Data Pointer) | 데이터가 전송되어야 하는 위치를 지정. Figure 92에서 이 필드의 정의 참조 |

### 2.2 Command Dword 10 (CDW10) - Figure 186

| Bits | Field | Description |
|------|-------|-------------|
| 31:00 | **NUMD** (Number of Dwords) | 이 펌웨어 부분에 대해 전송할 dword 수를 지정. 0's based 값임. FWUG 필드(Figure 313 참조)에 표시된 요구사항을 충족하지 않으면 컨트롤러가 Invalid Field in Command 상태 코드로 명령을 중단할 수 있음 |

### 2.3 Command Dword 11 (CDW11) - Figure 187

| Bits | Field | Description |
|------|-------|-------------|
| 31:00 | **OFST** (Offset) | 컨트롤러에 다운로드되는 펌웨어 이미지 시작 부분부터의 dword 오프셋을 지정. 오프셋은 펌웨어가 여러 조각으로 다운로드될 때 전체 펌웨어 이미지를 구성하는 데 사용됨. 펌웨어 이미지 시작에 해당하는 조각은 Offset이 0h여야 함. FWUG 필드(Figure 313 참조)에 표시된 요구사항을 충족하지 않으면 컨트롤러가 Invalid Field in Command 상태 코드로 명령을 중단할 수 있음 |

---

## 3. Enum 정의

### 3.1 Admin Command Opcode

| Value | Name | Description |
|-------|------|-------------|
| 0x11 | FIRMWARE_IMAGE_DOWNLOAD | Firmware Image Download 명령 opcode |

---

## 4. Status Codes

### 4.1 Command Specific Status Values - Figure 188

| Value | Name | Description |
|-------|------|-------------|
| 14h | OVERLAPPING_RANGE | 컨트롤러가 펌웨어 이미지에 중복되는 범위를 감지했음. 이 오류는 다운로드된 펌웨어 이미지의 granularity 또는 alignment가 Identify Controller 데이터 구조에 표시된 Firmware Update Granularity 필드를 준수하지 않음을 나타낼 수 있음 |

### 4.2 Generic Status Codes (적용 가능)

| Value | Name | Condition |
|-------|------|-----------|
| 00h | SUCCESS | 명령이 성공적으로 완료됨 |
| 02h | INVALID_FIELD_IN_COMMAND | NUMD 또는 OFST 필드 값이 FWUG 필드 요구사항을 충족하지 않는 경우 |

---

## 5. 기능 요구사항

| ID | 요구사항 | 우선순위 | 스펙 참조 |
|----|----------|----------|-----------|
| REQ-FID-001 | Firmware Image Download 명령은 Admin Command opcode 0x11을 사용해야 한다 | P1 | Section 5.1.9 |
| REQ-FID-002 | Data Pointer(DPTR) 필드는 데이터가 전송될 위치를 지정해야 한다 | P1 | Figure 185 |
| REQ-FID-003 | CDW10의 NUMD 필드(bits 31:00)는 전송할 dword 수를 0's based 값으로 지정해야 한다 | P1 | Figure 186 |
| REQ-FID-004 | CDW11의 OFST 필드(bits 31:00)는 펌웨어 이미지 시작부터의 dword 오프셋을 지정해야 한다 | P1 | Figure 187 |
| REQ-FID-005 | 펌웨어 이미지 시작에 해당하는 조각은 OFST 값이 0h여야 한다 | P1 | Figure 187 |
| REQ-FID-006 | NUMD 필드 값이 FWUG 필드 요구사항을 충족하지 않으면 컨트롤러는 Invalid Field in Command 상태 코드로 명령을 중단할 수 있다 | P1 | Figure 186 |
| REQ-FID-007 | OFST 필드 값이 FWUG 필드 요구사항을 충족하지 않으면 컨트롤러는 Invalid Field in Command 상태 코드로 명령을 중단할 수 있다 | P1 | Figure 187 |
| REQ-FID-008 | 이미지 조각들의 dword 범위가 중복되면 컨트롤러는 Overlapping Range 오류(14h)를 반환할 수 있다 | P1 | Figure 188 |
| REQ-FID-009 | Firmware Image Download 명령은 Admin Submission Queue 또는 I/O Submission Queue에 다른 명령이 처리 중일 때도 제출될 수 있어야 한다 | P1 | Section 5.1.9 |
| REQ-FID-010 | 펌웨어 이미지는 여러 조각으로 나누어 개별적인 Firmware Image Download 명령으로 다운로드될 수 있어야 한다 | P1 | Section 5.1.9 |
| REQ-FID-011 | 펌웨어 조각은 컨트롤러에 순서에 관계없이 제출될 수 있어야 한다 | P2 | Section 5.1.9 |
| REQ-FID-012 | Boot Partition 업데이트 시에는 이미지 조각을 순서대로 제출해야 한다 | P2 | Section 5.1.9 |
| REQ-FID-013 | 새 펌웨어 이미지는 Firmware Image Download 명령의 일부로 활성화되지 않아야 한다 | P1 | Section 5.1.9 |
| REQ-FID-014 | 이미지 다운로드 후 다른 이미지를 다운로드하기 전에 Firmware Commit 명령을 발행해야 한다 | P1 | Section 5.1.9 |
| REQ-FID-015 | Firmware Commit 명령 완료 후 첫 번째 Firmware Image Download 명령 처리 시 컨트롤러는 기존에 다운로드된 이미지 조각들을 모두 폐기해야 한다 | P1 | Section 5.1.9 |
| REQ-FID-016 | 펌웨어 다운로드와 Firmware Commit 명령 완료 사이에 Controller Level Reset이 발생하면 컨트롤러는 다운로드된 이미지 조각들을 모두 폐기해야 한다 | P1 | Section 5.1.9 |
| REQ-FID-017 | 호스트 소프트웨어는 이미지 조각들의 dword 범위가 중복되지 않도록 해야 한다 | P2 | Section 5.1.9 |
| REQ-FID-018 | 호스트 소프트웨어는 NUMD 및 OFST 필드가 FWUG 필드에 표시된 alignment 및 granularity 요구사항을 충족하도록 해야 한다 | P2 | Section 5.1.9 |
| REQ-FID-019 | Firmware Image Download 명령 완료 시 컨트롤러는 Admin Completion Queue에 completion queue entry를 post해야 한다 | P1 | Section 5.1.9 |
| REQ-FID-020 | 호스트 소프트웨어는 Boot Partition 및/또는 펌웨어 이미지를 업데이트하는 명령 시퀀스가 중복되지 않도록 해야 한다 | P2 | Section 5.1.9 |
| REQ-FID-021 | 컨트롤러가 둘 이상의 펌웨어 이미지 및/또는 Boot Partition의 중복 업데이트 명령 시퀀스를 감지하면 결과를 completion queue entry의 Dword 0에 보고해야 한다 | P2 | Section 5.1.9 |

---

## 6. 구현 참고사항

### 6.1 C++ 구조체 설계

```cpp
// CDW10 Union 구조 - Figure 186
union FirmwareImageDownloadCDW10 {
    struct {
        uint32_t NUMD : 32;   // Bits 31:00 - Number of Dwords (0's based)
    } bits;
    uint32_t raw;
};

// CDW11 Union 구조 - Figure 187
union FirmwareImageDownloadCDW11 {
    struct {
        uint32_t OFST : 32;   // Bits 31:00 - Offset (dword offset)
    } bits;
    uint32_t raw;
};
```

### 6.2 Enum 정의

```cpp
enum class AdminOpcode : uint8_t {
    // ... other opcodes
    FirmwareImageDownload = 0x11,
    // ... other opcodes
};

enum class FirmwareImageDownloadStatusCode : uint8_t {
    Success = 0x00,
    InvalidFieldInCommand = 0x02,
    OverlappingRange = 0x14,
};
```

### 6.3 주요 구현 고려사항

1. **데이터 전송 크기**: NUMD는 0's based 값이므로 실제 전송 dword 수 = NUMD + 1
2. **실제 전송 바이트 수**: (NUMD + 1) * 4 바이트
3. **오프셋 계산**: 실제 바이트 오프셋 = OFST * 4
4. **FWUG 검증**: Identify Controller 데이터의 FWUG 필드를 참조하여 granularity 요구사항 검증
5. **중복 범위 검증**: 다운로드된 조각들의 범위를 추적하여 중복 감지

---

## 7. 변경 이력

| 버전 | 날짜 | 작성자 | 변경 내용 |
|------|------|--------|-----------|
| 1.0 | 2026-01-13 | Claude | 초기 작성 |
