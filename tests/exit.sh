#!/bin/sh

cd ..
make
cd src
#printf "Begin single command tests: \n\n"
../bin/./rshell << 'EOF'
exit
EOF


../bin/./rshell << 'EOF'
ls && ls -a && exit && echo "NOOO I DON'T GET TO PRINT"
EOF


../bin/./rshell << 'EOF'
ls
echo "Prepare to exit!"
exit
EOF
printf "\n"
#End single command tests!\n"