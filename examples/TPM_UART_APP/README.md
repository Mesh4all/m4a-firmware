## Uart example

this is an uart example for this example i used esp32-wrover-kit and one bluetooth device hc-06

to configure the bluetooth device write in the shell:

1) AT for inter at mode
2) AT+RESET for reset the device
3) AT+PIN password  for change password
4) AT+NAME name for change name

to send messages through uart to the bluetooth device write `send message` since the shell

to compile this example:

`make -C examples/TPM_UART_APP  BOARD=your_board  PORT=/dev/your_port flash term`