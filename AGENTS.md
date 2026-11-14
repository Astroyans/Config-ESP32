# AGENTS.md

Guidance for AI coding agents working in this repo.

## Purpose

ESP32 firmware for a NASA hackathon project. Today it is a single-node MicroSD route storage test. The goal is a mesh of ESP32 nodes linked to a Raspberry Pi.

`ROADMAP.md` is the source of truth for tasks. When you finish a task, tick its checkbox there. Add new tasks to `ROADMAP.md`, not elsewhere (not in README, code comments, or new files).

## Hardware

- Board: ESP32 DevKit V1 (PlatformIO env `esp32doit-devkit-v1`).
- Storage: SPI MicroSD module with a FAT32 card. See `README.md` for the full wiring table; do not duplicate it here.
- Serial: 115200 baud.

## Layout

- `platformio.ini`: PlatformIO environment
- `src/`: firmware (`src/main.cpp`)
- `README.md`: wiring, setup, how to run
- `ROADMAP.md`: phased task list and open decisions
- `AGENTS.md`: this file

## Commands

- `pio run`: build
- `pio run -t upload`: flash the board
- `pio device monitor`: serial monitor
- `pio run -t clean`: clean build output

There are no unit tests yet. Verification today is a successful `pio run`. Hardware behavior must be checked manually on a real board through the serial monitor. If you cannot do that, say so instead of claiming it works.

## Conventions

- C++ with the Arduino framework.
- English for code, comments, Serial messages, and docs.
- Keep changes minimal and focused.
- Do not add `lib_deps` without a stated justification.
- Check return values of SD and IO calls, and report errors over Serial.
- Never block forever silently. Print a message if waiting or retrying.
- Define pin numbers as named constants, not magic numbers.

## Workflow

- Use conventional commits (the repo uses `feat:`, `docs:`).
- Do not commit build output (`.pio/`).
- Update `README.md` when wiring or commands change.

## Open decisions

See "Open Decisions" in `ROADMAP.md`. Do not unilaterally choose the mesh technology or the Raspberry Pi protocol or link. Present options and let the user decide, then record the outcome in `ROADMAP.md`.
