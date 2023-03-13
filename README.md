<p align="center">
    <a href="https://mesh4all.org">
        <img height="300px" src="https://raw.githubusercontent.com/Mesh4all/m4a-firmware/main/doc/media/m4a-logo.png">
    </a>
</p>

<h1 align="center">M4a Firmware</h1>
<div align="center">

[![Compile test](https://img.shields.io/github/actions/workflow/status/Mesh4all/m4a-firmware/compilation_check.yml?label=compile+Test&branch=main)](https://github.com/Mesh4all/m4a-firmware/actions/workflows/compilation_check.yml)
[![build-docs](https://img.shields.io/github/actions/workflow/status/Mesh4all/m4a-firmware/build-docs.yml?label=documentation&branch=main)](https://mesh4all.github.io/m4a-firmware/)
![License Apache](https://img.shields.io/badge/license-Apache_License_2.0-blue)
</div>
<br>

<p align= "justify"> The firmware for connect everyone and everything around the world.
Expand all your wireless network limits and join to the big mesh.</p>

<p align= "justify">m4a-firmware is a software implemented in hardware able to provide enroute data between devices. The principal proposite is create only a wide wireless network.
m4a-firmware is based under <a href="https://github.com/RIOT-OS/RIOT"> RIOT OS</a> and we follow some of its design principles: energy-efficiency and modularity. We implemented our network vision approach and expanded capabilities to join some devices to networks.</p>

## Features

**m4a-firmware** provides these principals features:

- IPv6
- CoAp
- RPL

## Getting Started
Enjoy your firmware following these steps:

- [Clone the Repository and update the submodules.](https://github.com/Mesh4all/m4a-firmware/blob/main/CONTRIBUTING.md#titlegs)

-
    <summary>
    Compile
    </summary>
    for mesh boards

    ```sh
        make
    ```

    for other supported devices

    ```sh
        make BOARD=your_dev_name
    ```
-
    <summary>
        Flashing
    </summary>

    ```sh
        make flash
    ```
    Passing your boards name as the above point

    ```sh
        make BOARD=your_dev_name flash

    ```
- Open terminal and look what's happening.

```sh
    make term
```

## Support

We invite you to read our [code of conduct](CODE_OF_CONDUCT.md) but you probably won't read it, we know, it's very long and life is short but we summarice it in two short sentences:
- Respect the opinion of others
- Be polite and nice to other people

### Getting help

If you need help, you can contact us via [twitter](https://twitter.com/mesh4all) or [discord](https://discord.gg/DPUnkb7P)

### Issue tracker

If you find some bug or would you like to propose new features please open a issue:

https://github.com/Mesh4all/m4a-firmware/issues

Make sure it doesn't already exist, you can use the built-in search engine for that.

### Developers

New code or code fixing is always welcome, pull requests are open to everyone, we just ask that your contribution fits our [coding conventions](CODING_CONVENTIONS.md)

## License

 Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License

 You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  or in the [LICENSE](LICENSE) file, in the root folder of this repository.

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

See the License for the specific language governing permissions and limitations under the License.

Copyright (c) 2022 Mesh4all <mesh4all.org>

Licensed under the Apache License Version 2.0 Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center">
    <a href="https://mesh4all.github.io/m4a-firmware">Documentation</a> |
    <a href="https://t.me/mesh4all"> Telegram </a>|
    <a href="https://twitter.com/mesh4all"> Twitter </a>
</p>