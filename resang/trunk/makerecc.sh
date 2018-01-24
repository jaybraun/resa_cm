for token in `ls recc*.c`
do
    subtoken=${token%%.*}
    #echo $subtoken
    gcc -c -g $token
    gcc -o $subtoken "$subtoken".o
done
