" ~/.vimrc

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => General
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" Enable features not Vi compatible
set nocompatible

filetype on
filetype plugin on
filetype indent on

" Enable mouse and scrolling
set mouse=a

" fix alacritty mouse 
set ttymouse=sgr

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => VIM user interface
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" Encoding options
set encoding=utf-8

" Show current position
set ruler

" Show line numbers
set number relativenumber
" https://jeffkreeftmeijer.com/vim-number/
augroup numbertoggle
    autocmd!
    autocmd BufEnter,FocusGained,InsertLeave * set relativenumber
    autocmd BufLeave,FocusLost,InsertEnter   * set norelativenumber
augroup END

" Show current editing mode
set showmode

" Show matching brackets when text indicator is over them
set showmatch

" Show the status line
set laststatus=2

" Always display the tabline, even if there is only one tab
set showtabline=2

" Colored column
set colorcolumn=81

" No terminal bells on error
set noerrorbells

" Fix <esc>+O timeout
set timeout timeoutlen=5000 ttimeoutlen=100

" Use tab expansion in settings/commands
set wildmenu
set wildmode=full

" Display extra whitespaces in blue
set list
set listchars=tab:»·,trail:·


""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => Colors and Fonts
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

syntax on
set background=dark

" Enable 256 colors in Vim
set t_Co=256

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => Text, tab and indent related
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" 80 cols
set cc=80

" File type specific options overwrite these parameters
" (see .vim/after/ftplugin)

" Use 8 spaces
set shiftwidth=8
set tabstop=8
set softtabstop=4
set expandtab
set smarttab

" Indentation
set autoindent
set shiftround
set smartindent
set cindent

" http://vim.wikia.com/wiki/Restoring_indent_after_typing_hash
" set smartindent
set cindent
set cinkeys-=0#
set indentkeys-=0#

" Smart backspace
set backspace=indent,eol,start

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => Search
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" Highlight search results
set hlsearch

" Ctrl-N stops hlsearch
nmap <silent> <C-N> :silent noh<cr>

set showmatch
set ignorecase
set smartcase
" ignorecase does not play nicely with completion
set infercase
set incsearch

" First search for the current directory containing the current
" file, then the current directory, then each directory under
" the current directory
set path=.,,**

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => Autocomplete
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

set omnifunc=syntaxcomplete#Complete
set spell " enable spellchecking
syntax enable " enable syntax highlighting

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => Files, backups and undo
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" File
set autoread " Re-read file when it has been changed outside of vim
set autowrite " Automatically write file on some commands (like :make, :next...)

" Miscellaneaous
set list " Enable listchars

" Persistent undo
set undofile
set undodir=~/.vimtmp/undo

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => Templating
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

if has("autocmd")
	augroup templates
		autocmd BufNewFile *.c 0r ~/.vim/templates/template.c
		autocmd BufNewFile *.h 0r ~/.vim/templates/template.h
		autocmd BufNewFile *.cpp 0r ~/.vim/templates/template.cpp
		autocmd BufNewFile *.html 0r ~/.vim/templates/template.html
		autocmd BufNewFile *.php 0r ~/.vim/templates/template.php
		autocmd BufNewFile *.hpp 0r ~/.vim/templates/template.hpp
		autocmd BufNewFile *.py 0r ~/.vim/templates/template.py
		autocmd BufNewFile *.sh 0r ~/.vim/templates/template.sh
		autocmd BufNewFile Program.cs 0r ~/.vim/templates/template_Program.cs
		autocmd BufNewFile *.cs 0r ~/.vim/templates/template.cs
		autocmd BufNewFile *.tex 0r ~/.vim/templates/template.tex
	augroup END
endif

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => Custom mappings
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

let mapleader = "\<Space>"
let g:mapleader = "\<Space>"

" Easier navigation between splits
nnoremap <C-J> <C-W><C-J>
nnoremap <C-K> <C-W><C-K>
nnoremap <C-L> <C-W><C-L>
nnoremap <C-H> <C-W><C-H>

" Toggle paste
nnoremap <F2> :set invpaste paste?<CR>
set pastetoggle=<F2>

" FZF vim integration
set rtp+=~/.fzf
map <c-p> :FZF<CR>

" Improve jumping between buffers
nnoremap gb :ls<CR>:b<Space>

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => Misc
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" Return to last edit position when opening files
autocmd BufReadPost *
    \ if line("'\"") > 0 && line("'\"") <= line("$") |
    \ exe "normal! g`\"" |
    \ endif
" Remember info about open buffers on close
set viminfo^=%

" Disable automatic comment insertion
autocmd FileType * setlocal formatoptions-=c formatoptions-=r formatoptions-=o

" Automatically equalize splits when Vim is resized
autocmd VimResized * wincmd =


""""""¨ """"""" TODO
noremap <C-i> :syntax on<cr>
set clipboard=unnamedplus

set cursorline "highlight the line under scheme

set notermguicolors
set list listchars=tab:»\ ,trail:·,eol:$ " Indicators for invisible characters

" vundle config
" set nocompatible
" filetype off

call plug#begin('~/.vim/plugged')

Plug 'gmarik/Vundle.vim'
Plug 'ludovicchabant/vim-gutentags'
Plug 'morhetz/gruvbox'
Plug 'vim-airline/vim-airline'
Plug 'vim-airline/vim-airline-themes'
Plug 'tpope/vim-fugitive'

call plug#end()

let g:gruvbox_bold = 1
let g:gruvbox_transpartent_bg = 1
let g:gruvbox_underline = 1

set background=dark
colorscheme gruvbox


let g:airline_theme='gruvbox'

" Gutten tag
set statusline+=%{gutentags#statusline()}
let g:gutentags_project_root = ['Makefile']


"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""                     Short curts                       """""""""""""
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" crtl+t -> Tree
noremap <C-t> :Sexplore<cr>
noremap <C-a> :terminal<cr>

" exit vim if only tree remaining windows
autocmd FileType make setlocal noexpandtab softtabstop=0


" change lines
noremap + ddkP
noremap - yyp
noremap _ ddp
