## External led example

this is a simple test where i turn on and turn off an external led in esp32 for this example i used esp32-wrover-kit

## How to test it

first you must be defined the pins you need in this example i used the gpio 23

make -C examples/moisture_sensor_hw390  BOARD=your_board  PORT=/dev/your_port flash term