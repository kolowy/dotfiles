" ~/.vimrc

set encoding=utf-8
set cc=80
noremap <C-i> :syntax on<cr>
set expandtab
set shiftwidth=4
set shiftround
set softtabstop=4
set tabstop=8
set smartindent
set cindent
set autoindent
set smarttab
set mouse=a " for scrolling
set clipboard=unnamedplus
    
set textwidth=80
set backspace=eol,start,indent
set belloff=all

set hlsearch " search highlighting
set lazyredraw " for perf

set linebreak " avoid wrapping a line in the middle of a word
syntax enable " enable syntax highlighting
set cursorline "highlight the line under scheme
set number " number line
"set relativenumber " nb relatifs
set spell " enable spellchecking
set notermguicolors

autocmd Filetype make setlocal noexpandtab " Make . on the end of the lines
set list listchars=tab:»·,trail:·

" vundle config
set nocompatible
filetype off

set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()

Plugin 'gmarik/Vundle.vim'

Plugin 'morhetz/gruvbox'
Plugin 'vim-airline/vim-airline'
Plugin 'vim-airline/vim-airline-themes'
Plugin 'ycm-core/YouCompleteMe'
Plugin 'preservim/nerdtree'
Plugin 'tpope/vim-fugitive'
Plugin 'airblade/vim-gitgutter'
Plugin 'lervag/vimtex'
Plugin 'Yggdroot/indentLine'
Plugin 'zxqfl/tabnine-vim'

call vundle#end()
filetype plugin indent on

set background=dark
colorscheme gruvbox
syntax on

let g:airline_theme='gruvbox'

" tabulation
let g:indentLine_char_list = ['|', '¦', '┆', '┊']
let g:indentLine_leadingSpaceChar=' '
let g:indentLine_leadingSpaceEnabled='1'
" "let g:ycm_global_ycm_extra_conf='~/.ycm_extra_conf.py'
" "set runtimepath+=~/.vim/bundle/YouCompleteMe


"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""                     Short curts                       """""""""""""
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""



"" autocomplete for () and {} ect
" autocmd Filetype {cs,py,c,cpp,h} call SetCSharpAutocompletion()
" function SetCSharpAutocompletion()
  inoremap {      {}<Left>
  inoremap {<CR>  <CR>{<CR>}<Esc>O
  inoremap {{     {
  inoremap {}     {}
  inoremap (  ()<Left>
  inoremap <expr> )  strpart(getline('.'), col('.')-1, 1) == ")" ? "\<Right>" : ")"
  inoremap <expr> <BS>  strpart(getline('.'), col('.')-2, 2) == "()" ? "\<BS><Del>" : strpart(getline('.'), col('.')-2, 2) == "''" ? "\<BS><Del>" : strpart(getline('.'), col('.')-2, 2) == "\"\"" ? "\<BS><Del>" : "\<BS>"
  inoremap [  []<Left>
  inoremap <expr> ]  strpart(getline('.'), col('.')-1, 1) == "]" ? "\<Right>" : "]"

  inoremap <expr> ' strpart(getline('.'), col('.')-1, 1) == "\'" ? "\<Right>" : "\'\'\<Left>"
  inoremap <expr> " strpart(getline('.'), col('.')-1, 1) == "\"" ? "\<Right>" : "\"\"\<Left>"
"  inoremap <expr> <BS>  strpart(getline('.'), col('.')-2, 2) == "''" ? "\<BS><Del>" : "\<BS>"
"  inoremap <expr> ; strpart(getline('.'), col('.')-1, 1) == ")" ? "\<End>;" : ";"
  inoremap for<Space> for ()<Left>
  inoremap if<Space> if ()<Left>
  inoremap if<Tab> if ()<Left>
" endfunction


" crtl+t -> Tree
noremap <C-t> :NERDTreeToggle<cr>
noremap <C-a> :terminal<cr>

" exit vim if only tree remaining windows
autocmd BufEnter * if (winnr('$') == 1 && exists('b:NEARDTree') && b:NERDTree.isTabTree()) | q | endif

" F7 -> syntaxe pour l'algo
noremap <F7> :if exists("g:syntax_on") <Bar> 
        \   syntax off <Bar>
        \ else <Bar>
        \   syntax enable <Bar> 
        \ endif <CR>


" change lines
noremap + ddkP
noremap - ddp
noremap * yyp
imap ii <Esc>


