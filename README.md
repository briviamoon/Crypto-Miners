# Crypto-Miners

Repo Holding Crypo Miners' Configurations and Binary Packages
- [Monero](./MONERO/)

# Binary Programs

---
[initialize](./Initialize)

# Windows Startup Program with Resource Management

## Overview

This program allows you to automatically start a specified program when your Windows system boots. It also adjusts the resource usage of the program based on system activity and hides the program's folder.

## Features

- **Automatic Startup:** The program creates a scheduled task to launch your specified program when Windows starts.

- **Resource Management:** It adjusts the resource usage of the program based on system activity. When the computer is in use, it runs the program with lower resource usage (25%), and when the computer is idle, it runs the program with higher resource usage (75%).

- **Folder Hiding:** The program hides the folder containing the specified program and the folder of the targeted program to maintain a low profile.

## Prerequisites

- Windows operating system
- C compiler (e.g., Visual Studio, MinGW, or another suitable compiler)
- Basic knowledge of C programming

## Usage

1. **Clone the Repository:** Clone or download this repository to your computer.

2. **Compile the Code:** Compile the C program using a C compiler.

   ```sh
   gcc Initialize.c -o Initializer
   ```

3. **Execute the Program:** Run the compiled program. This will set up a scheduled task to run the specified program at system startup and adjust its resource usage based on system activity.
   ```sh
   Initializer <PATH-TO-YOUR-.EXE-FILE>
   ```

4. **Background Operation:** The program will run in the background indefinitely. To stop it, you will need to manually terminate it from the command prompt or task manager.

## Customization

- You can customize the program by modifying the variables in the C code to match your specific requirements. For example, you can change the paths, adjust resource management settings, or modify the resource management logic.

- The code dynamically determines the parent folder of the running program and the parent folder of the targeted program.

## Warning

- Modifying Windows system behavior and resource management settings can have unintended consequences. Use this program responsibly and understand the potential effects on your system.

## License

This code is provided under the [MIT License](LICENSE).

---
