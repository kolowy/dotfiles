#!/bin/bash

icon="/home/paul/.config/bin/lock.png"
img=$(mktemp /tmp/XXXXXXXXX.png)

import -window root $img
convert $img -scale 5% -scale 2000% $img
convert $img $icon -fill black -gravity center -composite $img

# suspend the message display
pkill -u "$USER" -USR1 dunst

i3lock -n --indicator -k -f -i $img \
    --inside-color=ffffff1c --insidever-color=0000001c \
    --insidewrong-color=0000001c --time-color=ffffff99 --date-color=ffffff99 \
    --verif-color=aaaaff99 --wrong-color=ffaaaa99 -i $img

# resume message
pkill -u "$USER" -USR2 dunst

# del the img
rm $img


