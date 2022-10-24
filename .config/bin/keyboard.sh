keyboard=$(setxkbmap -query | grep -oP 'layout:\s*\K([\w,]+)')

if [[ $keyboard == 'us' ]]
then
    setxkbmap fr
else
    setxkbmap us
fi 
