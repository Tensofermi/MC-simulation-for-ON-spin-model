# move all the data from ./cmp and ./raw into a new directory, and create new.

mkdir $1
mv ./cmp $1
mv ./raw $1
mkdir cmp
mkdir raw