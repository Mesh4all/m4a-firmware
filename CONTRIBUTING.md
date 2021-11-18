# Contributing to Mesh4all

Welcome to contribute to Mesh4all, all ideas are welcome, feel free to share your requirements, comments, bugs or problems.
Remember that the main idea is to connect everyone and everything around us.

Check all the following sections according to the requirement you need.

- <a href="#titlegs">Getting Started</a>
- <a href="#titlebrfr"> Bug Reports and Feature Request</a>
- <a href="#titlecc"> Contributing Code</a>

<br>

<h2 id="titlegs"> Getting Started </h2>

---

if you are starting to work with mesh4all, you can read our documentation, and you will know all supported boards, and resources.

First Steps:
1. Cloning
2. Update submodules.

<br>
To start with m4ag-firmware, we need to clone the repository:

<br>

### 1. Cloning

#### Via SSH:
```sh
    git clone git@github.com:Mesh4all/m4a-firmware.git
```
#### Via HTTPS:
```sh
    git clone https://github.com/Mesh4all/m4a-firmware.git
```

*Note:* We recommend cloning through SSH, due to external dependencies of some module

<br>

### 2. Updating Submodules

Our firmware is based on [RIOT-OS](https://github.com/RIOT-OS/RIOT), so you have to apply updates from the RIOT repository within the m4ag firmware using the following command lines with git.

Enter to the m4ag-firmware directory
```sh
    cd m4a-firmware/
```
update your submodules
```sh
    git submodule update --init --recursive
```

<h2 id="titlebrfr">  Bug Reports and Feature Request </h2>

---
[This link allows you to generate a bug report or a feature request, you will take the option you want.](https://github.com/Mesh4all/m4a-firmware/issues/new/choose)

if you have started to work with the firmware, and some kind of [bugs or issues](https://github.com/Mesh4all/m4a-firmware/issues/new?assignees=&labels=&template=bug_report.md) has occurred during the process, please do not hesitate to report it describing it. (We recommend following the structure given by github).

Similarly, if you want to generate a [feature request](https://github.com/Mesh4all/m4a-firmware/issues/new?assignees=&labels=&template=feature_request.md) that could complement and help improve the firmware.

*Please, before generating a bug report or a feature request, verify that it's not present in the [list of issues](https://github.com/Mesh4all/m4a-firmware/issues)*

<br>

<h2 id="titlecc"> Contributing Code </h2>

---

if you think that your work will be a growth for the project, integrate in the repository m4ag-firmware, we recommend:

1. Fork the m4ag-firmware git repository.
2. Create your contribution branch.
3. Give a git push to your branch.
4. Generate a [Pull Request](https://github.com/Mesh4all/m4a-firmware/compare) (if you can't see your branch, try repeat the third step).

Thank you very much for your future contribution, we are now connected together in this great mesh around the world!!!.
