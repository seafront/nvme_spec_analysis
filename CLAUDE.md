# NVMe Spec Analysis Project

NVMe(Non-Volatile Memory Express) 스펙을 분석하고 C++ 라이브러리로 구현하는 프로젝트입니다.

## Build

```bash
# Windows (Clang + Ninja)
build.bat

# CMake 직접 사용
cmake -B build -G Ninja -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl
cmake --build build --config Release
```

## Test

```bash
cd build
ctest --output-on-failure
```

## Project Structure

- `header/` - NVMe 커맨드 헤더 파일들 (nvme_*.h)
- `src/` - C++ 구현 소스 파일
- `test/` - Google Test 유닛 테스트
- `chunked_text/` - NVMe 스펙에서 추출한 마크다운 문서 (섹션 5.1-5.2)
- `build/` - CMake 빌드 출력

## Tech Stack

- **Language**: C++17
- **Build**: CMake 3.16+, Ninja
- **Compiler**: Clang-cl (Windows), GCC, MSVC
- **Test**: Google Test 1.14.0

## Conventions

### Naming

- **Enum/Struct/Union/Class**: PascalCase (`AdminOpcode`, `SubmissionQueueEntry`)
- **Namespace**: lowercase (`nvme`)
- **Header files**: `nvme_[command_name].h`
- **Methods**: PascalCase (`IsOpen()`, `Identify()`)

### Code Style

- 모든 헤더는 `nvme` 네임스페이스 내에 정의
- NVMe 스펙 Figure 번호를 주석으로 참조 (예: `// Figure 308`)
- Command Dword 구조체는 union + bits 패턴 사용:

```cpp
union CommandNameCDW10 {
    struct {
        uint32_t FIELD1 : nbits;  // 스펙 참조 주석
        uint32_t FIELD2 : nbits;
    } bits;
    uint32_t raw;
};
```

- 구조체 크기는 static_assert로 컴파일 타임 검증
- RAII 패턴, std::unique_ptr 사용
- Copy 비활성화, Move semantics 활성화

### Error Handling

- bool 반환 + `GetLastErrorString()` / `GetLastStatusCode()` 패턴

## NVMe Commands Reference

Admin Commands (섹션 5.1.x):
- Queue Management: Create/Delete I/O Queues
- Device: Identify, Get/Set Features, Get Log Page
- Firmware: Download, Commit
- Namespace: Create/Delete/Attach/Detach
- Security: Send/Receive, Sanitize

PCIe Transport Commands (섹션 5.2.x):
- Queue management for PCIe transport layer
