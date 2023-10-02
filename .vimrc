" ~/.vimrc

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => General
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" Enable features not Vi compatible
set nocompatible

filetype on
filetype plugin on
filetype indent on


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


" No terminal bells on error
set noerrorbells

" Fix <esc>+O timeout
set timeout timeoutlen=5000 ttimeoutlen=100

" Use tab expansion in settings/commands
set wildmenu
set wildmode=full

" Display extra whitespaces in blue
set list
set list listchars=tab:»\ ,trail:·,eol:$ " Indicators for invisible characters


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
set colorcolumn=80

" File type specific options overwrite these parameters
" (see .vim/after/ftplugin)

" Use 8 spaces
set shiftwidth=4
set tabstop=4
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

" Highlight the line under scheme
set cursorline

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
		autocmd BufNewFile Makefile 0r ~/.vim/templates/Makefile
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

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => Misc
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

autocmd FileType make setlocal noexpandtab softtabstop=0

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


""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => Plugin
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

call plug#begin('~/.vim/plugged')

Plug 'ludovicchabant/vim-gutentags'
Plug 'vim-airline/vim-airline'
Plug 'vim-airline/vim-airline-themes'
Plug 'tpope/vim-fugitive'

" Theme
Plug 'morhetz/gruvbox'

call plug#end()

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" => Theme
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
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
" => Shortcuts
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

set splitbelow
set splitright

noremap <C-t> :Sexplore<cr>
" config for :term
set termwinsize=10x
noremap <C-a> :terminal<cr>

noremap <C-i> :syntax on<cr>

" change lines
noremap + ddkP
noremap _ ddp
noremap - yyp

