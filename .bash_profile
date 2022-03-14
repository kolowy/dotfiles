#
# ~/.bash_profile
#

[[ -f ~/.bashrc ]] && . ~/.bashrc

# opam configuration
test -r /home/paul/.opam/opam-init/init.sh && . /home/paul/.opam/opam-init/init.sh > /dev/null 2> /dev/null || true

if [ -f "/usr/bin/random-lyrics" ] || [ -f "$HOME/.local/bin/random-lyrics" ];
then
random-lyrics $HOME/.config/random-lyrics/eminem 100
fi

startx > /dev/null 2>&1
