cpp = g++
OBT = O2       # optimaization

Cppfileserver = server.cpp 
Cppclint = clint.cpp  
Cppclint2 = clint2.cpp
Cppclint3 = clint3.cpp

OfileServer = server.o  
Ofileclint = clint.o  
Ofileclint2 = clint2.o
Ofileclint3 = clint3.o


clint : $(Ofileclint)
	@echo "*"*"*Making clint  Done*"*"*"
	@$(cpp) -o  $@ $^ -lwsock32

%.o: %.cpp
	@$(cpp) $(OBT) -c -o $@ $^


clint2 : $(Ofileclint2)
	@echo "*"*"*Making clint2  Done*"*"*"
	@$(cpp) -o  $@ $^ -lwsock32
	
%.o: %.cpp
	@$(cpp) $(OBT) -c -o $@ $^


clint3 : $(Ofileclint3)
	@echo "*"*"*Making clint3  Done*"*"*"
	@$(cpp) -o  $@ $^ -lwsock32
	
%.o: %.cpp
	@$(cpp) $(OBT) -c -o $@ $^


server: $(OfileServer)
	@$(cpp) -o  $@ $^ -lwsock32
	@echo "*"*"*Making Server  Done*"*"*"

%.o: %.cpp
	@$(cpp) $(OBT) -c -o $@ $^


clean: 
	@rm *.o  run


