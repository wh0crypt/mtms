# Contributing to MTMS

Thanks for your interest in improving **MTMS**.

## Before You Start

1. Open an issue for bug reports, feature requests, or design discussions.
2. For major changes, align on scope in an issue before coding.
3. Keep pull requests focused and small enough to review clearly.

## Development Setup

### Requirements

- C++23 compiler (Clang 17+ or GCC 13+ recommended)
- CMake 3.25+
- Ninja (recommended) or Make
- Qt6 (`qt6-base-dev`) only if you build GUI targets

### Build Commands

```bash
# CLI only (default)
cmake -S . -B build -DBUILD_CLI=ON -DBUILD_GUI=OFF -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build build

# CLI + GUI
cmake -S . -B build -DBUILD_CLI=ON -DBUILD_GUI=ON -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build build
```

### Run CLI Example

```bash
./build/mtms-cli --config examples/bin_inc.toml --input "1011" --batch
```

## Coding Standards

### C++ Style

- Follow `.clang-format` (Microsoft-based, 4-space indent, 100-column limit).
- Keep code C++23-compatible with the current project settings.
- Prefer clear, deterministic logic and explicit error reporting over silent failure.

### Static Analysis

- Keep changes clean under the enabled `.clang-tidy` checks (`bugprone-*`, `modernize-*`, `performance-*`, `readability-*`, and related profiles).
- If you introduce new warnings, address them before opening the PR.

### Project Conventions

- Core engine code lives in `src/core`.
- CLI app code lives in `src/cli`.
- TOML machine examples go in `examples/`.
- `include/toml++/toml.hpp` is vendored third-party code; do not modify it unless updating that dependency intentionally.

## Documentation

- Update `README.md` when behavior, build flow, options, or examples change.
- Public APIs should keep Doxygen-friendly comments consistent with existing headers.
- If you change docs-related behavior, ensure `Doxyfile` input/output assumptions stay valid.

## Pull Request Checklist

1. Branch from `main`.
2. Keep commits focused and descriptive.
3. Ensure the project builds for the target(s) you touched (CLI, GUI, or both).
4. Update docs/examples when relevant.
5. Open a PR against `main` with:
   - clear summary of what changed
   - rationale and tradeoffs (if any)
   - issue reference (if applicable)

## Community

By participating, you agree to follow the repository [Code of Conduct](CODE_OF_CONDUCT.md).
