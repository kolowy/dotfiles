" ~/.vimrc

set encoding=utf-8
set cc=200
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
set lazyredraw " for perfu

set linebreak " avoid wrapping a line in the middle of a word
syntax enable " enable syntax highlighting
set cursorline "highlight the line under scheme
set number " number line
set relativenumber " nb relatifs
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
Plugin 'preservim/nerdtree'
Plugin 'tpope/vim-fugitive'
Plugin 'airblade/vim-gitgutter'
Plugin 'lervag/vimtex'
Plugin 'Yggdroot/indentLine'

" autocomplete
Plugin 'prabirshrestha/vim-lsp'
Plugin 'mattn/vim-lsp-settings'

Plugin 'prabirshrestha/asyncomplete.vim'
Plugin 'prabirshrestha/asyncomplete-lsp.vim'

call vundle#end()
filetype plugin indent on

let g:gruvbox_bold = 1
let g:gruvbox_transpartent_bg = 1
let g:gruvbox_underline = 1

set background=dark
colorscheme gruvbox
syntax on


let g:airline_theme='gruvbox'

" tabulation
let g:indentLine_char_list = ['|', '¦', '┆', '┊']
let g:indentLine_leadingSpaceChar=' '
let g:indentLine_leadingSpaceEnabled='1'


"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"""""""""""                     Short curts                       """""""""""""
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
"" autocomplete for () and {} ect
autocmd Filetype {cs,py,c,cpp,h} call SetCSharpAutocompletion()
function SetCSharpAutocompletion()
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
  inoremap for<Space> for ()<Left>
  inoremap if<Space> if ()<Left>
  inoremap if<Tab> if ()<Left>
endfunction


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




inoremap <expr> <Tab>   pumvisible() ? "\<C-n>" : "\<Tab>"
inoremap <expr> <S-Tab> pumvisible() ? "\<C-p>" : "\<S-Tab>"
inoremap <expr> <cr>    pumvisible() ? asyncomplete#close_popup() : "\<cr>"

imap <c-space> <Plug>(asyncomplete_force_refresh)
" For Vim 8 (<c-@> corresponds to <c-space>):
" " imap <c-@> <Plug>(asyncomplete_force_refresh)


let g:asyncomplete_auto_popup = 0

function! s:check_back_space() abort
        let col = col('.') - 1
            return !col || getline('.')[col - 1]  =~ '\s'
        endfunction

        inoremap <silent><expr> <TAB>
          \ pumvisible() ? "\<C-n>" :
          \ <SID>check_back_space() ? "\<TAB>" :
          \ asyncomplete#force_refresh()
        inoremap <expr><S-TAB> pumvisible() ? "\<C-p>" : "\<C-h>"

" allow modifying the completeopt variable, or it will
" be overridden all the time
let g:asyncomplete_auto_completeopt = 1

set completeopt=menuone,noinsert,noselect,preview"


autocmd! CompleteDone * if pumvisible() == 0 | pclose | endif








