#!/usr/bin/env sh

WIDTH=${WIDTH:-200}
HEIGHT=${HEIGHT:-200}

OPTIND=1


case "$BLOCK_BUTTON" in
  1|2|3) 

	# the position of the upper left corner of the popup
	posX=$(($BLOCK_X - $WIDTH + 50))
	posY=$(($BLOCK_Y + 10))

        urxvt -e sh -c \
            "cd ~/Documents/S2/prog/tp11-paul.dufour ; bash ; git pull"
esac
