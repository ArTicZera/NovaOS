# 🌟 About NovaOS
NovaOS is a x86 protected mode operating system made in C and x86 Assembly by ArTic/JhoPro. It has a lot of features, including a GUI and a shell! This project is being actively developed as a personal journey to dive deeper into the world of operating systems. As such, NovaOS is constantly evolving, with new features and improvements being added over time. It’s a work in progress, but every update pushes the boundaries of what’s possible in this ambitious endeavor.


# ✨ Showcase Video
https://github.com/user-attachments/assets/a0811d8d-ad4b-4c1e-9a27-c164b22fbce8

# ⚠️ Important
NovaOS will only work on QEMU, so it can have more easily and readable operations and functions.

# 🛠️ Current Progress
- ✅ **VBE Support (640x480 8bpp)**
- ✅ **Global Descriptor Table (GDT)**
- ✅ **Entering Protected Mode**
- ✅ **Fonts and Print Functions**
- ✅ **Interrupts (IDT, ISR, IRQ)**
- ✅ **Keyboard Driver**
- ✅ **Mouse Driver**
- ✅ **Memory Management**
- ✅ **File System**
- ✅ **Shell**
- ✅ **Graphical Interface (GUI)**
- ✅ **ELF Loader**
- ❌ **Network Driver**
- ❌ **Processes**
- ❌ **Multitasking**
- ❌ **User Documentation**

# ⚙️ Building
### 🧰 Necessary Components
i686 elf gcc and nasm
### 📄 Compiling
Run the make.bat script for windows (Not included bash)
### 🚀 Running
use `qemu-system-x86_64 -drive format=raw,file="NovaOS.img"`

# 🤝 Contribute
NovaOS is an Open Source project, so everyone can contibute for it! You can help me starring/forking this project or help with code using pull requests (and optimize more!)
