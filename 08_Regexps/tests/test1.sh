#!/bin/bash
prog=$(../esub '.(png|jpg)$$' '.hehe-haha\\\1\\' 'beautiful picture.png')
target=$(echo 'beautiful picture.png' | sed -E 's/\.(png|jpg)/.hehe-haha\\\1\\/')
if test "$prog" = "$target"; then exit 0; else exit 1; fi
