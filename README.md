![GitHub Repo stars](https://img.shields.io/github/stars/ArTicZera/NovaOS?style=flat-square)
![GitHub forks](https://img.shields.io/github/forks/ArTicZera/NovaOS?style=flat-square)
![GitHub Contributors](https://img.shields.io/github/contributors/ArTicZera/NovaOS?style=flat-square)
![License](https://img.shields.io/github/license/ArTicZera/NovaOS?style=flat-square)
![Issues](https://img.shields.io/github/issues/ArTicZera/NovaOS?style=flat-square)
![Pull Requests](https://img.shields.io/github/issues-pr/ArTicZera/NovaOS?style=flat-square)
![Downloads](https://img.shields.io/github/downloads/ArTicZera/NovaOS/total?style=flat-square)

<br />
<div align = "center">
  <a href = "https://github.com/ArTicZera/NovaOS">
    <img src = "logo.png" alt = "Logo" width = "120" height = "110">
  </a>
</div>

<h3 align = "center">
  NovaOS
</h3>

# 🌟 About NovaOS
NovaOS is a x86 protected mode operating system made in C and x86 Assembly by ArTic/JhoPro. It has a lot of features, including a GUI and a shell! This project is being actively developed as a personal journey to dive deeper into the world of operating systems. As such, NovaOS is constantly evolving, with new features and improvements being added over time. It’s a work in progress, but every update pushes the boundaries of what’s possible in this ambitious endeavor.


# ✨ Showcase Video
https://github.com/user-attachments/assets/83935faa-3283-430f-b691-2e1115c4d2a7


https://github.com/user-attachments/assets/6422312e-26cf-408d-883a-c8334bc61d39

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
- ✅ **Task State Segment (TSS)**
- ✅ **Network Driver**
- ✅ **Audio Driver**
- ❌ **Processes**
- ❌ **Multitasking**
- ❌ **Installation Setup**
- ❌ **User Documentation**

# ⚙️ Building
### 🧰 Necessary Components
i686 elf gcc and nasm
### 📄 Compiling
Run the make.bat script for windows (Not included bash)
### 🚀 Running
use `qemu-img create -f raw disk.raw 256M` and then `qemu-system-x86_64 -device sb16 -net nic,model=rtl8139 -net user -drive file=NovaOS.img,format=raw,if=ide,index=0 -drive file=disk.raw,format=raw,if=ide,index=2`

# 🤝 Contribute
NovaOS is an Open Source project, so everyone can contibute for it! You can help me starring/forking this project or help with code using pull requests (and optimize more!)
