#!/bin/bash

sudo apt install make ttf-ancient-fonts-symbola build-essential libx11-dev libxft-dev libxinerama-dev libxcb1-dev libx11-xcb-dev libxcb-res0-dev

echo "IMPORTANT: If using Ubuntu, follow the steps commented in this script below"

# TODO: the following has to be done in Ubuntu too
#sudo apt install dwm
#sudo cp /usr/share/xsessions/dwm.desktop{,.bak}
#sudo apt remove --purge dwm
#sudo mv /usr/share/xsessions/dwm.desktop{.bak,}
#
#sudo apt install lightdm
#
#reboot, select dwm XSession when logging in

# Backlight control
sudo apt install light
sudo chmod +s /usr/bin/light

# Notifications controller
sudo apt install dunst
