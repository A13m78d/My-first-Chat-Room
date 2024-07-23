c = g++
OBT = O2       # optimaization

Cfiles = server.cpp
OfileServer = server.o  


run: $(OfileServer)
	@$(c) -o  $@ $^ -lwsock32
	@echo "*"*"*Making cuple of c files  Done*"*"*"

%.o: %.c
	@$(c) $(OBT) -c -o $@ $^


clean: 
	@rm *.o run


