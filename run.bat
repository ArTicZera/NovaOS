qemu-img create -f raw disk.raw 256M
qemu-system-x86_64 -device sb16 -netdev user,id=net0 -net nic,model=rtl8139,netdev=net0 -object filter-dump,id=f1,netdev=net0,file=net.pcap -serial stdio -drive file=NovaOS.iso,format=raw,if=ide,index=0 -drive file=disk.raw,format=raw,if=ide,index=2
