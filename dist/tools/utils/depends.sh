#!/bin/bash
sudo add-apt-repository ppa:npalix/coccinelle
sudo apt update
sudo apt install codespell cppcheck vera++ flake8 uncrustify pcregrep shellcheck doxygen coccinelle
sudo apt upgrade

echo "All dependencies were Installed"
