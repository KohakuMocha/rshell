#!/bin/sh

cd ../src
g++ rshell.cpp
#printf "Begin single command tests: \n\n"
../bin/./rshell << 'EOF'
ls
ls -a
ls -l
echo "Hello there! Scriptbo is happy to see you in rshell!
git status
exit
EOF
printf "\n"
#End single command tests!\n"