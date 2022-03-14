things to include

1. relative line numbers
2. use c
3. surround
4. primer on vim modes

1. use / to search for word
2. use cgn to delete next instance of word search


# Switching From Normal Mode to Edit Mode and Back
So far we have only been navigating with normal mode. There are several methods
to enter edit mode.

* i - edit mode at the beginning of the cursor, this has the effect of moving back.
* I - edit mode at the beginning of the line.
* a - edit mode at the end of the cursor, this is what most people are familiar with.
* A - edit mode at the end of the line.
* o - insert line below and change to edit mode.
* O - insert line above and change to edit mode.

To swtich back to Normal Mode press the <span class='kbd'>Esc</span> key.

Selections in regular text editors either uses mouse drags or <span
class='kbd'>Shift</span>+direction keys to highlight and select. The equivalent
in Vim is the visual selection mode, this is activated by using the <span
class='kbd'>v</span> key.

Vim has a more powerful way to select "things" in the editor. These are text
objects. Instead of choosing a start and end point, one can use context
sensitive "text objects" as a suffix for an actions like selection, deletion,
and copying. These are perhaps the most used patterns in Vim, so take some time
to familiarize these commands. It is useful to think of the following commands
as below,

* iw - inner word
* aw - a word

Combining with an action prefix such as <span class='kbd'>v</span>, into viw and
vaw will select the word under the cursor

* viw - select inner word
* vaw - select inner word including trailing space(leading space if at eol)

Text object pattern
action + in/out + object type


* v, c, y, d
* i, a
* w, ", ', (, {, [, <, t

* i" - inner double quotes
* a" - including double quotes

# VS Game Mode
battlefield of code using git commits
whoever can push first wins or whoever kills the other player
removing characters/words on your field damages enemy
inserting characters adds health.

each side has 10 lines of vim tasks they can fire at each other.
for example,
delete line

1. get text = original text
2. add line to original_text = new_text
3. task = delete line in new_text =
4. add line to original_text = pasted_text
5. task = paste line in new_text


a(original)
b
c

a(new text)
b
c
added(delete here)

added(paste here)(color this and low contrast text)
a
b
c


Vscode overwrite settings
trim empty line settings

graphs
current
user average

best

demo version free
levels included
1.1 - 1.15
2.1
3.2-3.5
4.1

full version 20usd
patreon support for demo version
Done
1. website
2. paypal account
3. esun bank account
4. license generator/validator
5. config to settings
1. language fall back options

TODO
1. verify lang
1. vim manual to csv
2. verify levels
3. better task complete graphics
4. database for logging user metrics
5.

ffmpeg -i Demo2.avi -vf yadif,format=yuv420p -force_key_frames expr:gte(t,n_forced/2) -c:v libx264 -crf 18 -bf 2 -c:a aac -q:a 1 -ac 2 -ar 48000 -use_editlist 0 -movflags +faststart Demo2.mp4

c:\ffmpeg\bin\ffmpeg.exe -threads 4 -r 30 -video_size 1460x1396 -offset_x -1460 -offset_y 2 -f gdigrab -i desktop -preset ultrafast -crf 0 -pix_fmt yuv444p -c:v libx264 -y  C:\Users\julio\Dropbox\Projects\hardcore_coding_warrior\data\output\vscode-1605078000111.mp4


svg default value remove
stroke-linecap="butt"
stroke-linejoin="miter"
fill-opacity="1"
opacity="1"
style="display: block;"
 transform="matrix(1,0,0,1,0,0)"

 		"watch": "tsc -watch -p ./",
		 		"onCommand:vva.start"


				 ${workspaceRoot}: workspace root folder
${file}: path of saved file
${relativeFile}: relative path of saved file
${fileBasename}: saved file's basename
${fileDirname}: directory name of saved file
${fileExtname}: extension (including .) of saved file
${fileBasenameNoExt}: saved file's basename without extension
${cwd}:current working directory



ho
hl
bw
BW
gee
gEE
0^$

vertical
jk

operators
v
d
c
y
p

vl
vw
vb

# HELPERS

## horizontal
0,^,$
B,W
b,w
ge,e
gE,E

## horizontal 2


## vim anchor to active

## cursor locator

## theme
