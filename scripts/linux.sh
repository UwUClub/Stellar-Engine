#! /bin/bash

# get package manager
if [ -x "$(command -v apt-get)" ]; then
    sudo apt-get install -y libboost-all-dev
elif [ -x "$(command -v pacman)" ]; then
    sudo pacman -S --noconfirm boost
else
    echo "No package manager found. Exiting."
    exit 1
fi
