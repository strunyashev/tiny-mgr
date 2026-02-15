# tiny-mgr

A lightweight, "clean-code" system monitor for Linux written in pure C. 

## Why?
I wrote this project just for fun and for learning purposes. I'm a 12-year-old aspiring developer exploring the depths of Linux and the C language. I wanted to see if I could build a working system utility that talks directly to the kernel without being bloated.

## Features
- **CPU Usage**: Dynamic calculation of processor load by parsing /proc/stat.
- **GPU Usage**: Real-time utilization tracking for NVIDIA cards.
- **Thermal Monitoring**: Instant CPU/GPU temperature readings with color-coded alerts (Red/Green).
- **Memory Usage**: Visual progress bar for RAM.
- **Disk Health**: Quick look at your root partition usage.
- **Network Speeds**: Live Download/Upload tracking (KB/s).
- **Zero Bloat**: No heavy libraries, just pure C and ANSI escape codes.
- **ANSI UI**: Minimalist, flicker-free interface using escape sequences.

## Installation
You can compile and install it with a single flag:

```bash
gcc -O3 main.c -o tiny-mgr
./tiny-mgr --install
```

Now you can just type tiny-mgr anywhere in your terminal!

## Technical Stuff
- **Language**: C
- **OS**: Linux (Tested on Ubuntu WSL)
- **Concepts used**: File I/O (parsing /proc and /sys), ANSI escape sequences for UI, Process pipes (popen).

## Disclaimer
This is a learning project. Since Linux is vast and hardware varies, some sensors might behave differently depending on your setup. It's not meant to replace htop, but it sure looks cooler in a minimalist setup!


Made with ☕ and curiosity by a 12-year-old coder.
