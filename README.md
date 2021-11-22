<p align="center">
    <a href="https://mesh4all.org">
        <img height="300px" src="doc/media/m4a-logo.png">
    </a>
</p>

<h1 align="center">M4a Firmware</h1>
<div align="center">
    <a href="https://github.com/Mesh4all/m4a-firmware/actions/workflows/compilation_check.yml">
        <img src="https://github.com/Mesh4all/m4a-firmware/actions/workflows/compilation_check.yml/badge.svg">
        </img>
    </a>
    <a href="http://www.apache.org/licenses/LICENSE-2.0">
        <img src="https://img.shields.io/badge/license-Apache_License_2.0-blue">
        </img>
    </a>
</div>
<br>


<p align= "justify"> The firmware for connect everyone and everything around the world.
Expand all your wireless network limits and join to the big mesh.</p>

<p align= "justify">m4a-firmware is a software implemented in hardware able to provide enroute data between devices. The principal proposite is create only a wide wireless network.
m4a-firmware is based under <a href="https://github.com/RIOT-OS/RIOT"> RIOT OS</a> and we follow some of its design principles: energy-efficiency and modularity. We implemented our network vision approach and expanded capabilities to join some devices to networks.</p>

## FEATURES

m4a-firmware provides these principals features:

- IPv6
- CoAp
- RPL

## Getting Started
Enjoy your firmware following these steps:

- [Clone the Repository and update the submodules.](https://github.com/Mesh4all/m4a-firmware/blob/main/CONTRIBUTING.md#titlegs)

 - <details close style=>
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
    </details>

- <details close>
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
    </details>

- Open terminal and look what's happening.

```sh
    make term
```

<p align= "justify">Enjoy your firmware and if you have some failure or bugs please present <a href= 'https://github.com/Mesh4all/m4a-firmware/issues'> issue </a> and we'll be working in resolve it. Remember check if another user had the same issue.

For more information follow us in [twitter](https://twitter.com/mesh4all) and join to us and our [discord](https://discord.gg/DPUnkb7P) channel
## License

 <p align= "justify">Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.</p>

 You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  or in the [LICENSE](LICENSE) file, in the root folder of this repository.

<p align= "justify">Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.</p>

See the License for the specific language governing permissions and limitations under the License.

Copyright (c) 2021 Mesh4all <mesh4all.org>

<p align= "justify">Licensed under the Apache License Version 2.0 Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.</p>

<br>

<p align="center">
    <a href='#'>Documentation</a> |
    <a href='#'> Discord </a> |
    <a href='#'>Installation</a>
</p>
