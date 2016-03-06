#!/bin/sh

cd ../src
g++ rshell.cpp
#printf "Begin multi command tests: \n\n"
../bin/./rshell << 'EOF'
ls ; ls -a
ls -a || ls
ls -l || ls || ls -a
echo "Hello there! Scriptbo is happy to see you in rshell! && echo "Oh you're still here!"
ls || echo "Do you think there is more behind me?" && echo "You've guessed it!"
exit
EOF
printf "\n"
#End multi command tests!\n"