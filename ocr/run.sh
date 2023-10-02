make
rm results/*
./main
make clean
firefox results/*.pdf &
