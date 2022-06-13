#!/usr/bin/env bash

##
## Author : Paul Dufour
## Mail : paul.paul.dufour@gmail.com
## GITHUB @kolowy
##


style=$HOME/.config/rofi/config.rasi 

path=$(pwd)

findfiles()
{
    rofi_cmd="rofi -theme $style"

    options=".."

    for f in $path/*; do
        if [[ -d $f ]]; then
            options=$options"\n "${f##*/};
        else
            options=$options"\n "${f##*/};
        fi;
    done;

    chosen=$(echo -e "$options\nEXIT" | $rofi_cmd -p "RofiFile : $path" -dmenu -selected-row 0)

    echo $path/$chosen
    chosen=${chosen##* }
    echo $path/$chosen
    if [[ -d $path/${chosen##* } ]]; then
        if [[ $chosen == ".." ]]; then
            path=${path%/*}
        else
            path=$path/$chosen
        fi;
        findfiles
    else
        if [[ $chosen == "EXIT" ]]; then
            exit 1;
        fi;
        # xdg-open $path/$chosen

        



        defdesk=$(xdg-mime query default $(echo $(xdg-mime query filetype "${path}/${chosen}")))
        defapp=$(grep ^Exec /usr/share/applications/${defdesk} | sed -e 's/^[^=]*=//g' | sed -e 's/%.*//g')

        allapps=$(for d in $(echo $PATH | sed 's/:/\n/g'); do cd $d; ls; done)
        run=$(rofi -modi drun | rofi -dmenu -p "ouvrir avec ")
        # run=$(echo -e "${defapp}\n${allapps}" | \
                # rofi -dmenu -p "ouvrir ${path}/${chosen} avec ")
        ${run} "${path}/${chosen}" && exit


    fi
}

findfiles

