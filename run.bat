qemu-img create -f raw disk.raw 256M
qemu-system-x86_64 -device sb16 -net nic,model=rtl8139 -net user -drive file=NovaOS.iso,format=raw,if=ide,index=0 -drive file=disk.raw,format=raw,if=ide,index=2