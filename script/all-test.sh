#!/bin/bash
trap 'rm $3;echo delete file $3' SIGHUP SIGINT
script/all.exp "$1" "$2" "$4"|tee "$3"
rm $3
