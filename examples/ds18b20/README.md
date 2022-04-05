<h2 align=center> ds18b20 </h2>

### Getting Started:

Follow the next steps:

<br>

### Go to the example folder

```sh
cd examples/esp-wroom32/ds18b20
```

### Compile your code and flash it

```sh
make flash term
```

Check your code to understand functionalities

### First
init the ds18_pin
```c
init_temperature_sensor(pin);
```

### Last steps
Check the temperature

```c
get_temperature_value(&var_temperature); // var_temperature has to defined previously as int16_t variable //
```
