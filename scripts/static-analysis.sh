#!/bin/bash
cd "$(dirname "$0")"
cd ..

SOURCES=`find . -regextype posix-extended -regex '^.*(\/).*(\.cpp|\.c|\.hpp|\.h)$'`
SOURCES="$(echo "$SOURCES"|tr '\n' ' ')"

clang-tidy --config-file=.clang-tidy $SOURCES
