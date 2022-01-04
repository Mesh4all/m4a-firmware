### Moisture App

this is a basic example of how read data in an analog moisture sensor, you will must take in considered that i used
an esp32 with a ref de 10 bits and vhref de 1.1 vol.
### How to use
make -C examples/moisture_sensor_hw390 BOARD=your_board PORT=/dev/tty.usbserial-142201 flash term`
### Expected result
```c
2022-01-04 10:30:03,533 # the moisture sensor is percent 23
2022-01-04 10:30:08,525 # the moisture sensor is percent 23
2022-01-04 10:30:13,533 # the moisture sensor is percent 23
2022-01-04 10:30:18,525 # the moisture sensor is percent 23
2022-01-04 10:30:23,533 # the moisture sensor is percent 23
2022-01-04 10:30:28,525 # the moisture sensor is percent 23
2022-01-04 10:30:33,532 # the moisture sensor is percent 23
2022-01-04 10:30:38,525 # the moisture sensor is percent 23
2022-01-04 10:30:43,533 # the moisture sensor is percent 22
2022-01-04 10:30:48,525 # the moisture sensor is percent 23
2022-01-04 10:30:53,533 # the moisture sensor is percent 22
2022-01-04 10:30:58,525 # the moisture sensor is percent 22
2022-01-04 10:31:03,533 # the moisture sensor is percent 22
2022-01-04 10:31:08,540 # the moisture sensor is percent 21
2022-01-04 10:31:13,533 # the moisture sensor is percent 22
2022-01-04 10:31:18,540 # the moisture sensor is percent 22
2022-01-04 10:31:23,533 # the moisture sensor is percent 21
```
