" Copy this into your vimrc.

highlight Comment ctermfg=Cyan cterm=bold
highlight Constant ctermfg=Cyan cterm=none
" parens/ssyntax very subtle
highlight Delimiter ctermfg=DarkMagenta cterm=none
" quasiquote/unquote slightly less subtle
highlight Preproc ctermfg=Blue cterm=bold

" quoted syms are like literal numbers or strings
highlight link lispAtom Constant

function! WartSettings()
  set filetype=lisp
  set nolisp " since we lack parens so often

  set iskeyword=@,48-57,?,!,_,$

  syntax clear lispComment
  syntax match lispComment /#.*$/  contains=@lispCommentGroup

  "" unquote and splice
  " ,@a (,@b)
  syntax match Unquote /,@\|,/
  syntax cluster lispListCluster add=Unquote
  highlight link Unquote Preproc
  " @a (@b)
  syntax match Splice /@/
  syntax cluster lispListCluster add=Splice
  highlight link Splice Delimiter

  " quoted and keyword literals
  " :a (:b -12 -5.6 :c -7e-8 "d" 'e :f) ',g :h    these are all literals except g
  syntax clear lispAtom
  syntax clear lispKey
  syntax match lispAtom "'[^ \t'.!(),@`]\+" contains=lispAtomMark
  " :a b!:c :d?!e f:g.h i?:j.k l&:m
  " ^    ^  ^                                     literal
  "     ^      ^   ^ ^    ^ ^   ^x                delimiter
  syntax match lispAtom "\([^ \t'.!~(),@`]\)\@<!:[^ \t'.!~(),@`:&]\+"
  syntax cluster lispListCluster add=lispAtom

  " deemphasize period operator
  syntax match wartCall /[^ .]\zs\./
  syntax cluster lispListCluster add=wartCall
  highlight link wartCall Delimiter
  " hack: symbols are interfering with operators
  syntax clear lispSymbol
  syntax clear lispFunc
  " have to redo numbers; lispNumber depended on lispSymbol
  " a2 b-3 c%4 d?5 should not highlight digits
  " 34 3.5 .1 1.3e5 (34) (a 3) b.34 c!34 d:34 e&35 should
  syntax clear lispNumber
  syntax match Number "\([^ (\t'.!()@`:&]\)\@<![+-]*\(\.\d\+\|\d\+\(\.\d*\)\=\)\([eE][-+]\=\d\+\)\="
  syntax cluster lispListCluster add=Number
endfunction

autocmd BufReadPost,BufNewFile *.wart,*.test,*.wtst call WartSettings()
if (expand("%:e") =~ 'wart\|test\|wtst') " in case we loaded this too late on startup
  call WartSettings()
endif

autocmd FileType c,cpp syntax match Delimiter /[{}();,:.]/
autocmd FileType c,cpp syntax match Delimiter /->/
if (expand("%:e") =~ 'c\|cc\|cpp\|h') " in case we loaded this too late on startup
  syntax match Delimiter /[{}();,:.]/
  syntax match Delimiter /->/
endif
