# Lockdown Command 테스트 시나리오 명세서

## 1. 개요

### 1.1 문서 정보

| 항목 | 값 |
|------|-----|
| **Command Name** | Lockdown |
| **요구사항 문서** | `docs/requirements/lockdown_requirements.md` |
| **작성일** | 2026-01-12 |
| **작성자** | Claude |

### 1.2 요약

| 항목 | 값 |
|------|-----|
| **총 요구사항 수** | 24 |
| **도출된 시나리오 수** | 48 |
| **커버리지 목표** | 100% (모든 요구사항 최소 1개 시나리오) |
| **Positive 시나리오** | 28 |
| **Negative 시나리오** | 14 |
| **Boundary 시나리오** | 6 |

---

## 2. 테스트 시나리오 목록

### 2.1 기본 명령 구조 테스트

#### TS-001: Opcode 검증
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-001 |
| **시나리오 유형** | Positive |
| **설명** | Lockdown 명령이 Admin Command opcode 0x24를 올바르게 사용하는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. Lockdown 명령 객체 생성<br>2. opcode 값 확인 |
| **기대 결과** | opcode == 0x24 |
| **우선순위** | P1 |

### 2.2 CDW10 필드 테스트

#### TS-002: OFI 필드 - Admin Command Opcode 설정
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-002 |
| **시나리오 유형** | Positive |
| **설명** | OFI 필드에 Admin command opcode를 설정했을 때 올바르게 저장되는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성, SCP=0h |
| **테스트 단계** | 1. OFI 필드에 0x06 (Identify) 설정<br>2. CDW10.bits.OFI 값 확인 |
| **기대 결과** | CDW10.bits.OFI == 0x06 |
| **우선순위** | P1 |

#### TS-003: OFI 필드 - Feature Identifier 설정
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-002 |
| **시나리오 유형** | Positive |
| **설명** | OFI 필드에 Set Features Feature Identifier를 설정했을 때 올바르게 저장되는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성, SCP=2h |
| **테스트 단계** | 1. OFI 필드에 0x01 (Arbitration FID) 설정<br>2. CDW10.bits.OFI 값 확인 |
| **기대 결과** | CDW10.bits.OFI == 0x01 |
| **우선순위** | P1 |

#### TS-004: OFI 필드 최대값 경계 테스트
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-002 |
| **시나리오 유형** | Boundary |
| **설명** | OFI 필드가 8비트 최대값(0xFF)까지 저장 가능한지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. OFI 필드에 0xFF 설정<br>2. CDW10.bits.OFI 값 확인 |
| **기대 결과** | CDW10.bits.OFI == 0xFF |
| **우선순위** | P2 |

#### TS-005: OFI 필드 최소값 경계 테스트
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-002 |
| **시나리오 유형** | Boundary |
| **설명** | OFI 필드가 8비트 최소값(0x00)을 저장 가능한지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. OFI 필드에 0x00 설정<br>2. CDW10.bits.OFI 값 확인 |
| **기대 결과** | CDW10.bits.OFI == 0x00 |
| **우선순위** | P2 |

#### TS-006: IFC 필드 - Admin Submission Queue (00b)
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-003, REQ-LKD-010 |
| **시나리오 유형** | Positive |
| **설명** | IFC 필드가 00b일 때 Admin Submission Queue만 영향받도록 설정되는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. IFC 필드에 LockdownInterface::AdminSubmissionQueue (0) 설정<br>2. CDW10.bits.IFC 값 확인 |
| **기대 결과** | CDW10.bits.IFC == 0 |
| **우선순위** | P1 |

#### TS-007: IFC 필드 - Admin SQ 및 Management Endpoint (01b)
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-003, REQ-LKD-011 |
| **시나리오 유형** | Positive |
| **설명** | IFC 필드가 01b일 때 Admin SQ와 Management Endpoint 모두 영향받도록 설정되는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성, Management Endpoint 존재 |
| **테스트 단계** | 1. IFC 필드에 LockdownInterface::AdminSQAndMgmtEndpoint (1) 설정<br>2. CDW10.bits.IFC 값 확인 |
| **기대 결과** | CDW10.bits.IFC == 1 |
| **우선순위** | P2 |

#### TS-008: IFC 필드 - Management Endpoint Only (10b)
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-003, REQ-LKD-012 |
| **시나리오 유형** | Positive |
| **설명** | IFC 필드가 10b일 때 Management Endpoint만 영향받도록 설정되는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성, Management Endpoint 존재 |
| **테스트 단계** | 1. IFC 필드에 LockdownInterface::MgmtEndpointOnly (2) 설정<br>2. CDW10.bits.IFC 값 확인 |
| **기대 결과** | CDW10.bits.IFC == 2 |
| **우선순위** | P2 |

#### TS-009: IFC 필드 - Reserved 값 (11b) 검증
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-023 |
| **시나리오 유형** | Negative |
| **설명** | IFC 필드에 Reserved 값(11b)이 설정되지 않도록 검증하거나, 설정 시 적절히 처리되는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. IFC 필드에 LockdownInterface::Reserved (3) 설정 시도<br>2. 구현에서 경고 또는 검증 수행 확인 |
| **기대 결과** | Reserved 값 사용에 대한 적절한 경고 또는 검증 |
| **우선순위** | P2 |

#### TS-010: PRHBT 필드 - 금지 설정 (1)
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-004 |
| **시나리오 유형** | Positive |
| **설명** | PRHBT 필드가 1일 때 명령 실행이 금지되도록 설정되는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. PRHBT 필드에 LockdownProhibit::Prohibit (1) 설정<br>2. CDW10.bits.PRHBT 값 확인 |
| **기대 결과** | CDW10.bits.PRHBT == 1 |
| **우선순위** | P1 |

#### TS-011: PRHBT 필드 - 허용 설정 (0)
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-004 |
| **시나리오 유형** | Positive |
| **설명** | PRHBT 필드가 0일 때 명령 실행이 허용되도록 설정되는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. PRHBT 필드에 LockdownProhibit::Allow (0) 설정<br>2. CDW10.bits.PRHBT 값 확인 |
| **기대 결과** | CDW10.bits.PRHBT == 0 |
| **우선순위** | P1 |

#### TS-012: SCP 필드 - Admin Command Opcode (0h)
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-005, REQ-LKD-006 |
| **시나리오 유형** | Positive |
| **설명** | SCP 필드가 0h일 때 OFI가 Admin command opcode로 해석되는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. SCP 필드에 LockdownScope::AdminCommandOpcode (0) 설정<br>2. CDW10.bits.SCP 값 확인 |
| **기대 결과** | CDW10.bits.SCP == 0 |
| **우선순위** | P1 |

#### TS-013: SCP 필드 - Set Features FID (2h)
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-005, REQ-LKD-007 |
| **시나리오 유형** | Positive |
| **설명** | SCP 필드가 2h일 때 OFI가 Set Features Feature Identifier로 해석되는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. SCP 필드에 LockdownScope::SetFeaturesFID (2) 설정<br>2. CDW10.bits.SCP 값 확인 |
| **기대 결과** | CDW10.bits.SCP == 2 |
| **우선순위** | P1 |

#### TS-014: SCP 필드 - MI Command Set Opcode (3h)
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-005, REQ-LKD-008 |
| **시나리오 유형** | Positive |
| **설명** | SCP 필드가 3h일 때 OFI가 Management Interface Command Set opcode로 해석되는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. SCP 필드에 LockdownScope::MICommandSetOpcode (3) 설정<br>2. CDW10.bits.SCP 값 확인 |
| **기대 결과** | CDW10.bits.SCP == 3 |
| **우선순위** | P2 |

#### TS-015: SCP 필드 - PCIe Command Set Opcode (4h)
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-005, REQ-LKD-009 |
| **시나리오 유형** | Positive |
| **설명** | SCP 필드가 4h일 때 OFI가 PCIe Command Set opcode로 해석되는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. SCP 필드에 LockdownScope::PCIeCommandSetOpcode (4) 설정<br>2. CDW10.bits.SCP 값 확인 |
| **기대 결과** | CDW10.bits.SCP == 4 |
| **우선순위** | P2 |

#### TS-016: SCP 필드 - Reserved 값 (1h) 검증
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-024 |
| **시나리오 유형** | Negative |
| **설명** | SCP 필드에 Reserved 값(1h)이 설정되지 않도록 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. SCP 필드에 Reserved1 (1) 설정 시도<br>2. 구현에서 경고 또는 검증 수행 확인 |
| **기대 결과** | Reserved 값 사용에 대한 적절한 경고 또는 검증 |
| **우선순위** | P2 |

#### TS-017: SCP 필드 - Reserved 값 (5h-Fh) 경계 테스트
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-024 |
| **시나리오 유형** | Boundary |
| **설명** | SCP 필드 4비트의 경계값 테스트 (5h, Fh) |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. SCP 필드에 5 설정 시도<br>2. SCP 필드에 15(0xF) 설정 시도<br>3. 각각의 결과 확인 |
| **기대 결과** | Reserved 값 범위(5h-Fh)에 대한 적절한 처리 |
| **우선순위** | P2 |

### 2.3 CDW14 필드 테스트

#### TS-018: UIDX 필드 - UUID Index 설정
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-013 |
| **시나리오 유형** | Positive |
| **설명** | CDW14의 UIDX 필드에 UUID Index 값이 올바르게 설정되는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성, SCP=2h |
| **테스트 단계** | 1. UIDX 필드에 유효한 UUID Index (예: 1) 설정<br>2. CDW14.bits.UIDX 값 확인 |
| **기대 결과** | CDW14.bits.UIDX == 1 |
| **우선순위** | P2 |

#### TS-019: UIDX 필드 최대값 경계 테스트
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-013 |
| **시나리오 유형** | Boundary |
| **설명** | UIDX 필드가 7비트 최대값(0x7F=127)까지 저장 가능한지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. UIDX 필드에 0x7F 설정<br>2. CDW14.bits.UIDX 값 확인 |
| **기대 결과** | CDW14.bits.UIDX == 0x7F |
| **우선순위** | P2 |

#### TS-020: UIDX 필드 - SCP=2h 및 UUID 지원 시 사용
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-014 |
| **시나리오 유형** | Positive |
| **설명** | 컨트롤러가 UUID 선택을 지원하고 SCP가 2h일 때 CDW14의 UUID Index가 사용되는지 검증 |
| **사전 조건** | UUID 선택 지원 컨트롤러, SCP=2h |
| **테스트 단계** | 1. SCP 필드에 2h 설정<br>2. UIDX 필드에 유효한 값 설정<br>3. 명령 생성 후 CDW14 값 확인 |
| **기대 결과** | CDW14의 UIDX 값이 명령에 반영됨 |
| **우선순위** | P2 |

#### TS-021: UIDX 필드 - SCP != 2h일 때 무시 검증
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-015 |
| **시나리오 유형** | Positive |
| **설명** | SCP 필드가 2h가 아닌 경우 UUID Index 필드가 무시되는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. SCP 필드에 0h (Admin Command) 설정<br>2. UIDX 필드에 값 설정<br>3. 명령 동작 시 UIDX가 무시되는지 확인 |
| **기대 결과** | SCP != 2h일 때 UIDX 필드는 처리에 영향 없음 |
| **우선순위** | P2 |

### 2.4 Reserved 필드 테스트

#### TS-022: CDW10 Reserved 필드 (bits 31:16) 0 설정
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-021 |
| **시나리오 유형** | Positive |
| **설명** | CDW10의 bits 31:16이 0으로 설정되는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. Lockdown 명령 객체 생성<br>2. CDW10.raw의 상위 16비트 확인 |
| **기대 결과** | (CDW10.raw >> 16) == 0 |
| **우선순위** | P2 |

#### TS-023: CDW10 Reserved 필드 (bit 07) 0 설정
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-021 |
| **시나리오 유형** | Positive |
| **설명** | CDW10의 bit 07이 0으로 설정되는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. Lockdown 명령 객체 생성<br>2. CDW10.bits.Rsvd7 값 확인 |
| **기대 결과** | CDW10.bits.Rsvd7 == 0 |
| **우선순위** | P2 |

#### TS-024: CDW14 Reserved 필드 (bits 31:07) 0 설정
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-022 |
| **시나리오 유형** | Positive |
| **설명** | CDW14의 bits 31:07이 0으로 설정되는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. Lockdown 명령 객체 생성<br>2. CDW14.raw의 bits 31:07 확인 |
| **기대 결과** | (CDW14.raw >> 7) == 0 |
| **우선순위** | P2 |

### 2.5 Status Code 테스트

#### TS-025: 성공 상태 코드 검증
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-020 |
| **시나리오 유형** | Positive |
| **설명** | Lockdown 명령이 성공적으로 완료될 때 SUCCESS(00h) 상태 코드 반환 검증 |
| **사전 조건** | 유효한 Lockdown 명령 준비, 금지 가능한 opcode 사용 |
| **테스트 단계** | 1. 유효한 Lockdown 명령 실행<br>2. 반환된 status code 확인 |
| **기대 결과** | status code == 0x00 (SUCCESS) |
| **우선순위** | P1 |

#### TS-026: Prohibition Not Supported 상태 코드 (28h)
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-016 |
| **시나리오 유형** | Negative |
| **설명** | 금지 불가능한 opcode/FID 지정 시 status code 28h 반환 검증 |
| **사전 조건** | 금지 불가능한 opcode/FID가 존재하는 환경 |
| **테스트 단계** | 1. 금지 불가능한 opcode로 Lockdown 명령 실행<br>2. 반환된 status code 확인 |
| **기대 결과** | status code == 0x28 (PROHIBITION_NOT_SUPPORTED) |
| **우선순위** | P1 |

#### TS-027: Invalid Field - Management Endpoint 없음 (IFC=01b)
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-017 |
| **시나리오 유형** | Negative |
| **설명** | IFC=01b이고 Management Endpoint가 없을 때 Invalid Field 상태 코드 반환 검증 |
| **사전 조건** | Management Endpoint가 없는 NVM 서브시스템 |
| **테스트 단계** | 1. IFC=01b로 Lockdown 명령 실행<br>2. 반환된 status code 확인 |
| **기대 결과** | status code == 0x02 (INVALID_FIELD) |
| **우선순위** | P1 |

#### TS-028: Invalid Field - Management Endpoint 없음 (IFC=10b)
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-017 |
| **시나리오 유형** | Negative |
| **설명** | IFC=10b이고 Management Endpoint가 없을 때 Invalid Field 상태 코드 반환 검증 |
| **사전 조건** | Management Endpoint가 없는 NVM 서브시스템 |
| **테스트 단계** | 1. IFC=10b로 Lockdown 명령 실행<br>2. 반환된 status code 확인 |
| **기대 결과** | status code == 0x02 (INVALID_FIELD) |
| **우선순위** | P1 |

#### TS-029: Invalid Field - IFC=00b 및 SCP=4h 조합
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-018 |
| **시나리오 유형** | Negative |
| **설명** | IFC=00b이고 SCP=4h일 때 Invalid Field 상태 코드 반환 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. IFC=00b, SCP=4h로 Lockdown 명령 실행<br>2. 반환된 status code 확인 |
| **기대 결과** | status code == 0x02 (INVALID_FIELD) |
| **우선순위** | P1 |

#### TS-030: Invalid Field - IFC=01b 및 SCP=4h 조합
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-018 |
| **시나리오 유형** | Negative |
| **설명** | IFC=01b이고 SCP=4h일 때 Invalid Field 상태 코드 반환 검증 |
| **사전 조건** | Management Endpoint가 있는 NVM 서브시스템 |
| **테스트 단계** | 1. IFC=01b, SCP=4h로 Lockdown 명령 실행<br>2. 반환된 status code 확인 |
| **기대 결과** | status code == 0x02 (INVALID_FIELD) |
| **우선순위** | P1 |

### 2.6 동작 검증 테스트

#### TS-031: 이미 금지된 명령 재금지 - 오류 없음
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-019 |
| **시나리오 유형** | Positive |
| **설명** | 이미 금지된 명령을 다시 금지해도 오류가 발생하지 않는지 검증 |
| **사전 조건** | 특정 명령이 이미 금지된 상태 |
| **테스트 단계** | 1. 특정 opcode를 금지하는 Lockdown 명령 실행<br>2. 같은 opcode를 다시 금지하는 Lockdown 명령 실행<br>3. 두 번째 명령의 상태 코드 확인 |
| **기대 결과** | status code == 0x00 (SUCCESS) |
| **우선순위** | P1 |

#### TS-032: 이미 허용된 명령 재허용 - 오류 없음
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-019 |
| **시나리오 유형** | Positive |
| **설명** | 이미 허용된 명령을 다시 허용해도 오류가 발생하지 않는지 검증 |
| **사전 조건** | 특정 명령이 허용된 상태 (기본 상태) |
| **테스트 단계** | 1. 특정 opcode를 허용하는 Lockdown 명령 실행 (PRHBT=0)<br>2. 상태 코드 확인 |
| **기대 결과** | status code == 0x00 (SUCCESS) |
| **우선순위** | P1 |

#### TS-033: 금지 후 허용 전환
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-004, REQ-LKD-019 |
| **시나리오 유형** | Positive |
| **설명** | 금지된 명령을 다시 허용할 수 있는지 검증 |
| **사전 조건** | 특정 명령이 금지된 상태 |
| **테스트 단계** | 1. 특정 opcode를 금지하는 Lockdown 명령 실행<br>2. 같은 opcode를 허용하는 Lockdown 명령 실행<br>3. 두 번째 명령의 상태 코드 확인 |
| **기대 결과** | status code == 0x00 (SUCCESS), 명령이 다시 허용됨 |
| **우선순위** | P1 |

#### TS-034: Completion Queue Entry 생성 검증
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-020 |
| **시나리오 유형** | Positive |
| **설명** | Lockdown 명령 완료 시 Admin Completion Queue에 completion queue entry가 post되는지 검증 |
| **사전 조건** | Admin Queue 설정 완료 |
| **테스트 단계** | 1. Lockdown 명령 실행<br>2. Admin Completion Queue에서 completion entry 확인 |
| **기대 결과** | Completion queue entry가 올바르게 생성됨 |
| **우선순위** | P1 |

### 2.7 구조체 크기 및 레이아웃 테스트

#### TS-035: CDW10 Union 크기 검증
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-002, REQ-LKD-003, REQ-LKD-004, REQ-LKD-005 |
| **시나리오 유형** | Positive |
| **설명** | LockdownCDW10 union이 정확히 4바이트(32비트)인지 검증 |
| **사전 조건** | 없음 |
| **테스트 단계** | 1. sizeof(LockdownCDW10) 확인 |
| **기대 결과** | sizeof(LockdownCDW10) == 4 |
| **우선순위** | P1 |

#### TS-036: CDW14 Union 크기 검증
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-013 |
| **시나리오 유형** | Positive |
| **설명** | LockdownCDW14 union이 정확히 4바이트(32비트)인지 검증 |
| **사전 조건** | 없음 |
| **테스트 단계** | 1. sizeof(LockdownCDW14) 확인 |
| **기대 결과** | sizeof(LockdownCDW14) == 4 |
| **우선순위** | P1 |

#### TS-037: CDW10 비트 필드 레이아웃 검증
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-002, REQ-LKD-003, REQ-LKD-004, REQ-LKD-005 |
| **시나리오 유형** | Positive |
| **설명** | CDW10의 비트 필드가 스펙에 맞게 배치되었는지 검증 |
| **사전 조건** | 없음 |
| **테스트 단계** | 1. 각 필드에 테스트 값 설정<br>2. raw 값과 비교하여 비트 위치 확인 |
| **기대 결과** | SCP: bits 0-3, PRHBT: bit 4, IFC: bits 5-6, OFI: bits 8-15 |
| **우선순위** | P1 |

#### TS-038: CDW14 비트 필드 레이아웃 검증
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-013 |
| **시나리오 유형** | Positive |
| **설명** | CDW14의 비트 필드가 스펙에 맞게 배치되었는지 검증 |
| **사전 조건** | 없음 |
| **테스트 단계** | 1. UIDX 필드에 테스트 값 설정<br>2. raw 값과 비교하여 비트 위치 확인 |
| **기대 결과** | UIDX: bits 0-6 |
| **우선순위** | P2 |

### 2.8 Enum 값 테스트

#### TS-039: LockdownScope Enum 값 검증
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-006, REQ-LKD-007, REQ-LKD-008, REQ-LKD-009 |
| **시나리오 유형** | Positive |
| **설명** | LockdownScope enum의 모든 값이 스펙과 일치하는지 검증 |
| **사전 조건** | 없음 |
| **테스트 단계** | 1. 각 enum 값 확인:<br>- AdminCommandOpcode == 0<br>- SetFeaturesFID == 2<br>- MICommandSetOpcode == 3<br>- PCIeCommandSetOpcode == 4 |
| **기대 결과** | 모든 enum 값이 스펙과 일치 |
| **우선순위** | P1 |

#### TS-040: LockdownInterface Enum 값 검증
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-010, REQ-LKD-011, REQ-LKD-012 |
| **시나리오 유형** | Positive |
| **설명** | LockdownInterface enum의 모든 값이 스펙과 일치하는지 검증 |
| **사전 조건** | 없음 |
| **테스트 단계** | 1. 각 enum 값 확인:<br>- AdminSubmissionQueue == 0<br>- AdminSQAndMgmtEndpoint == 1<br>- MgmtEndpointOnly == 2 |
| **기대 결과** | 모든 enum 값이 스펙과 일치 |
| **우선순위** | P1 |

#### TS-041: LockdownProhibit Enum 값 검증
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-004 |
| **시나리오 유형** | Positive |
| **설명** | LockdownProhibit enum의 모든 값이 스펙과 일치하는지 검증 |
| **사전 조건** | 없음 |
| **테스트 단계** | 1. 각 enum 값 확인:<br>- Allow == 0<br>- Prohibit == 1 |
| **기대 결과** | 모든 enum 값이 스펙과 일치 |
| **우선순위** | P1 |

### 2.9 통합 테스트

#### TS-042: Admin Command 금지 시나리오
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-004, REQ-LKD-006, REQ-LKD-010 |
| **시나리오 유형** | Positive |
| **설명** | Admin Command opcode를 금지하는 전체 시나리오 검증 |
| **사전 조건** | 금지 가능한 Admin command 존재 |
| **테스트 단계** | 1. SCP=0h, IFC=00b, PRHBT=1, OFI=target_opcode로 Lockdown 실행<br>2. 명령 성공 확인<br>3. 금지된 명령 실행 시도<br>4. Command Sequence Error 확인 |
| **기대 결과** | 명령이 금지되어 실행 불가 |
| **우선순위** | P1 |

#### TS-043: Set Features FID 금지 시나리오
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-004, REQ-LKD-007, REQ-LKD-010 |
| **시나리오 유형** | Positive |
| **설명** | Set Features Feature Identifier를 금지하는 전체 시나리오 검증 |
| **사전 조건** | 금지 가능한 Feature Identifier 존재 |
| **테스트 단계** | 1. SCP=2h, IFC=00b, PRHBT=1, OFI=target_fid로 Lockdown 실행<br>2. 명령 성공 확인<br>3. 금지된 FID로 Set Features 실행 시도<br>4. 실행 거부 확인 |
| **기대 결과** | 해당 FID의 Set Features가 금지됨 |
| **우선순위** | P1 |

#### TS-044: Management Interface 금지 시나리오
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-008, REQ-LKD-011 |
| **시나리오 유형** | Positive |
| **설명** | Management Interface Command를 금지하는 전체 시나리오 검증 |
| **사전 조건** | Management Endpoint 존재, MI Command 지원 |
| **테스트 단계** | 1. SCP=3h, IFC=01b, PRHBT=1로 Lockdown 실행<br>2. 명령 성공 확인 |
| **기대 결과** | MI Command가 금지됨 |
| **우선순위** | P2 |

#### TS-045: PCIe Command Set 금지 시나리오 (IFC=10b)
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-009, REQ-LKD-012 |
| **시나리오 유형** | Positive |
| **설명** | PCIe Command Set opcode를 Management Endpoint에서만 금지하는 시나리오 검증 |
| **사전 조건** | Management Endpoint 존재 |
| **테스트 단계** | 1. SCP=4h, IFC=10b, PRHBT=1로 Lockdown 실행<br>2. 명령 성공 확인 |
| **기대 결과** | PCIe Command가 Management Endpoint에서만 금지됨 |
| **우선순위** | P2 |

#### TS-046: UUID Index 사용 시나리오
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-013, REQ-LKD-014 |
| **시나리오 유형** | Positive |
| **설명** | vendor specific FID에 대해 UUID Index를 사용하는 시나리오 검증 |
| **사전 조건** | UUID 선택 지원 컨트롤러, vendor specific FID 존재 |
| **테스트 단계** | 1. SCP=2h, UIDX=valid_index로 Lockdown 실행<br>2. 명령 성공 확인 |
| **기대 결과** | 특정 UUID에 대한 FID 금지가 적용됨 |
| **우선순위** | P2 |

### 2.10 경계 조건 테스트

#### TS-047: 모든 필드 최대값 동시 설정
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-002, REQ-LKD-003, REQ-LKD-005, REQ-LKD-013 |
| **시나리오 유형** | Boundary |
| **설명** | 모든 필드에 최대 유효값을 설정했을 때 올바르게 동작하는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. OFI=0xFF, IFC=2, SCP=4, PRHBT=1, UIDX=0x7F 설정<br>2. 각 필드 값 확인 |
| **기대 결과** | 모든 필드가 올바르게 설정됨 |
| **우선순위** | P2 |

#### TS-048: 모든 필드 최소값 동시 설정
| 항목 | 내용 |
|------|------|
| **관련 요구사항** | REQ-LKD-002, REQ-LKD-003, REQ-LKD-005, REQ-LKD-013 |
| **시나리오 유형** | Boundary |
| **설명** | 모든 필드에 최소값(0)을 설정했을 때 올바르게 동작하는지 검증 |
| **사전 조건** | Lockdown 명령 객체 생성 |
| **테스트 단계** | 1. OFI=0, IFC=0, SCP=0, PRHBT=0, UIDX=0 설정<br>2. 각 필드 값 확인 |
| **기대 결과** | 모든 필드가 0으로 설정됨 |
| **우선순위** | P2 |

---

## 3. 요구사항-시나리오 매핑

| 요구사항 ID | 관련 테스트 시나리오 | 커버리지 |
|-------------|----------------------|----------|
| REQ-LKD-001 | TS-001 | 완료 |
| REQ-LKD-002 | TS-002, TS-003, TS-004, TS-005, TS-035, TS-037, TS-047, TS-048 | 완료 |
| REQ-LKD-003 | TS-006, TS-007, TS-008, TS-035, TS-037, TS-047, TS-048 | 완료 |
| REQ-LKD-004 | TS-010, TS-011, TS-033, TS-041, TS-042, TS-043 | 완료 |
| REQ-LKD-005 | TS-012, TS-013, TS-014, TS-015, TS-035, TS-037, TS-047, TS-048 | 완료 |
| REQ-LKD-006 | TS-012, TS-039 | 완료 |
| REQ-LKD-007 | TS-013, TS-039 | 완료 |
| REQ-LKD-008 | TS-014, TS-039, TS-044 | 완료 |
| REQ-LKD-009 | TS-015, TS-039, TS-045 | 완료 |
| REQ-LKD-010 | TS-006, TS-040, TS-042, TS-043 | 완료 |
| REQ-LKD-011 | TS-007, TS-040, TS-044 | 완료 |
| REQ-LKD-012 | TS-008, TS-040, TS-045 | 완료 |
| REQ-LKD-013 | TS-018, TS-019, TS-036, TS-038, TS-046, TS-047, TS-048 | 완료 |
| REQ-LKD-014 | TS-020, TS-046 | 완료 |
| REQ-LKD-015 | TS-021 | 완료 |
| REQ-LKD-016 | TS-026 | 완료 |
| REQ-LKD-017 | TS-027, TS-028 | 완료 |
| REQ-LKD-018 | TS-029, TS-030 | 완료 |
| REQ-LKD-019 | TS-031, TS-032, TS-033 | 완료 |
| REQ-LKD-020 | TS-025, TS-034 | 완료 |
| REQ-LKD-021 | TS-022, TS-023 | 완료 |
| REQ-LKD-022 | TS-024 | 완료 |
| REQ-LKD-023 | TS-009 | 완료 |
| REQ-LKD-024 | TS-016, TS-017 | 완료 |

---

## 4. 시나리오 유형별 분류

### 4.1 Positive 테스트 시나리오 (28개)

| 시나리오 ID | 설명 | 우선순위 |
|-------------|------|----------|
| TS-001 | Opcode 검증 | P1 |
| TS-002 | OFI 필드 - Admin Command Opcode 설정 | P1 |
| TS-003 | OFI 필드 - Feature Identifier 설정 | P1 |
| TS-006 | IFC 필드 - Admin Submission Queue (00b) | P1 |
| TS-007 | IFC 필드 - Admin SQ 및 Management Endpoint (01b) | P2 |
| TS-008 | IFC 필드 - Management Endpoint Only (10b) | P2 |
| TS-010 | PRHBT 필드 - 금지 설정 (1) | P1 |
| TS-011 | PRHBT 필드 - 허용 설정 (0) | P1 |
| TS-012 | SCP 필드 - Admin Command Opcode (0h) | P1 |
| TS-013 | SCP 필드 - Set Features FID (2h) | P1 |
| TS-014 | SCP 필드 - MI Command Set Opcode (3h) | P2 |
| TS-015 | SCP 필드 - PCIe Command Set Opcode (4h) | P2 |
| TS-018 | UIDX 필드 - UUID Index 설정 | P2 |
| TS-020 | UIDX 필드 - SCP=2h 및 UUID 지원 시 사용 | P2 |
| TS-021 | UIDX 필드 - SCP != 2h일 때 무시 검증 | P2 |
| TS-022 | CDW10 Reserved 필드 (bits 31:16) 0 설정 | P2 |
| TS-023 | CDW10 Reserved 필드 (bit 07) 0 설정 | P2 |
| TS-024 | CDW14 Reserved 필드 (bits 31:07) 0 설정 | P2 |
| TS-025 | 성공 상태 코드 검증 | P1 |
| TS-031 | 이미 금지된 명령 재금지 - 오류 없음 | P1 |
| TS-032 | 이미 허용된 명령 재허용 - 오류 없음 | P1 |
| TS-033 | 금지 후 허용 전환 | P1 |
| TS-034 | Completion Queue Entry 생성 검증 | P1 |
| TS-035 | CDW10 Union 크기 검증 | P1 |
| TS-036 | CDW14 Union 크기 검증 | P1 |
| TS-037 | CDW10 비트 필드 레이아웃 검증 | P1 |
| TS-038 | CDW14 비트 필드 레이아웃 검증 | P2 |
| TS-039 | LockdownScope Enum 값 검증 | P1 |
| TS-040 | LockdownInterface Enum 값 검증 | P1 |
| TS-041 | LockdownProhibit Enum 값 검증 | P1 |
| TS-042 | Admin Command 금지 시나리오 | P1 |
| TS-043 | Set Features FID 금지 시나리오 | P1 |
| TS-044 | Management Interface 금지 시나리오 | P2 |
| TS-045 | PCIe Command Set 금지 시나리오 (IFC=10b) | P2 |
| TS-046 | UUID Index 사용 시나리오 | P2 |

### 4.2 Negative 테스트 시나리오 (14개)

| 시나리오 ID | 설명 | 우선순위 |
|-------------|------|----------|
| TS-009 | IFC 필드 - Reserved 값 (11b) 검증 | P2 |
| TS-016 | SCP 필드 - Reserved 값 (1h) 검증 | P2 |
| TS-026 | Prohibition Not Supported 상태 코드 (28h) | P1 |
| TS-027 | Invalid Field - Management Endpoint 없음 (IFC=01b) | P1 |
| TS-028 | Invalid Field - Management Endpoint 없음 (IFC=10b) | P1 |
| TS-029 | Invalid Field - IFC=00b 및 SCP=4h 조합 | P1 |
| TS-030 | Invalid Field - IFC=01b 및 SCP=4h 조합 | P1 |

### 4.3 Boundary 테스트 시나리오 (6개)

| 시나리오 ID | 설명 | 우선순위 |
|-------------|------|----------|
| TS-004 | OFI 필드 최대값 경계 테스트 | P2 |
| TS-005 | OFI 필드 최소값 경계 테스트 | P2 |
| TS-017 | SCP 필드 - Reserved 값 (5h-Fh) 경계 테스트 | P2 |
| TS-019 | UIDX 필드 최대값 경계 테스트 | P2 |
| TS-047 | 모든 필드 최대값 동시 설정 | P2 |
| TS-048 | 모든 필드 최소값 동시 설정 | P2 |

---

## 5. 테스트 데이터 개요

### 5.1 CDW10 테스트 데이터

| 필드 | 비트 | 유효 범위 | 테스트 값 |
|------|------|-----------|-----------|
| SCP | 03:00 | 0h, 2h-4h (유효), 1h, 5h-Fh (Reserved) | 0, 2, 3, 4, 1, 5, 15 |
| PRHBT | 04 | 0, 1 | 0, 1 |
| IFC | 06:05 | 00b-10b (유효), 11b (Reserved) | 0, 1, 2, 3 |
| Reserved | 07 | 0 (항상) | 0 |
| OFI | 15:08 | 0x00-0xFF | 0x00, 0x06, 0x24, 0xFF |
| Reserved | 31:16 | 0 (항상) | 0 |

### 5.2 CDW14 테스트 데이터

| 필드 | 비트 | 유효 범위 | 테스트 값 |
|------|------|-----------|-----------|
| UIDX | 06:00 | 0x00-0x7F | 0x00, 0x01, 0x7F |
| Reserved | 31:07 | 0 (항상) | 0 |

### 5.3 Status Code 테스트 데이터

| Status Code | 값 | 테스트 조건 |
|-------------|-----|-------------|
| SUCCESS | 0x00 | 유효한 명령 실행 |
| INVALID_FIELD | 0x02 | IFC/SCP 조합 오류, Management Endpoint 없음 |
| PROHIBITION_NOT_SUPPORTED | 0x28 | 금지 불가능한 opcode/FID |

### 5.4 테스트용 Opcode/FID 예시

| 유형 | 값 | 설명 |
|------|-----|------|
| Admin Opcode | 0x06 | Identify |
| Admin Opcode | 0x09 | Set Features |
| Admin Opcode | 0x24 | Lockdown (자기 자신은 금지 불가) |
| Feature ID | 0x01 | Arbitration |
| Feature ID | 0x02 | Power Management |

---

## 6. 미커버 요구사항

모든 요구사항(REQ-LKD-001 ~ REQ-LKD-024)이 최소 1개 이상의 테스트 시나리오에 매핑되었습니다.

**커버리지: 100% (24/24 요구사항)**

---

## 7. 변경 이력

| 버전 | 날짜 | 작성자 | 변경 내용 |
|------|------|--------|-----------|
| 1.0 | 2026-01-12 | Claude | 초기 작성 |
