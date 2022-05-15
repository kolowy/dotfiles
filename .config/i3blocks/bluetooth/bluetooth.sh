#!/usr/bin/env sh

WIDTH=${WIDTH:-200}
HEIGHT=${HEIGHT:-200}

OPTIND=1


case "$BLOCK_BUTTON" in
  1|2|3) 

	# the position of the upper left corner of the popup
	posX=$(($BLOCK_X - $WIDTH + 50))
	posY=$(($BLOCK_Y + 10))

        TEST="aaa"
        TEXT=$(bluetoothctl devices | cut -f2 -d' ' | while read uuid; do
        bluetoothctl info $uuid; done | grep -e "Name" | cut -c 7-)
        echo $TEXT
esac
