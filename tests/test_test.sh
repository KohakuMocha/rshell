#!/bin/sh

cd ..
make
cd src
#printf "Begin multi command tests: \n\n"
../bin/./rshell << 'EOF'
test
test -e
test -efd
test rshell.cpp
test qshell.cpp
test -e rshell.cpp
test -f rshell.cpp
test -d rshell.cpp
test -efd rshell.cpp
test -efd qshell.cpp
test -e -f -d rshell.cpp
[ rshell.cpp ]
[ qshell.cpp ]
[ -e rshell.cpp ]
[ -f rshell.cpp ]
[ -d rshell.cpp ]
[ -efd rshell.cpp ]
[ -e -f -d rshell.cpp ]
test rshell.cpp && ls
test rshell.cpp && ls || test qshell.cpp
[ rshell.cpp ] && ls
[ rshell.cpp ] && ls || test qshell.cpp
ls -a || test -e rshell.cpp
ls -a || [ -e rshell.cpp]
exit
EOF
printf "\n"
#End multi command tests!\n"