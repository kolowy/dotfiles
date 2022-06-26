#!/usr/bin/env sh

OPTIND=1


case "$BLOCK_BUTTON" in
  1|2|3) 
    echo "test"
    exec ~/.config/rofi/applets/applets/powermenu.sh &
esac
