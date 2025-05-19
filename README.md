# 🎓 jigglyOS — Operating System Simulator in C

Welcome to **jigglyOS**, a terminal-based Operating System Simulator built entirely in **C**, developed as a final project for the Operating Systems Lab.

This simulator replicates the foundational behavior of a real OS — including process creation, scheduling, multitasking, and resource management — all within a controlled CLI environment (no hardware-level access required).

---

## 📘 Overview

**jigglyOS** mimics essential OS-level operations such as:
- Simulated boot sequence
- Independent task terminals
- Memory and CPU core management
- Process scheduling and synchronization
- Kernel/User mode separation

The system is configured and launched entirely through a command-line interface and allows users to simulate multiple concurrent tasks using virtualized resources.

---

## ⚙️ Main Features

### 🧰 OS Boot & Configuration
- Custom OS banner and animated boot sequence  
- User-defined system specs at startup:
  - RAM size  
  - Hard Drive capacity  
  - Number of CPU cores  
- Resources dynamically managed per task launch

### 👥 User Mode vs Kernel Mode
- **User Mode**: Launch, switch, and minimize tasks  
- **Kernel Mode**: System-level operations such as terminating all running processes

### 🧵 Process Management
- Each task runs in its own pseudo-terminal
- Validates available memory and disk space before launching tasks
- Automatic resource cleanup on task termination
- **FCFS (First-Come First-Served)** scheduling
- Thread and process synchronization using **semaphores**

---

## 📂 Included Tasks (15 Simulated Applications)

Each application runs as an independent simulated process:

| Task               | Description                                 |
|--------------------|---------------------------------------------|
| Notepad            | Simple text editor with save functionality  |
| Calculator         | Basic arithmetic operations                 |
| Create File        | Generate a new file                         |
| Copy File          | Duplicate an existing file                  |
| Rename File        | Rename selected file                        |
| Delete File        | Permanently remove a file                   |
| Tic Tac Toe        | Two-player console game                     |
| Temperature Convert| Celsius ↔ Fahrenheit                        |
| Reverse String     | Outputs reversed version of input           |
| Letter Counter     | Counts letters in a given string            |
| Age Calculator     | Calculates age based on birth year          |
| Play Song          | Simulates audio via system beeps            |
| Number Guessing    | Guess a randomly generated number           |
| Gender Guessing    | Simple guessing game based on user answers  |
| File Download      | Simulated download with progress bar        |

---

## 🛠️ Technologies Used
- **C (GCC)**
- **POSIX Threads**
- **Semaphores**
- **Ncurses** for terminal UI
- **Ubuntu (via VMware)**

---

## 🚀 How to Build & Run

### Prerequisites
- GCC compiler
- ncurses library
- pthread support

### Compilation

```bash
gcc project.c -lncurses -o project -pthread
