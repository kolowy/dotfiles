#!/usr/bin/env sh

# Terminate already running bar instances
killall -q polybar

# Wait until the processes have been shut down
while pgrep -x polybar >/dev/null; do sleep 1; done

# Launch polybar
polybar main &

#!/usr/bin/env bash

# Launch the bar
#STYLE="default"

#bash "$HOME"/.config/polybar/"$STYLE"/launch.sh
