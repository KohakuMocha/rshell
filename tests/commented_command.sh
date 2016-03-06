#!/bin/sh

cd ..
make
cd src
#printf "Begin single command tests: \n\n"
../bin/./rshell << 'EOF'
ls -a # ls -l
echo "Hello there! Scriptbo is happy to see you in rshell!" # Hey you! Hi!
git status #No!
exit
EOF
printf "\n"
#End single command tests!\n"