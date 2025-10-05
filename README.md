# SCHizOS

![Spoiler image](https://afghangoat.hu/img/img52.png)

My own operating system which I made from scratch using Assembly and C as the main technology.

## Main features

- Tasks
- Programs
- Custom memory management
- Custom file systems (Fat12 and VFS)
- Shell commands
- Kernel, 32bit protected mode
- Custom standard C libraries implemented from scratch
- Input-output handling
- Load external content (files)
- Sound playing
- Multiple keyboard language settings
- MOTD

You will need an elf i386 GCC to compile this, bud.
I got you a compiled version of the software. So make sure to install the qemu x86 systems package.
The documentation of the OS will go in this file... in a nice day sometimes in the future.

## Usage

Use Qemu if you want to have an easy time.
The emulation was tested in linux only.

Run:
`./run.sh`