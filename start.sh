if [ -f BaksoWar ]; then
	rm BaksoWar
fi

# Build new
gcc main.cpp -o BaksoWar -Wno-unused -lncurses

# Run
./BaksoWar