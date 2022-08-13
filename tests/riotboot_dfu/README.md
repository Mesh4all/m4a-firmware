Riotboot_dfu
============

To test the riotboot dfu you need to connect both usb ports (EDBG and Target) on a samr21-xpro
board. It's necessary because the bootloader installation and the upload firmware are two
process made it in different ports.

When you install the bootloader you will get advantage to upload any firmware, exampler or test
application, using the `make update` rule. So, you could define a `BOARD` if you want to test
in another usb dfu supported boards.

Usage
=====
Run the auto run test, you will install the `riotboot_dfu` bootloader and then you
will auto upload the `examples/blinking/` application. The expected results is
load the correctly firmware or application and auto run it after upload it.

Only you need to execute the following make rule.
```
make -C tests/riotboot_dfu test-boot
```
