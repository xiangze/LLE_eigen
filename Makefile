phony:all

bin=LLEeigen

$(bin):test.cpp $(bin).hpp 
	g++  $^ -o $@ 
#	g++ -std=c++0x $^ -o $@ 

$(bin)_d:test.cpp $(bin).hpp 
	g++ -g $^ -o $@ 

#$(bin)_red:test.cpp $(bin).hpp 
#	g++ -std=c++0x $^ -o $@ -D_MDS_WITH_REDSVD

clean:
	rm $(bin)

all:$(bin) $(bin)_d

test:
	./$(bin) $(bin)_d