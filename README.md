# MTMS - Modern Turing Machine Simulator

![C++](https://img.shields.io/badge/C%2B%2B-23-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.25+-brightgreen.svg)
![License](https://img.shields.io/badge/license-MIT-lightgrey.svg)

![CLI](https://github.com/wh0crypt/mtms/actions/workflows/cli.yml/badge.svg)
![GUI](https://github.com/wh0crypt/mtms/actions/workflows/gui.yml/badge.svg)
![Build](https://github.com/wh0crypt/mtms/actions/workflows/full.yml/badge.svg)
![Docs](https://github.com/wh0crypt/mtms/actions/workflows/docs.yml/badge.svg)

![Platform](https://img.shields.io/badge/platform-linux%20%7C%20windows%20%7C%20macos-informational.svg)
![Issues](https://img.shields.io/github/issues/wh0crypt/mtms)
![Stars](https://img.shields.io/github/stars/wh0crypt/mtms?style=social)

A modern educational and experimental simulator for deterministic multi-tape Turing machines written in C++23.

It has three layers:

- **Core**: the actual machine model and simulator
- **CLI**: a terminal app for running a machine from a TOML config file
- **GUI**: a Qt front end that will allow to create, modify, save and open machines

## Table of Contents

- [Features](#features)
- [Machine Specification](#machine-specification)
- [Machine File Format](#machine-file-format)
- [Requirements](#requirements)
- [Project Structure](#project-structure)
- [Build](#build)
- [Core](#core)
- [CLI](#cli)
- [GUI](#gui)
- [Documentation](#documentation)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [Acknowledgements](#acknowledgements)
- [License](#license)

## Features

- Deterministic multi-tape Turing machines
- Configurable through TOML
- Infinite tapes
- Interactive CLI
- Batch execution
- Project serialization
- Modern C++23 implementation
- Cross-platform

## Machine Specification

MTMS implements deterministic multi-tape Turing machines following the formal definition

$$
M = (Q,\Sigma,\Gamma,\delta,q_0,\sqcup,F)
$$

where

- $Q$ is the finite set of states.
- $\Sigma$ is the input alphabet.
- $\Gamma$ is the tape alphabet.
- $q_0 \in Q$ is the initial state.
- $\sqcup \in \Gamma$ is the blank symbol.
- $F \subseteq Q$ is the set of accepting states.

The transition function is

$$
\delta : Q \times \Gamma^k \rightarrow
Q \times \Gamma^k \times \{L,R,S\}^k
$$

where \(k\) denotes the number of tapes.

## Machine File Format

MTMS uses a TOML-based domain-specific format to define Turing machines.

A machine file is composed of four main sections:

- `metadata`
- `machine`
- `states`
- `transitions`

`examples/bin_inc.toml` is a good starting point.

It defines:

- a 2-tape machine
- binary input alphabet
- a blank-aware tape alphabet
- states for copy, increment, and accept
- transitions that copy the input, then add 1

## Requirements

- C++23 compatible compiler (Clang 17+, GCC 13+ recommended)
- CMake 3.25+
- Ninja or Make (recommended: Ninja)
- toml++ (header-only, included in [include/toml++/toml.hpp](./include/toml++/toml.hpp))

## Project Structure

See [ARCHITECTURE.md](./ARCHITECTURE.md)

## Build

The project uses CMake.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

If you want only the CLI, leave `BUILD_CLI=ON` and `BUILD_GUI=OFF` as they are. Otherwise:

```bash
# build the GUI AND the CLI
cmake -S . -B build -DBUILD_GUI=ON -DCMAKE_BUILD_TYPE=Release

# only build the GUI
cmake -S . -B build -DBUILD_CLI=OFF -DBUILD_GUI=ON -DCMAKE_BUILD_TYPE=Release
```

## Core

The Core library is independent from both the CLI and GUI and contains the complete implementation of the Turing machine model:

- **Symbols** and the blank tape symbol
- **Alphabets** for input (`Sigma`) and tape work symbols (`Gamma`)
- **Strings** as validated symbol sequences
- **Tapes** with an infinite left/right model
- **States** with accept/non-accept flags
- **Transitions** for deterministic multi-tape rules
- **Projects** that load and save TOML machine files
- **TuringMachine** execution across multiple tapes

In practice, the core:

1. Reads a TOML blueprint
2. Validates the machine structure
3. Loads the input onto tape 0
4. Steps through transitions one move at a time
5. Stops on accept or halt/reject
6. Can export the loaded machine back to TOML

The simulator is deterministic, multi-tape, and supports blank cells, left/right/stay movement, and a safety step cap during `TuringMachine::run()`.

## CLI

`mtms-cli` is the current user-facing app.

It:

- loads a machine from `--config`
- loads the input string with `--input`
- runs interactively by default
- runs silently in `--batch` mode
- can export the loaded machine to a TOML file with `--output`

### Usage

```bash
mtms-cli --config examples/bin_inc.toml --input "1011" --batch
```

### Options

- `-c, --config <path>`: machine config TOML file
- `-i, --input <string>`: input string loaded onto tape 0
- `-b, --batch`: non-interactive mode, prints a summary line
- `-o, --output <path>`: export the current project to TOML
- `-h, --help`: show help

### CLI behavior

- **Interactive mode** shows a live dashboard with the current state, step count, and all tapes.
- **Batch mode** prints a compact result like `STATUS=ACCEPTED;STEPS=...;FINAL_STATE=...`.
- If the input contains symbols outside the input alphabet, it fails early.
- If the TOML is invalid, incomplete, or non-deterministic, it stops before execution.

## GUI

The GUI is currently under development.

The GUI will be the same machine viewer/editor on top of the core instead of a separate simulator. That means:

- load and inspect TOML machine files
- visualize states, tapes, and transitions
- step through execution visually
- show accept/reject clearly
- export the project back to TOML

In short: the core does the math, the CLI does the terminal workflow, and the GUI will be the friendlier visual front end.

## Documentation

The complete API reference is generated automatically using Doxygen and published through GitHub Pages.

|Documentation|Description|
|---------------|-------------|
|**API Reference**|[https://wh0crypt.github.io/mtms](https://wh0crypt.github.io/mtms)|
|**Examples**|[examples/](./examples/)|

## Roadmap

### Current

- [x] Core library
- [x] TOML project format
- [x] Multi-tape simulator
- [x] CLI application

### In Progress

- [ ] Qt graphical interface
- [ ] Doxygen documentation
- [ ] Unit tests

### Planned

- [ ] Graphical state editor
- [ ] Interactive debugger
- [ ] Breakpoints
- [ ] Undo/Redo
- [ ] Export diagrams

## Contributing

Contributions are welcome.

If you would like to contribute:

1. Fork the repository.
2. Create a feature branch.
3. Follow the project's coding style.
4. Open a Pull Request.

Please open an issue before proposing major changes.

See [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines.

## License

This project is licensed under the MIT License. See the full license text here:  
[LICENSE](./LICENSE)

## Acknowledgements

- [toml++](https://github.com/marzer/tomlplusplus) — modern header-only TOML parser used in the core configuration system.
- [TheLazyFerret](https://github.com/TheLazyFerret/turing-machine-simulator) — inspiration for the CLI workflow and interactive simulation design.
- [Francisco de Sande](https://portalciencia.ull.es/investigadores/80876/detalle?lang=en) and [Gara Miranda Valladares](https://portalciencia.ull.es/investigadores/81584/detalle?lang=en) — for teaching the [Formal Languages](https://en.wikipedia.org/wiki/Formal_language) and [Automata Theory](https://en.wikipedia.org/wiki/Automata_theory) course that inspired this project.
- [JFLAP](https://www.jflap.org/) — for inspiring the graphical representation and simulation of Turing machines.
