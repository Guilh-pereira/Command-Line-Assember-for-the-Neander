# Neander Assembler

A lightweight command-line assembler for the Neander processor architecture, developed in C. This tool converts Neander assembly source files (`.asm`) into executable binary memory files (`.mem`).

## Features
* Validates and translates Neander mnemonics (`NOP`, `STA`, `LDA`, `ADD`, `OR`, `AND`, `NOT`, `JMP`, `JN`, `JZ`, `HLT`).
* Automatically generates the mandatory 4-byte header (`0x03 0x4E 0x44 0x52`) for Neander compatibility.
* Formats output into structured 2-byte binary sequences.

## How to Compile and Run

### Linux / WSL
1. Open your terminal and compile the code:
   ```bash
   gcc montador.c -o montador
   ```
2. Run the assembler by passing your assembly file as an argument:
   ```bash
   ./montador <input_file>.asm
   ```

### Windows (PowerShell / CMD)
1. Open your terminal and compile the code:
   ```powershell
   gcc montador.c -o montador.exe
   ```
2. Run the assembler by passing your assembly file as an argument:
   ```powershell
   .\montador.exe <input_file>.asm
   ```

## Output
The program will output a binary file with the exact same name as the input file, but with a `.mem` extension, ready to be loaded into a Neander simulator.

---
