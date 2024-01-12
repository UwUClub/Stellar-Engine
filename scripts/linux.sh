#! /bin/bash

# get package manager
if [ -x "$(command -v apt-get)" ]; then
    package_manager="apt-get install"
    no_confirm="-y"
elif [ -x "$(command -v yum)" ]; then
    package_manager="yum insta;;"
    no_confirm="-y"
elif [ -x "$(command -v pacman)" ]; then
    package_manager="pacman -S"
    no_confirm="--noconfirm"
else
    echo "No package manager found. Exiting."
    exit 1
fi

# install packages
sudo $package_manager $no_confirm git boost spdlog catch2
