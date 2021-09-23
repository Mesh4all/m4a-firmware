[![Compile test](https://github.com/Mesh4all/m4a-firmware/actions/workflows/compilation_check.yml/badge.svg)](https://github.com/Mesh4all/m4a-firmware/actions/workflows/compilation_check.yml)

# m4a-firmware
Firmware for all available devices.

**Important Notice**

> This README is a work in progress (WIP), some sections may be incomplete or need some extra work (TODO)

## Make it work

we hope we can make this a little easier, but in the meantime you must to follow the next steps

### Required dependencies and enviroment

- [ ] TODO

### Clone the source code

```sh
git clone git@github.com:Mesh4all/m4a-firmware.git
```

```sh
cd m4a-firmware
```

```sh
git submodule update --init --recursive
```

### Compile

Assuming your device is a `m4a-mb` from Meshall, then execute

```sh
make
```
For other supported devices it's necessary to specify the device's name with the `BOARD` variable.
As example, `BOARD=your_dev_name`:

```sh
make BOARD=your_dev_name
```

The compiling process can take a while depending of your computer's features, so keep calm

### Flashing

```sh
make flash
```

[OR] passing your board's name as the above point

```sh
make BOARD=your_dev_name flash
```
### Open a terminal and look what's happening

```sh
make term
```

Also, as before, you must pass the name of the board for devices other than those described in this document.

### Enjoy with moderation

Yes, now you have a basic stuff working on your desktop but the code is in a early stage and some parts or things can change very fast.

## License

 Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.

 You may obtain a copy of the License at
 
  http://www.apache.org/licenses/LICENSE-2.0

  or in the root folder of this repository

  https://github.com/Mesh4all/m4a-firmware/LICENSE


Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and limitations under the License.

Copyright (c) 2021 Mesh4all <mesh4all.org>

Licensed under the Apache License Version 2.0
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

