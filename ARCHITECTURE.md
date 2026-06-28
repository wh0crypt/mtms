# MTMS Architecture

This document uses **PlantUML** blocks so the architecture can be rendered in GitHub Pages pipelines that support PlantUML conversion.

## Static architecture (component/class view)

```plantuml
@startuml
title MTMS - High-Level Architecture
skinparam componentStyle rectangle
left to right direction

package "CLI Layer" {
  [mtms-cli\n(src/cli/main.cpp)] as cliMain
  [Program\n(parse args)] as cliProgram
  [CLI Session\n(run_cli_session)] as cliSession
}

package "Core Layer (mtms-core)" {
  [Project\n(load/save TOML)] as project
  [TuringMachine\n(step/run)] as tm
  [Tape] as tape
  [Transition] as transition
  [State] as state
  [Alphabet / Symbol / String] as alpha
}

database "TOML Config\nexamples/*.toml" as tomlFile

cliMain --> cliProgram : parse_args()
cliMain --> project : load_project(path)
project --> tomlFile : parse_file()
project --> tm : build machine
cliMain --> cliSession : run_cli_session(machine,input)
cliSession --> tm : load_input(), step()
tm --> tape : read/write/move
tm --> transition : transition lookup
transition --> state
tm --> alpha
project --> tm : save_project()

note right of tm
Deterministic multi-tape runtime.
Stops on accept/no rule/max steps.
end note
@enduml
```

## Execution flow (sequence "animation-ready" diagram)

```plantuml
@startuml
title MTMS CLI Execution Flow
autonumber

actor User
participant "mtms-cli\n(main)" as Main
participant "Program" as Program
participant "Project" as Project
participant "TuringMachine" as TM
participant "Tape[0..k-1]" as Tapes

User -> Main : mtms-cli -c machine.toml -i "1011" [--batch]
activate Main
Main -> Program : parse_args(argc, argv)
activate Program
Program --> Main : Options
deactivate Program

Main -> Project : load_project(config)
activate Project
Project --> Main : machine ready / error
deactivate Project

Main -> TM : load_input(input)
activate TM
TM -> Tapes : initialize tapes (input on tape0)
Tapes --> TM : ready

loop While TM::step() == true
  TM -> Tapes : read current symbols
  Tapes --> TM : tuple(symbols)
  TM -> TM : find transition(state, tuple)
  TM -> Tapes : write + move all heads
  Tapes --> TM : updated tapes
end

alt accept state reached
  TM --> Main : ACCEPTED
else no valid transition / halt
  TM --> Main : REJECTED
end

deactivate TM
Main --> User : status + steps + final_state
deactivate Main
@enduml
```
