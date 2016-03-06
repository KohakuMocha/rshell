#!/bin/sh

cd ..
make
cd src

#printf "Begin multi command tests: \n\n"
../bin/./rshell << 'EOF'
(echo a && echo b) || (echo c && echo d)
(echo a || echo b) && (echo c || echo d)
(echo a || (echo b && echo c))
(echo a; echo b; (echo c || echo d))
(echo a #echo b; (echo c || echo d))
(ls && (ls || ls -a) || echo a) && [ rshell.cpp ]
exit
EOF
printf "\n"
#End multi command tests!\n"