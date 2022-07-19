#
# ~/.bashrc
#

# If not running interactively, don't do anything
[[ $- != *i* ]] && return

alias grep='grep --color=auto'
alias ls='ls --color=always'

alias ls='ls --color=auto'
PS1='[\u@\h \W]\$ '

# shutdown alias
alias shutdown='poweroff'

## auto cd path
shopt -s autocd


# opam
eval $(opam env)

# z
. ~/.config/bin/z.sh

PROMPT_COMMAND=' '

export EDITOR=vim


export COLOR_NC='\e[0m' # No Color
export COLOR_BLACK='\e[38;5;0m'
export COLOR_GRAY='\e[38;5;8m'
export COLOR_RED='\e[38;5;88m'
export COLOR_LIGHT_RED='\e[38;5;160m'
export COLOR_GREEN='\e[38;5;34m'
export COLOR_LIGHT_GREEN='\e[38;5;84m'
export COLOR_BROWN='\e[38;5;94m'
export COLOR_YELLOW='\e[38;5;226m'
export COLOR_BLUE='\e[38;5;27m'
export COLOR_LIGHT_BLUE='\e[38;5;117m'
export COLOR_PURPLE='\e[38;5;55m'
export COLOR_LIGHT_PURPLE='\e[38;5;135m'
export COLOR_CYAN='\e[38;5;87m'
export COLOR_LIGHT_CYAN='\e[38;5;123m'
export COLOR_LIGHT_GRAY='\e[38;5;248m'
export COLOR_WHITE='\e[38;5;255m'

export LESS_TERMCAP_mb=$'\e[1;32m'
export LESS_TERMCAP_md=$'\e[1;32m'
export LESS_TERMCAP_me=$'\e[0m'
export LESS_TERMCAP_se=$'\e[0m'
export LESS_TERMCAP_so=$'\e[01;33m'
export LESS_TERMCAP_ue=$'\e[0m'
export LESS_TERMCAP_us=$'\e[1;4;31m'

UC=$COLOR_WHITE                      # user's color
[ $UID -eq "0" ] && UC=$COLOR_RED    # root's color

PS1="\n\[${UC}\]\h\[${COLOR_BLUE}\]@\[${COLOR_LIGHT_BLUE}\]\u \[${COLOR_LIGHT_RED}\]\[${COLOR_LIGHT_GREEN}\]\w\n\[${COLOR_GREEN}\]$\[${COLOR_NC}\] "


weather(){
    curl -s "wttr.in/$1?m3"; # arg : Paris : to have a location 
}
moon()
{
    curl -s "wttr.in/moon$1" #arg : @2020-06-13 to have the moon of my brithday
}

function count() 
{
    total=$1
    for ((i=total; i>0; i--)); do sleep 1; printf "Time left : $i s \r"; done
    echo -e "\a   TIME UP !!!!"
}
function calc()
{
    echo $0 | ./Documents/S2/prog/paul.dufour-2026-abacus/Abacus/bin/Debug/net5.0/Abacus
}
