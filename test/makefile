OBJ=unittest.cpp 
FUNC=../compiler.cpp
CXX=g++
CXXOPTIMFLAGS=-O3
CXXDEBUGFLAGS=-g -O0
LIBS=../lib/libtcg.a -lblas -llapack -lboost_system ../../../libmda/lib/libmda.a

gcc: $(OBJ) $(FUNC)
	$(CXX) $(CXXOPTIMFLAGS) -fexpensive-optimizations -Drestrict= -std=c++14 -o main $(OBJ) $(FUNC) $(LIBS)
	./main

gcc_debug: $(OBJ) $(FUNC)
	$(CXX) $(CXXDEBUGFLAGS) -std=c++14 -o main $(OBJ) $(FUNC) $(LIBS)
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./main

gcc_assembly: $(OBJ) $(FUNC)
	$(CXX) $(CXXDEBUGFLAGS) -O3 -S -std=c++14 $(OBJ) $(FUNC) $(LIBS)

intel: $(OBJ) $(FUNC)
	icc $(CXXOPTIMFLAGS) -ip -ipo -fast -std=c++14 -Wcontext-limit=100 -o main $(OBJ) $(FUNC) $(LIBS)
	./main

intel_debug: $(OBJ) $(FUNC)
	icc $(CXXDEBUGFLAGS) -std=c++14 -Wcontext-limit=100 -o main $(OBJ) $(FUNC) $(LIBS)
	valgrind --tool=memcheck --leak-check=yes ./main

clang: $(OBJ) $(FUNC)
	clang++ $(CXXOPTIMFLAGS) -std=c++14 -o main $(OBJ) $(FUNC) $(LIBS)
	./main

clang_debug: $(OBJ) $(FUNC)
	clang++ $(CXXDEBUGFLAGS) -std=c++14 -o main $(OBJ) $(FUNC) $(LIBS)
	valgrind --tool=memcheck --leak-check=yes ./main

clang_assembly: $(OBJ) $(FUNC)
	clang++ $(CXXDEBUGFLAGS) -O3 -S -std=c++14 $(OBJ) $(FUNC) $(LIBS)

clean:
	rm -f *core main

lib: include/axpy.cpp
	cd lib; \
	$(CXX) $(CXXOPTIMFLAGS) -std=c++14 -c ../include/axpy.cpp; \
	ar -cvq libtcg.a *.o;
