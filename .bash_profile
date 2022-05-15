#
# ~/.bash_profile
#

[[ -f ~/.bashrc ]] && . ~/.bashrc

## Browser
export BROWSER="/usr/bin/brave"


## Run startx when tty1
if [[ -z $DISPLAY ]] && [[ $(tty) = /dev/tty1 ]];
then 
    ./.config/bin/random-lyrics/random-lyrics.sh $HOME/.config/bin/random-lyrics/eminem 200
    exec startx > /dev/null 2>&1
fi

# opam configuration
test -r /home/paul/.opam/opam-init/init.sh && . /home/paul/.opam/opam-init/init.sh > /dev/null 2> /dev/null || true
