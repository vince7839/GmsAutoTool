#!/bin/bash
gnome-terminal -x bash -c "script/all-test.sh \"$1\" \"$2\" \"$3\" \"$4\";exec bash"
