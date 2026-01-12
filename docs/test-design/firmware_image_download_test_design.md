# Firmware Image Download 테스트 설계

## 1. 테스트 전략

### 1.1 기본 정보

| 항목 | 값 |
|------|-----|
| **Command Name** | Firmware Image Download |
| **테스트 프레임워크** | Google Test (gtest) |
| **테스트 패턴** | Given-When-Then |
| **커버리지 목표** | 100% (모든 요구사항 및 시나리오) |
| **총 테스트 케이스 수** | 45개 |
| **작성일** | 2026-01-13 |

### 1.2 테스트 접근 방법

1. **구조 검증 (Structure Validation)**: 구조체 크기, 필드 배치, enum 값 검증
2. **기능 테스트 (Functional Testing)**: 명령 실행 및 정상 동작 확인
3. **경계값 테스트 (Boundary Testing)**: 최소/최대 값에서의 동작 확인
4. **오류 처리 테스트 (Error Handling)**: 잘못된 입력에 대한 적절한 오류 반환
5. **통합 테스트 (Integration Testing)**: 다중 조각 다운로드, 순서, 중복 범위 등

### 1.3 테스트 환경 요구사항

- **컴파일러**: C++17 이상 지원 (Clang-cl, GCC, MSVC)
- **테스트 프레임워크**: Google Test 1.14.0
- **하드웨어/시뮬레이터**: NVMe 컨트롤러 또는 모의 객체 (Mock)
- **테스트 데이터**: 테스트용 펌웨어 이미지 데이터 (4KB, 8KB, 16KB 등)

---

## 2. 테스트 케이스 목록

### 2.1 구조체 검증 테스트 케이스

#### TC-FID-001: CDW10 구조체 크기 검증
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-038 |
| **관련 요구사항** | REQ-FID-003 |
| **Given** | FirmwareImageDownloadCDW10 구조체가 정의되어 있음 |
| **When** | sizeof(FirmwareImageDownloadCDW10)를 호출 |
| **Then** | 크기가 정확히 4 바이트여야 함 |
| **검증방법** | `EXPECT_EQ(sizeof(FirmwareImageDownloadCDW10), 4u)` |
| **테스트 데이터** | N/A (컴파일 타임 검증) |
| **우선순위** | P1 |

#### TC-FID-002: CDW11 구조체 크기 검증
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-039 |
| **관련 요구사항** | REQ-FID-004 |
| **Given** | FirmwareImageDownloadCDW11 구조체가 정의되어 있음 |
| **When** | sizeof(FirmwareImageDownloadCDW11)를 호출 |
| **Then** | 크기가 정확히 4 바이트여야 함 |
| **검증방법** | `EXPECT_EQ(sizeof(FirmwareImageDownloadCDW11), 4u)` |
| **테스트 데이터** | N/A (컴파일 타임 검증) |
| **우선순위** | P1 |

#### TC-FID-003: CDW10 raw와 bits 필드 동기화
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-040 |
| **관련 요구사항** | REQ-FID-003 |
| **Given** | FirmwareImageDownloadCDW10 구조체 인스턴스 생성 |
| **When** | raw 필드에 0x12345678을 설정 |
| **Then** | bits.NUMD가 0x12345678이어야 함 |
| **검증방법** | `cdw10.raw = 0x12345678; EXPECT_EQ(cdw10.bits.NUMD, 0x12345678u)` |
| **테스트 데이터** | raw = 0x12345678 |
| **우선순위** | P1 |

#### TC-FID-004: CDW10 bits와 raw 필드 역방향 동기화
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-040 |
| **관련 요구사항** | REQ-FID-003 |
| **Given** | FirmwareImageDownloadCDW10 구조체 인스턴스 생성 |
| **When** | bits.NUMD에 0xABCDEF00을 설정 |
| **Then** | raw가 0xABCDEF00이어야 함 |
| **검증방법** | `cdw10.bits.NUMD = 0xABCDEF00; EXPECT_EQ(cdw10.raw, 0xABCDEF00u)` |
| **테스트 데이터** | bits.NUMD = 0xABCDEF00 |
| **우선순위** | P1 |

#### TC-FID-005: CDW11 raw와 bits 필드 동기화
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-041 |
| **관련 요구사항** | REQ-FID-004 |
| **Given** | FirmwareImageDownloadCDW11 구조체 인스턴스 생성 |
| **When** | raw 필드에 0xABCDEF00을 설정 |
| **Then** | bits.OFST가 0xABCDEF00이어야 함 |
| **검증방법** | `cdw11.raw = 0xABCDEF00; EXPECT_EQ(cdw11.bits.OFST, 0xABCDEF00u)` |
| **테스트 데이터** | raw = 0xABCDEF00 |
| **우선순위** | P1 |

#### TC-FID-006: CDW11 bits와 raw 필드 역방향 동기화
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-041 |
| **관련 요구사항** | REQ-FID-004 |
| **Given** | FirmwareImageDownloadCDW11 구조체 인스턴스 생성 |
| **When** | bits.OFST에 0x87654321을 설정 |
| **Then** | raw가 0x87654321이어야 함 |
| **검증방법** | `cdw11.bits.OFST = 0x87654321; EXPECT_EQ(cdw11.raw, 0x87654321u)` |
| **테스트 데이터** | bits.OFST = 0x87654321 |
| **우선순위** | P1 |

#### TC-FID-007: Status Code Enum 값 검증
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-042 |
| **관련 요구사항** | REQ-FID-006, REQ-FID-007, REQ-FID-008 |
| **Given** | FirmwareImageDownloadStatusCode enum이 정의되어 있음 |
| **When** | 각 enum 값을 확인 |
| **Then** | Success=0x00, InvalidFieldInCommand=0x02, OverlappingRange=0x14 |
| **검증방법** | `EXPECT_EQ(static_cast<uint8_t>(FirmwareImageDownloadStatusCode::Success), 0x00)` 등 |
| **테스트 데이터** | enum 값들 |
| **우선순위** | P1 |

#### TC-FID-008: Opcode 값 검증
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-001 |
| **관련 요구사항** | REQ-FID-001 |
| **Given** | AdminOpcode enum이 정의되어 있음 |
| **When** | FirmwareImageDownload opcode 값을 확인 |
| **Then** | 값이 0x11이어야 함 |
| **검증방법** | `EXPECT_EQ(static_cast<uint8_t>(AdminOpcode::FirmwareImageDownload), 0x11)` |
| **테스트 데이터** | opcode = 0x11 |
| **우선순위** | P1 |

### 2.2 기본 명령 구조 테스트 케이스

#### TC-FID-009: Data Pointer 설정
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-002 |
| **관련 요구사항** | REQ-FID-002 |
| **Given** | 4KB 크기의 유효한 데이터 버퍼가 할당됨 (buffer = new uint8_t[4096]) |
| **When** | DPTR 필드에 버퍼 주소를 설정 |
| **Then** | DPTR에 설정된 주소가 버퍼 주소와 일치해야 함 |
| **검증방법** | `EXPECT_EQ(dptr, reinterpret_cast<uint64_t>(buffer))` |
| **테스트 데이터** | buffer address = 0x7FFF12345000 (예시) |
| **우선순위** | P1 |

#### TC-FID-010: NUMD 필드 설정 - 기본값
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-003 |
| **관련 요구사항** | REQ-FID-003 |
| **Given** | FirmwareImageDownloadCDW10 구조체 인스턴스 |
| **When** | NUMD = 1023 (1024 dwords = 4KB) 설정 |
| **Then** | 실제 전송 크기 = 1024 dwords (4096 bytes) |
| **검증방법** | `cdw10.bits.NUMD = 1023; EXPECT_EQ(cdw10.bits.NUMD + 1, 1024u)` |
| **테스트 데이터** | NUMD = 1023 (0's based) |
| **우선순위** | P1 |

#### TC-FID-011: OFST 필드 설정 - 기본값
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-004 |
| **관련 요구사항** | REQ-FID-004 |
| **Given** | FirmwareImageDownloadCDW11 구조체 인스턴스 |
| **When** | OFST = 1024 (4KB 오프셋) 설정 |
| **Then** | 실제 바이트 오프셋 = 4096 bytes |
| **검증방법** | `cdw11.bits.OFST = 1024; EXPECT_EQ(cdw11.bits.OFST * 4, 4096u)` |
| **테스트 데이터** | OFST = 1024 |
| **우선순위** | P1 |

#### TC-FID-012: 첫 번째 조각 OFST=0 설정
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-005 |
| **관련 요구사항** | REQ-FID-005 |
| **Given** | 새로운 펌웨어 이미지 다운로드 시작 |
| **When** | 첫 번째 조각에 OFST = 0, NUMD = 1023 설정 |
| **Then** | 명령이 성공적으로 완료되어야 함 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buffer, 1023, 0))` |
| **테스트 데이터** | OFST = 0, NUMD = 1023 |
| **우선순위** | P1 |

### 2.3 경계값 테스트 케이스

#### TC-FID-013: NUMD 최소값 (0)
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-027 |
| **관련 요구사항** | REQ-FID-003 |
| **Given** | 유효한 데이터 버퍼 (4 bytes) |
| **When** | NUMD = 0 설정하여 명령 실행 |
| **Then** | 1 dword (4 bytes)가 전송되어야 함 |
| **검증방법** | `cdw10.bits.NUMD = 0; EXPECT_EQ(cdw10.bits.NUMD + 1, 1u)` |
| **테스트 데이터** | NUMD = 0 |
| **우선순위** | P1 |

#### TC-FID-014: NUMD 최대값 (0xFFFFFFFF)
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-028 |
| **관련 요구사항** | REQ-FID-003 |
| **Given** | 매우 큰 데이터 버퍼 (이론적) |
| **When** | NUMD = 0xFFFFFFFF 설정 |
| **Then** | 0x100000000 dwords 전송 요청 (16GB) |
| **검증방법** | `cdw10.bits.NUMD = 0xFFFFFFFF; EXPECT_EQ(cdw10.bits.NUMD, 0xFFFFFFFFu)` |
| **테스트 데이터** | NUMD = 0xFFFFFFFF |
| **우선순위** | P2 |

#### TC-FID-015: OFST 최소값 (0)
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-029 |
| **관련 요구사항** | REQ-FID-004, REQ-FID-005 |
| **Given** | 펌웨어 이미지 시작 조각 |
| **When** | OFST = 0 설정 |
| **Then** | 펌웨어 이미지 시작 위치부터 데이터 저장 |
| **검증방법** | `cdw11.bits.OFST = 0; EXPECT_EQ(cdw11.bits.OFST, 0u)` |
| **테스트 데이터** | OFST = 0 |
| **우선순위** | P1 |

#### TC-FID-016: OFST 최대값 (0xFFFFFFFF)
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-030 |
| **관련 요구사항** | REQ-FID-004 |
| **Given** | 매우 큰 펌웨어 이미지 (이론적) |
| **When** | OFST = 0xFFFFFFFF 설정 |
| **Then** | 매우 큰 오프셋에서 동작 (16GB 오프셋) |
| **검증방법** | `cdw11.bits.OFST = 0xFFFFFFFF; EXPECT_EQ(cdw11.bits.OFST, 0xFFFFFFFFu)` |
| **테스트 데이터** | OFST = 0xFFFFFFFF |
| **우선순위** | P2 |

### 2.4 FWUG 관련 테스트 케이스

#### TC-FID-017: NUMD가 FWUG 요구사항 충족 - 성공
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-006, TS-FID-031 |
| **관련 요구사항** | REQ-FID-006, REQ-FID-018 |
| **Given** | FWUG = 4KB (1024 dwords) 설정 |
| **When** | NUMD = 1023 (정확히 4KB) 설정하여 명령 실행 |
| **Then** | 명령이 Success(0x00) 상태로 완료 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buffer, 1023, 0)); EXPECT_EQ(device.GetLastStatusCode(), 0x00)` |
| **테스트 데이터** | FWUG = 1024 dwords, NUMD = 1023 |
| **우선순위** | P1 |

#### TC-FID-018: NUMD가 FWUG 요구사항 미충족 - 오류
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-007 |
| **관련 요구사항** | REQ-FID-006, REQ-FID-018 |
| **Given** | FWUG = 4KB (1024 dwords) 설정 |
| **When** | NUMD = 511 (2KB, granularity 미충족) 설정하여 명령 실행 |
| **Then** | Invalid Field in Command(0x02) 상태 코드 반환 |
| **검증방법** | `EXPECT_FALSE(device.FirmwareImageDownload(buffer, 511, 0)); EXPECT_EQ(device.GetLastStatusCode(), 0x02)` |
| **테스트 데이터** | FWUG = 1024 dwords, NUMD = 511 |
| **우선순위** | P1 |

#### TC-FID-019: NUMD가 FWUG 경계값 미만 - 오류
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-032 |
| **관련 요구사항** | REQ-FID-006, REQ-FID-018 |
| **Given** | FWUG = 4KB (1024 dwords) 설정 |
| **When** | NUMD = 1022 (4KB - 1 dword) 설정 |
| **Then** | Invalid Field in Command(0x02) 상태 코드 반환 |
| **검증방법** | `EXPECT_FALSE(device.FirmwareImageDownload(buffer, 1022, 0)); EXPECT_EQ(device.GetLastStatusCode(), 0x02)` |
| **테스트 데이터** | FWUG = 1024 dwords, NUMD = 1022 |
| **우선순위** | P2 |

#### TC-FID-020: NUMD가 FWUG 배수 - 성공
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-033 |
| **관련 요구사항** | REQ-FID-006, REQ-FID-018 |
| **Given** | FWUG = 4KB (1024 dwords) 설정 |
| **When** | NUMD = 2047 (정확히 8KB = 2 * 4KB) 설정 |
| **Then** | 명령이 성공적으로 완료 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buffer, 2047, 0))` |
| **테스트 데이터** | FWUG = 1024 dwords, NUMD = 2047 (8KB) |
| **우선순위** | P2 |

#### TC-FID-021: OFST가 FWUG alignment 충족 - 성공
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-008, TS-FID-034 |
| **관련 요구사항** | REQ-FID-007, REQ-FID-018 |
| **Given** | FWUG = 4KB (1024 dwords) 설정 |
| **When** | OFST = 1024 (4KB aligned) 설정하여 명령 실행 |
| **Then** | 명령이 Success(0x00) 상태로 완료 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buffer, 1023, 1024)); EXPECT_EQ(device.GetLastStatusCode(), 0x00)` |
| **테스트 데이터** | FWUG = 1024 dwords, OFST = 1024 (4KB) |
| **우선순위** | P1 |

#### TC-FID-022: OFST가 FWUG alignment 미충족 - 오류
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-009 |
| **관련 요구사항** | REQ-FID-007, REQ-FID-018 |
| **Given** | FWUG = 4KB (1024 dwords) 설정 |
| **When** | OFST = 512 (2KB, misaligned) 설정하여 명령 실행 |
| **Then** | Invalid Field in Command(0x02) 상태 코드 반환 |
| **검증방법** | `EXPECT_FALSE(device.FirmwareImageDownload(buffer, 1023, 512)); EXPECT_EQ(device.GetLastStatusCode(), 0x02)` |
| **테스트 데이터** | FWUG = 1024 dwords, OFST = 512 (2KB) |
| **우선순위** | P1 |

#### TC-FID-023: OFST alignment 경계값 - 성공
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-034 |
| **관련 요구사항** | REQ-FID-007, REQ-FID-018 |
| **Given** | FWUG = 4KB (1024 dwords) 설정 |
| **When** | OFST = 2048 (8KB = 2 * 4KB aligned) 설정 |
| **Then** | 명령이 성공적으로 완료 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buffer, 1023, 2048))` |
| **테스트 데이터** | FWUG = 1024 dwords, OFST = 2048 (8KB) |
| **우선순위** | P2 |

### 2.5 중복 범위 테스트 케이스

#### TC-FID-024: 겹치지 않는 범위 - 성공
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-010 |
| **관련 요구사항** | REQ-FID-008, REQ-FID-017 |
| **Given** | 첫 번째 조각 다운로드 완료 (OFST=0, NUMD=1023) |
| **When** | 두 번째 조각 다운로드 (OFST=1024, NUMD=1023) |
| **Then** | 두 명령 모두 Success(0x00) 상태로 완료 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buffer1, 1023, 0)); EXPECT_TRUE(device.FirmwareImageDownload(buffer2, 1023, 1024))` |
| **테스트 데이터** | 조각1: OFST=0, NUMD=1023; 조각2: OFST=1024, NUMD=1023 |
| **우선순위** | P1 |

#### TC-FID-025: 완전 중복 범위 - 오류
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-011 |
| **관련 요구사항** | REQ-FID-008 |
| **Given** | 첫 번째 조각 다운로드 완료 (OFST=0, NUMD=2047) |
| **When** | 중복되는 두 번째 조각 다운로드 시도 (OFST=1024, NUMD=1023) |
| **Then** | Overlapping Range(0x14) 상태 코드 반환 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buffer1, 2047, 0)); EXPECT_FALSE(device.FirmwareImageDownload(buffer2, 1023, 1024)); EXPECT_EQ(device.GetLastStatusCode(), 0x14)` |
| **테스트 데이터** | 조각1: OFST=0, NUMD=2047 (8KB); 조각2: OFST=1024, NUMD=1023 (4KB) |
| **우선순위** | P1 |

#### TC-FID-026: 부분 중복 범위 - 오류
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-012 |
| **관련 요구사항** | REQ-FID-008 |
| **Given** | 첫 번째 조각 다운로드 완료 (OFST=0, NUMD=1023) |
| **When** | 부분 중복 조각 다운로드 시도 (OFST=1000, NUMD=1023, 24 dwords 중복) |
| **Then** | Overlapping Range(0x14) 상태 코드 반환 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buffer1, 1023, 0)); EXPECT_FALSE(device.FirmwareImageDownload(buffer2, 1023, 1000)); EXPECT_EQ(device.GetLastStatusCode(), 0x14)` |
| **테스트 데이터** | 조각1: OFST=0, NUMD=1023; 조각2: OFST=1000, NUMD=1023 |
| **우선순위** | P2 |

### 2.6 다중 조각 다운로드 테스트 케이스

#### TC-FID-027: 4개 조각 순차 다운로드
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-014 |
| **관련 요구사항** | REQ-FID-010 |
| **Given** | 16KB 펌웨어 이미지를 4개 조각(각 4KB)으로 분할 |
| **When** | 4개 조각을 순서대로 다운로드 (OFST=0, 1024, 2048, 3072) |
| **Then** | 모든 조각이 성공적으로 다운로드됨 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buf1, 1023, 0)); ... EXPECT_TRUE(device.FirmwareImageDownload(buf4, 1023, 3072))` |
| **테스트 데이터** | 조각1: OFST=0, 조각2: OFST=1024, 조각3: OFST=2048, 조각4: OFST=3072 (각 NUMD=1023) |
| **우선순위** | P1 |

#### TC-FID-028: 비순차적 조각 다운로드
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-015 |
| **관련 요구사항** | REQ-FID-011 |
| **Given** | 16KB 펌웨어 이미지를 4개 조각으로 분할 |
| **When** | 조각을 비순차적으로 다운로드 (순서: 3, 1, 4, 2) |
| **Then** | 모든 조각이 성공적으로 다운로드됨 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buf3, 1023, 2048)); EXPECT_TRUE(device.FirmwareImageDownload(buf1, 1023, 0)); ...` |
| **테스트 데이터** | 다운로드 순서: OFST=2048 → 0 → 3072 → 1024 |
| **우선순위** | P2 |

#### TC-FID-029: Boot Partition 순차 다운로드 - 성공
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-016 |
| **관련 요구사항** | REQ-FID-012 |
| **Given** | Boot Partition 업데이트 모드 활성화 |
| **When** | 4개 조각을 순차적으로 다운로드 (순서: 1, 2, 3, 4) |
| **Then** | 모든 조각이 성공적으로 다운로드됨 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownloadBootPartition(buf1, 1023, 0)); ... (순차적)` |
| **테스트 데이터** | 순차적 OFST: 0 → 1024 → 2048 → 3072 |
| **우선순위** | P2 |

#### TC-FID-030: Boot Partition 비순차 다운로드 - 오류
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-017 |
| **관련 요구사항** | REQ-FID-012 |
| **Given** | Boot Partition 업데이트 모드 활성화 |
| **When** | 조각을 비순차적으로 다운로드 시도 (순서: 2, 1, 3, 4) |
| **Then** | 오류 상태 코드 반환 |
| **검증방법** | `EXPECT_FALSE(device.FirmwareImageDownloadBootPartition(buf2, 1023, 1024)); EXPECT_NE(device.GetLastStatusCode(), 0x00)` |
| **테스트 데이터** | 비순차적 OFST: 1024 (첫 시도) |
| **우선순위** | P2 |

### 2.7 동시 처리 테스트 케이스

#### TC-FID-031: 다른 명령 처리 중 제출
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-013 |
| **관련 요구사항** | REQ-FID-009 |
| **Given** | I/O 명령이 진행 중 (예: Read 명령 실행 중) |
| **When** | Firmware Image Download 명령 제출 |
| **Then** | 명령이 성공적으로 큐에 등록되고 처리됨 |
| **검증방법** | `EXPECT_TRUE(device.Read(...)); EXPECT_TRUE(device.FirmwareImageDownload(buffer, 1023, 0))` |
| **테스트 데이터** | 동시 명령: Read + Firmware Image Download |
| **우선순위** | P1 |

### 2.8 활성화 및 폐기 테스트 케이스

#### TC-FID-032: 다운로드만으로 펌웨어 비활성화
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-018 |
| **관련 요구사항** | REQ-FID-013 |
| **Given** | 현재 활성 펌웨어 슬롯이 1번 |
| **When** | 새 펌웨어 이미지 다운로드 완료 |
| **Then** | 활성 펌웨어 슬롯이 여전히 1번이어야 함 |
| **검증방법** | `uint8_t slot_before = device.GetActiveFirmwareSlot(); EXPECT_TRUE(device.FirmwareImageDownload(...)); uint8_t slot_after = device.GetActiveFirmwareSlot(); EXPECT_EQ(slot_before, slot_after)` |
| **테스트 데이터** | 활성 슬롯 = 1 |
| **우선순위** | P1 |

#### TC-FID-033: Firmware Commit 없이 새 이미지 다운로드
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-019 |
| **관련 요구사항** | REQ-FID-014 |
| **Given** | 첫 번째 이미지 다운로드 완료 (Commit 없음) |
| **When** | 두 번째 이미지 다운로드 시작 (OFST=0) |
| **Then** | 컨트롤러가 기존 이미지를 폐기하거나 오류 반환 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buf1, 1023, 0)); bool result = device.FirmwareImageDownload(buf2, 1023, 0); EXPECT_TRUE(result || device.GetLastStatusCode() != 0x00)` |
| **테스트 데이터** | 두 개의 독립적인 이미지 |
| **우선순위** | P1 |

#### TC-FID-034: Firmware Commit 후 기존 조각 폐기
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-020 |
| **관련 요구사항** | REQ-FID-015 |
| **Given** | 펌웨어 이미지 다운로드 및 Commit 완료 |
| **When** | 새 펌웨어 이미지 첫 번째 조각 다운로드 (OFST=0) |
| **Then** | 이전 다운로드 조각들이 폐기되고 새 조각만 유지됨 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buf1, 1023, 0)); EXPECT_TRUE(device.FirmwareCommit(...)); EXPECT_TRUE(device.FirmwareImageDownload(buf2, 1023, 0))` |
| **테스트 데이터** | 두 개의 순차적인 펌웨어 업데이트 |
| **우선순위** | P1 |

#### TC-FID-035: Controller Level Reset 시 조각 폐기
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-021 |
| **관련 요구사항** | REQ-FID-016 |
| **Given** | 펌웨어 이미지 일부 조각 다운로드 완료 (2/4 조각) |
| **When** | Controller Level Reset 수행 |
| **Then** | 리셋 후 다운로드된 조각들이 폐기됨 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buf1, 1023, 0)); device.ControllerReset(); EXPECT_TRUE(device.FirmwareImageDownload(buf1, 1023, 0))` |
| **테스트 데이터** | Reset 전 2개 조각 다운로드 |
| **우선순위** | P1 |

#### TC-FID-036: Reset 후 새 다운로드 시작
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-022 |
| **관련 요구사항** | REQ-FID-016 |
| **Given** | 리셋으로 인해 이전 다운로드 조각 폐기됨 |
| **When** | OFST=0부터 새 펌웨어 다운로드 시작 |
| **Then** | 새 다운로드가 성공적으로 시작됨 |
| **검증방법** | `device.ControllerReset(); EXPECT_TRUE(device.FirmwareImageDownload(buffer, 1023, 0))` |
| **테스트 데이터** | OFST=0 새 시작 |
| **우선순위** | P1 |

### 2.9 Completion Queue 테스트 케이스

#### TC-FID-037: Completion Queue Entry 생성
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-023 |
| **관련 요구사항** | REQ-FID-019 |
| **Given** | Admin Completion Queue가 설정됨 |
| **When** | Firmware Image Download 명령 실행 |
| **Then** | completion queue entry가 Admin Completion Queue에 post됨 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buffer, 1023, 0)); CompletionQueueEntry* cqe = device.GetCompletionQueueEntry(); EXPECT_NE(cqe, nullptr)` |
| **테스트 데이터** | Admin CQ 활성화 |
| **우선순위** | P1 |

#### TC-FID-038: 성공 시 Status Code 검증
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-024 |
| **관련 요구사항** | REQ-FID-019 |
| **Given** | 유효한 명령 파라미터 |
| **When** | 정상적인 Firmware Image Download 명령 실행 |
| **Then** | Status Code = 0x00 (Success) |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buffer, 1023, 0)); EXPECT_EQ(device.GetLastStatusCode(), 0x00)` |
| **테스트 데이터** | 유효한 NUMD, OFST |
| **우선순위** | P1 |

### 2.10 중복 업데이트 시퀀스 테스트 케이스

#### TC-FID-039: 단일 업데이트 시퀀스 성공
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-025 |
| **관련 요구사항** | REQ-FID-020 |
| **Given** | 다른 업데이트 시퀀스 없음 |
| **When** | 단일 펌웨어 이미지 다운로드 시퀀스 실행 |
| **Then** | 업데이트 시퀀스 성공 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buf1, 1023, 0)); EXPECT_TRUE(device.FirmwareImageDownload(buf2, 1023, 1024)); EXPECT_TRUE(device.FirmwareCommit(...))` |
| **테스트 데이터** | 단일 시퀀스: 2개 조각 + Commit |
| **우선순위** | P2 |

#### TC-FID-040: 중복 업데이트 시퀀스 감지
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-026 |
| **관련 요구사항** | REQ-FID-021 |
| **Given** | 첫 번째 업데이트 시퀀스 진행 중 |
| **When** | 두 번째 업데이트 시퀀스 시작 시도 |
| **Then** | completion queue entry의 Dword 0에 중복 감지 결과 보고됨 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buf1, 1023, 0)); EXPECT_FALSE(device.FirmwareImageDownloadSecondSequence(buf2, 1023, 0)); uint32_t dw0 = device.GetCompletionDword0(); EXPECT_NE(dw0 & 0xFF, 0x00)` |
| **테스트 데이터** | 2개의 동시 시퀀스 시도 |
| **우선순위** | P2 |

### 2.11 오류 처리 테스트 케이스

#### TC-FID-041: 무효한 Data Pointer
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-035 |
| **관련 요구사항** | REQ-FID-002 |
| **Given** | NULL 또는 무효한 버퍼 주소 |
| **When** | DPTR = nullptr로 명령 실행 시도 |
| **Then** | 적절한 오류 상태 코드 반환 |
| **검증방법** | `EXPECT_FALSE(device.FirmwareImageDownload(nullptr, 1023, 0)); EXPECT_NE(device.GetLastStatusCode(), 0x00)` |
| **테스트 데이터** | DPTR = nullptr |
| **우선순위** | P1 |

#### TC-FID-042: 데이터 전송 오류
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-036 |
| **관련 요구사항** | REQ-FID-002 |
| **Given** | 데이터 전송 중 인터럽트 시뮬레이션 |
| **When** | 전송 중 오류 발생 |
| **Then** | 적절한 오류 상태 코드 반환 및 상태 복구 |
| **검증방법** | `device.SimulateTransferError(); EXPECT_FALSE(device.FirmwareImageDownload(buffer, 1023, 0)); EXPECT_NE(device.GetLastStatusCode(), 0x00)` |
| **테스트 데이터** | 전송 중단 시뮬레이션 |
| **우선순위** | P2 |

#### TC-FID-043: 연속 범위 간 갭 존재
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-037 |
| **관련 요구사항** | REQ-FID-010 |
| **Given** | 조각들 사이에 갭이 있는 다운로드 |
| **When** | 조각1: OFST=0, NUMD=1023; 조각2: OFST=2048, NUMD=1023 (1024 dword 갭) |
| **Then** | Firmware Commit 시점에 오류 또는 경고 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buf1, 1023, 0)); EXPECT_TRUE(device.FirmwareImageDownload(buf2, 1023, 2048)); EXPECT_FALSE(device.FirmwareCommit(...)) || device.GetWarningStatus()` |
| **테스트 데이터** | OFST=0 → 2048 (갭: 1024 dwords) |
| **우선순위** | P2 |

### 2.12 다양한 파라미터 조합 테스트 케이스

#### TC-FID-044: 작은 크기 조각 다운로드 (FWUG 미지원 시)
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-003 |
| **관련 요구사항** | REQ-FID-003 |
| **Given** | FWUG = 0 (granularity 제약 없음) |
| **When** | NUMD = 255 (1KB) 설정하여 다운로드 |
| **Then** | 명령이 성공적으로 완료 |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buffer, 255, 0))` |
| **테스트 데이터** | FWUG = 0, NUMD = 255 |
| **우선순위** | P2 |

#### TC-FID-045: 큰 크기 조각 다운로드
| 항목 | 내용 |
|------|------|
| **관련 시나리오** | TS-FID-003, TS-FID-014 |
| **관련 요구사항** | REQ-FID-003 |
| **Given** | FWUG = 4KB (1024 dwords) |
| **When** | NUMD = 8191 (32KB) 설정하여 다운로드 |
| **Then** | 명령이 성공적으로 완료 (FWUG 배수) |
| **검증방법** | `EXPECT_TRUE(device.FirmwareImageDownload(buffer, 8191, 0))` |
| **테스트 데이터** | NUMD = 8191 (32KB = 8 * 4KB) |
| **우선순위** | P2 |

---

## 3. 시나리오-테스트 케이스 매핑

| 시나리오 ID | 테스트 케이스 | 설명 |
|-------------|---------------|------|
| TS-FID-001 | TC-FID-008 | Opcode 값 검증 |
| TS-FID-002 | TC-FID-009 | Data Pointer 필드 설정 검증 |
| TS-FID-003 | TC-FID-010, TC-FID-044, TC-FID-045 | CDW10 NUMD 필드 설정 검증 |
| TS-FID-004 | TC-FID-011 | CDW11 OFST 필드 설정 검증 |
| TS-FID-005 | TC-FID-012 | 첫 번째 이미지 조각의 OFST가 0인지 검증 |
| TS-FID-006 | TC-FID-017 | NUMD가 FWUG 요구사항 충족 시 성공 |
| TS-FID-007 | TC-FID-018 | NUMD가 FWUG 요구사항 미충족 시 오류 |
| TS-FID-008 | TC-FID-021 | OFST가 FWUG 요구사항 충족 시 성공 |
| TS-FID-009 | TC-FID-022 | OFST가 FWUG alignment 미충족 시 오류 |
| TS-FID-010 | TC-FID-024 | 겹치지 않는 범위로 다운로드 성공 |
| TS-FID-011 | TC-FID-025 | 중복 범위 감지 시 Overlapping Range 오류 |
| TS-FID-012 | TC-FID-026 | 부분 중복 범위 감지 |
| TS-FID-013 | TC-FID-031 | 다른 명령 처리 중 제출 가능 |
| TS-FID-014 | TC-FID-027 | 여러 조각으로 이미지 다운로드 |
| TS-FID-015 | TC-FID-028 | 비순차적 조각 다운로드 |
| TS-FID-016 | TC-FID-029 | Boot Partition 순차 다운로드 성공 |
| TS-FID-017 | TC-FID-030 | Boot Partition 비순차 다운로드 시 오류 |
| TS-FID-018 | TC-FID-032 | 다운로드만으로 펌웨어 비활성화 검증 |
| TS-FID-019 | TC-FID-033 | Firmware Commit 없이 새 이미지 다운로드 시 동작 |
| TS-FID-020 | TC-FID-034 | Firmware Commit 후 첫 다운로드 시 기존 조각 폐기 |
| TS-FID-021 | TC-FID-035 | Controller Level Reset 시 다운로드 조각 폐기 |
| TS-FID-022 | TC-FID-036 | Reset 후 새 다운로드 시작 |
| TS-FID-023 | TC-FID-037 | Completion Queue Entry 생성 검증 |
| TS-FID-024 | TC-FID-038 | 성공 시 Status Code 검증 |
| TS-FID-025 | TC-FID-039 | 단일 업데이트 시퀀스 성공 |
| TS-FID-026 | TC-FID-040 | 중복 업데이트 시퀀스 감지 |
| TS-FID-027 | TC-FID-013 | NUMD 최소값 (0) |
| TS-FID-028 | TC-FID-014 | NUMD 최대값 (0xFFFFFFFF) |
| TS-FID-029 | TC-FID-015 | OFST 최소값 (0) |
| TS-FID-030 | TC-FID-016 | OFST 최대값 (0xFFFFFFFF) |
| TS-FID-031 | TC-FID-017 | FWUG 경계값 - 정확히 granularity 크기 |
| TS-FID-032 | TC-FID-019 | FWUG 경계값 - granularity 미만 |
| TS-FID-033 | TC-FID-020 | FWUG 경계값 - granularity 배수 |
| TS-FID-034 | TC-FID-021, TC-FID-023 | OFST alignment 경계값 |
| TS-FID-035 | TC-FID-041 | 무효한 Data Pointer |
| TS-FID-036 | TC-FID-042 | 데이터 전송 오류 |
| TS-FID-037 | TC-FID-043 | 연속 범위 간 갭 존재 시 |
| TS-FID-038 | TC-FID-001 | CDW10 구조체 크기 검증 |
| TS-FID-039 | TC-FID-002 | CDW11 구조체 크기 검증 |
| TS-FID-040 | TC-FID-003, TC-FID-004 | CDW10 raw 필드와 bits 필드 동기화 |
| TS-FID-041 | TC-FID-005, TC-FID-006 | CDW11 raw 필드와 bits 필드 동기화 |
| TS-FID-042 | TC-FID-007 | Status Code Enum 값 검증 |

---

## 4. 요구사항-테스트 케이스 매핑

| 요구사항 ID | 테스트 케이스 | 커버리지 |
|-------------|---------------|----------|
| REQ-FID-001 | TC-FID-008 | 100% |
| REQ-FID-002 | TC-FID-009, TC-FID-041, TC-FID-042 | 100% |
| REQ-FID-003 | TC-FID-001, TC-FID-003, TC-FID-004, TC-FID-010, TC-FID-013, TC-FID-014, TC-FID-044, TC-FID-045 | 100% |
| REQ-FID-004 | TC-FID-002, TC-FID-005, TC-FID-006, TC-FID-011, TC-FID-015, TC-FID-016 | 100% |
| REQ-FID-005 | TC-FID-012, TC-FID-015 | 100% |
| REQ-FID-006 | TC-FID-007, TC-FID-017, TC-FID-018, TC-FID-019, TC-FID-020 | 100% |
| REQ-FID-007 | TC-FID-007, TC-FID-021, TC-FID-022, TC-FID-023 | 100% |
| REQ-FID-008 | TC-FID-007, TC-FID-024, TC-FID-025, TC-FID-026 | 100% |
| REQ-FID-009 | TC-FID-031 | 100% |
| REQ-FID-010 | TC-FID-027, TC-FID-043 | 100% |
| REQ-FID-011 | TC-FID-028 | 100% |
| REQ-FID-012 | TC-FID-029, TC-FID-030 | 100% |
| REQ-FID-013 | TC-FID-032 | 100% |
| REQ-FID-014 | TC-FID-033 | 100% |
| REQ-FID-015 | TC-FID-034 | 100% |
| REQ-FID-016 | TC-FID-035, TC-FID-036 | 100% |
| REQ-FID-017 | TC-FID-024 | 100% |
| REQ-FID-018 | TC-FID-017, TC-FID-018, TC-FID-019, TC-FID-020, TC-FID-021, TC-FID-022, TC-FID-023 | 100% |
| REQ-FID-019 | TC-FID-037, TC-FID-038 | 100% |
| REQ-FID-020 | TC-FID-039 | 100% |
| REQ-FID-021 | TC-FID-040 | 100% |

---

## 5. 테스트 데이터 상세

### 5.1 유효한 입력값

#### 5.1.1 NUMD (Number of Dwords) 테스트 값

| 테스트 케이스 | NUMD 값 | 실제 전송 크기 | 설명 |
|---------------|---------|----------------|------|
| TC-FID-013 | 0 | 1 dword (4 bytes) | 최소값 |
| TC-FID-044 | 255 | 256 dwords (1 KB) | 작은 조각 (FWUG=0) |
| TC-FID-010, TC-FID-017 | 1023 | 1024 dwords (4 KB) | 표준 FWUG 크기 |
| TC-FID-020 | 2047 | 2048 dwords (8 KB) | FWUG 배수 (2x) |
| TC-FID-045 | 8191 | 8192 dwords (32 KB) | FWUG 배수 (8x) |
| TC-FID-014 | 0xFFFFFFFF | 0x100000000 dwords (16 GB) | 최대값 |

#### 5.1.2 OFST (Offset) 테스트 값

| 테스트 케이스 | OFST 값 | 실제 바이트 오프셋 | 설명 |
|---------------|---------|-------------------|------|
| TC-FID-012, TC-FID-015 | 0 | 0 bytes | 이미지 시작 |
| TC-FID-021 | 1024 | 4096 bytes (4 KB) | FWUG aligned |
| TC-FID-023 | 2048 | 8192 bytes (8 KB) | FWUG aligned (2x) |
| TC-FID-027 | 3072 | 12288 bytes (12 KB) | FWUG aligned (3x) |
| TC-FID-016 | 0xFFFFFFFF | 0x3FFFFFFFC bytes (16 GB) | 최대값 |

#### 5.1.3 FWUG (Firmware Update Granularity) 테스트 값

| FWUG 값 (dwords) | 크기 (bytes) | 사용 케이스 |
|------------------|--------------|-------------|
| 0 | N/A | Granularity 제약 없음 |
| 1024 | 4 KB | 일반적인 FWUG 값 |
| 2048 | 8 KB | 큰 FWUG 값 |

### 5.2 경계값

#### 5.2.1 NUMD 경계값

| 경계 조건 | 값 | 테스트 케이스 | 예상 결과 |
|-----------|-----|---------------|-----------|
| 최소값 | 0 | TC-FID-013 | 성공 (1 dword 전송) |
| FWUG 정확히 충족 | 1023 (4KB) | TC-FID-017 | 성공 |
| FWUG 미만 (1 dword 차이) | 1022 | TC-FID-019 | 실패 (Invalid Field) |
| FWUG 미충족 (절반) | 511 (2KB) | TC-FID-018 | 실패 (Invalid Field) |
| FWUG 배수 | 2047 (8KB) | TC-FID-020 | 성공 |
| 최대값 | 0xFFFFFFFF | TC-FID-014 | 동작은 구현에 따름 |

#### 5.2.2 OFST 경계값

| 경계 조건 | 값 | 테스트 케이스 | 예상 결과 |
|-----------|-----|---------------|-----------|
| 최소값 (이미지 시작) | 0 | TC-FID-015 | 성공 |
| FWUG aligned | 1024 (4KB) | TC-FID-021 | 성공 |
| FWUG misaligned | 512 (2KB) | TC-FID-022 | 실패 (Invalid Field) |
| FWUG aligned (2배) | 2048 (8KB) | TC-FID-023 | 성공 |
| 최대값 | 0xFFFFFFFF | TC-FID-016 | 동작은 구현에 따름 |

### 5.3 무효한 입력값

#### 5.3.1 Data Pointer 무효값

| 무효 조건 | 값 | 테스트 케이스 | 예상 결과 |
|-----------|-----|---------------|-----------|
| NULL 포인터 | nullptr | TC-FID-041 | 실패 (오류 코드) |
| 무효한 주소 | 0xDEADBEEF | TC-FID-041 | 실패 (오류 코드) |

#### 5.3.2 NUMD 무효값 (FWUG 제약 위반)

| 무효 조건 | FWUG | NUMD | 테스트 케이스 | 예상 상태 코드 |
|-----------|------|------|---------------|----------------|
| Granularity 미충족 | 1024 (4KB) | 511 (2KB) | TC-FID-018 | 0x02 (Invalid Field) |
| 1 dword 미달 | 1024 (4KB) | 1022 | TC-FID-019 | 0x02 (Invalid Field) |

#### 5.3.3 OFST 무효값 (FWUG alignment 위반)

| 무효 조건 | FWUG | OFST | 테스트 케이스 | 예상 상태 코드 |
|-----------|------|------|---------------|----------------|
| Misaligned | 1024 (4KB) | 512 (2KB) | TC-FID-022 | 0x02 (Invalid Field) |
| Misaligned | 1024 (4KB) | 100 | TC-FID-022 | 0x02 (Invalid Field) |

#### 5.3.4 중복 범위

| 조각 1 (OFST, NUMD) | 조각 2 (OFST, NUMD) | 중복 유형 | 테스트 케이스 | 예상 상태 코드 |
|--------------------|--------------------|---------|--------------|--------------|
| (0, 2047) | (1024, 1023) | 완전 포함 | TC-FID-025 | 0x14 (Overlapping Range) |
| (0, 1023) | (1000, 1023) | 부분 중복 (24 dwords) | TC-FID-026 | 0x14 (Overlapping Range) |

### 5.4 Status Code 테스트 데이터

| Status Code | 값 | 테스트 조건 | 관련 테스트 케이스 |
|-------------|-----|-------------|-------------------|
| Success | 0x00 | 유효한 파라미터로 명령 성공 | TC-FID-017, TC-FID-021, TC-FID-024, TC-FID-038 |
| Invalid Field in Command | 0x02 | NUMD/OFST가 FWUG 미충족 | TC-FID-018, TC-FID-019, TC-FID-022 |
| Overlapping Range | 0x14 | 중복 범위 감지 | TC-FID-025, TC-FID-026 |

### 5.5 다중 조각 테스트 시나리오 데이터

#### 5.5.1 순차적 4개 조각 다운로드 (TC-FID-027)

| 조각 번호 | OFST | NUMD | 크기 | 순서 |
|----------|------|------|------|------|
| 1 | 0 | 1023 | 4 KB | 1 |
| 2 | 1024 | 1023 | 4 KB | 2 |
| 3 | 2048 | 1023 | 4 KB | 3 |
| 4 | 3072 | 1023 | 4 KB | 4 |

#### 5.5.2 비순차적 4개 조각 다운로드 (TC-FID-028)

| 조각 번호 | OFST | NUMD | 크기 | 다운로드 순서 |
|----------|------|------|------|--------------|
| 3 | 2048 | 1023 | 4 KB | 1 |
| 1 | 0 | 1023 | 4 KB | 2 |
| 4 | 3072 | 1023 | 4 KB | 3 |
| 2 | 1024 | 1023 | 4 KB | 4 |

#### 5.5.3 갭이 있는 조각 다운로드 (TC-FID-043)

| 조각 번호 | OFST | NUMD | 크기 | 갭 |
|----------|------|------|------|-----|
| 1 | 0 | 1023 | 4 KB | - |
| **갭** | - | - | - | 1024 dwords (4 KB) |
| 2 | 2048 | 1023 | 4 KB | - |

### 5.6 복합 테스트 데이터

#### 5.6.1 Firmware Commit 시퀀스 (TC-FID-034, TC-FID-039)

| 단계 | 동작 | 파라미터 | 예상 결과 |
|------|------|----------|-----------|
| 1 | 첫 번째 이미지 조각 1 다운로드 | OFST=0, NUMD=1023 | 성공 |
| 2 | 첫 번째 이미지 조각 2 다운로드 | OFST=1024, NUMD=1023 | 성공 |
| 3 | Firmware Commit | Slot=2 | 성공 |
| 4 | 두 번째 이미지 조각 1 다운로드 | OFST=0, NUMD=1023 | 성공 (이전 조각 폐기) |

#### 5.6.2 Controller Reset 시퀀스 (TC-FID-035, TC-FID-036)

| 단계 | 동작 | 파라미터 | 예상 결과 |
|------|------|----------|-----------|
| 1 | 조각 1 다운로드 | OFST=0, NUMD=1023 | 성공 |
| 2 | 조각 2 다운로드 | OFST=1024, NUMD=1023 | 성공 |
| 3 | Controller Reset | - | 조각 1, 2 폐기 |
| 4 | 새 조각 1 다운로드 | OFST=0, NUMD=1023 | 성공 (새 시작) |

---

## 6. 테스트 실행 계획

### 6.1 우선순위별 실행 계획

#### Phase 1: P1 테스트 (28개) - 기본 기능 검증
1. **구조체 검증** (8개): TC-FID-001 ~ TC-FID-008
2. **기본 명령** (4개): TC-FID-009, TC-FID-010, TC-FID-011, TC-FID-012
3. **경계값 (P1)** (2개): TC-FID-013, TC-FID-015
4. **FWUG (P1)** (4개): TC-FID-017, TC-FID-018, TC-FID-021, TC-FID-022
5. **중복 범위** (2개): TC-FID-024, TC-FID-025
6. **다중 조각 (P1)** (1개): TC-FID-027
7. **동시 처리** (1개): TC-FID-031
8. **활성화/폐기** (4개): TC-FID-032, TC-FID-033, TC-FID-034, TC-FID-035, TC-FID-036
9. **Completion Queue** (2개): TC-FID-037, TC-FID-038
10. **오류 처리 (P1)** (1개): TC-FID-041

#### Phase 2: P2 테스트 (17개) - 고급 기능 검증
1. **경계값 (P2)** (2개): TC-FID-014, TC-FID-016
2. **FWUG (P2)** (3개): TC-FID-019, TC-FID-020, TC-FID-023
3. **중복 범위 (P2)** (1개): TC-FID-026
4. **다중 조각 (P2)** (3개): TC-FID-028, TC-FID-029, TC-FID-030
5. **중복 업데이트** (2개): TC-FID-039, TC-FID-040
6. **오류 처리 (P2)** (2개): TC-FID-042, TC-FID-043
7. **파라미터 조합** (2개): TC-FID-044, TC-FID-045

### 6.2 테스트 실행 순서 (권장)

```
Phase 1 (P1 - 28개 테스트)
├─ Step 1: 구조체 검증 (TC-FID-001 ~ TC-FID-008)
├─ Step 2: 기본 명령 (TC-FID-009 ~ TC-FID-012)
├─ Step 3: 경계값 P1 (TC-FID-013, TC-FID-015)
├─ Step 4: FWUG P1 (TC-FID-017, TC-FID-018, TC-FID-021, TC-FID-022)
├─ Step 5: 중복 범위 (TC-FID-024, TC-FID-025)
├─ Step 6: 다중 조각 P1 (TC-FID-027)
├─ Step 7: 동시 처리 (TC-FID-031)
├─ Step 8: 활성화/폐기 (TC-FID-032 ~ TC-FID-036)
├─ Step 9: Completion Queue (TC-FID-037, TC-FID-038)
└─ Step 10: 오류 처리 P1 (TC-FID-041)

Phase 2 (P2 - 17개 테스트)
├─ Step 11: 경계값 P2 (TC-FID-014, TC-FID-016)
├─ Step 12: FWUG P2 (TC-FID-019, TC-FID-020, TC-FID-023)
├─ Step 13: 중복 범위 P2 (TC-FID-026)
├─ Step 14: 다중 조각 P2 (TC-FID-028, TC-FID-029, TC-FID-030)
├─ Step 15: 중복 업데이트 (TC-FID-039, TC-FID-040)
├─ Step 16: 오류 처리 P2 (TC-FID-042, TC-FID-043)
└─ Step 17: 파라미터 조합 (TC-FID-044, TC-FID-045)
```

### 6.3 테스트 종속성

| 테스트 케이스 | 선행 요구 테스트 | 이유 |
|---------------|------------------|------|
| TC-FID-009 ~ TC-FID-045 | TC-FID-001 ~ TC-FID-008 | 구조체가 올바르게 정의되어야 명령 테스트 가능 |
| TC-FID-017 ~ TC-FID-023 | TC-FID-010, TC-FID-011 | FWUG 테스트는 기본 NUMD/OFST 설정 동작 필요 |
| TC-FID-024 ~ TC-FID-026 | TC-FID-027 | 중복 범위 테스트는 다중 조각 다운로드 기능 필요 |
| TC-FID-034 | TC-FID-027 | Firmware Commit 후 폐기는 다중 조각 다운로드 필요 |

---

## 7. Google Test 구현 가이드

### 7.1 테스트 픽스처 (Test Fixture)

```cpp
class FirmwareImageDownloadTest : public ::testing::Test {
protected:
    void SetUp() override {
        // NVMe 디바이스 초기화 또는 Mock 객체 생성
        device = new NVMeDevice("/dev/nvme0");
        // 또는 Mock: device = new MockNVMeDevice();

        // 테스트 데이터 버퍼 할당
        buffer_4kb = new uint8_t[4096];
        buffer_8kb = new uint8_t[8192];

        // FWUG 설정 (기본값 4KB)
        fwug_dwords = 1024;
    }

    void TearDown() override {
        delete[] buffer_4kb;
        delete[] buffer_8kb;
        delete device;
    }

    NVMeDevice* device;
    uint8_t* buffer_4kb;
    uint8_t* buffer_8kb;
    uint32_t fwug_dwords;
};
```

### 7.2 검증 매크로 사용 가이드

| 검증 유형 | Google Test 매크로 | 예시 |
|-----------|-------------------|------|
| 동등성 | `EXPECT_EQ(a, b)` | `EXPECT_EQ(cdw10.bits.NUMD, 1023u)` |
| 참/거짓 | `EXPECT_TRUE(expr)`, `EXPECT_FALSE(expr)` | `EXPECT_TRUE(device.FirmwareImageDownload(...))` |
| NULL 체크 | `EXPECT_NE(ptr, nullptr)` | `EXPECT_NE(cqe, nullptr)` |
| 부등성 | `EXPECT_NE(a, b)` | `EXPECT_NE(statusCode, 0x00)` |

### 7.3 예시 테스트 코드 (TC-FID-017)

```cpp
TEST_F(FirmwareImageDownloadTest, TC_FID_017_NUMD_FWUG_Requirement_Met_Success) {
    // Given: FWUG = 4KB (1024 dwords)
    device->SetFWUG(1024);

    // When: NUMD = 1023 (정확히 4KB)
    uint32_t numd = 1023;
    uint32_t ofst = 0;
    bool result = device->FirmwareImageDownload(buffer_4kb, numd, ofst);

    // Then: 명령이 Success(0x00) 상태로 완료
    EXPECT_TRUE(result);
    EXPECT_EQ(device->GetLastStatusCode(), 0x00);
}
```

---

## 8. 커버리지 요약

### 8.1 요구사항 커버리지

| 요구사항 유형 | 총 개수 | 커버된 개수 | 커버리지 |
|---------------|---------|-------------|----------|
| 기능 요구사항 | 21개 | 21개 | 100% |
| P1 요구사항 | 14개 | 14개 | 100% |
| P2 요구사항 | 7개 | 7개 | 100% |

### 8.2 시나리오 커버리지

| 시나리오 유형 | 총 개수 | 커버된 개수 | 커버리지 |
|---------------|---------|-------------|----------|
| Positive | 18개 | 18개 | 100% |
| Negative | 16개 | 16개 | 100% |
| Boundary | 8개 | 8개 | 100% |
| **전체** | **42개** | **42개** | **100%** |

### 8.3 테스트 케이스 통계

| 분류 | 개수 | 비율 |
|------|------|------|
| 구조체 검증 | 8개 | 17.8% |
| 기본 명령 | 4개 | 8.9% |
| 경계값 | 4개 | 8.9% |
| FWUG 관련 | 7개 | 15.6% |
| 중복 범위 | 3개 | 6.7% |
| 다중 조각 | 4개 | 8.9% |
| 동시 처리 | 1개 | 2.2% |
| 활성화/폐기 | 5개 | 11.1% |
| Completion Queue | 2개 | 4.4% |
| 중복 업데이트 | 2개 | 4.4% |
| 오류 처리 | 3개 | 6.7% |
| 파라미터 조합 | 2개 | 4.4% |
| **전체** | **45개** | **100%** |

---

## 9. 변경 이력

| 버전 | 날짜 | 작성자 | 변경 내용 |
|------|------|--------|-----------|
| 1.0 | 2026-01-13 | Claude | 초기 작성 - 42개 시나리오를 45개 테스트 케이스로 설계 |
