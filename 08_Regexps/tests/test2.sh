#!/bin/bash
prog=$(../esub '.(png|jpg)$$' '.hehe-haha\\\1\\' 'beautiful picture')
target=$(echo 'beautiful picture' | sed -E 's/\.(png|jpg)/.hehe-haha\\\1\\/')
if test "$prog" = "$target"; then exit 0; else exit 1; fi
