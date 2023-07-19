
" Remember position of last edit and return on reopen
if has("autocmd")
  au BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
endif

set nu
set fileencodings=utf-8,gb2312,gbk
set hlsearch
set tabstop=4
set expandtab
set paste

filetype on
syntax on
