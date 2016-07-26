OBJ=main.cpp
FUNC=expression.cpp statement.cpp compiler.cpp code_generator.cpp
CXX=g++
CXXFLAGS=-Wall
CXXOPTIMFLAGS=-O3
CXXDEBUGFLAGS=-g -O0
LIBS=-pthread -lboost_system -lblas -llapack

gcc: $(OBJ) $(FUNC)
	$(CXX) $(CXXFLAGS) $(CXXOPTIMFLAGS) -fexpensive-optimizations -Drestrict= -std=c++14 -o main $(OBJ) $(FUNC) $(LIBS)
	./main

gcc_debug: $(OBJ) $(FUNC)
	$(CXX) $(CXXFLAGS) $(CXXDEBUGFLAGS) -std=c++14 -o main $(OBJ) $(FUNC) $(LIBS)
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./main

gcc_assembly: $(OBJ) $(FUNC)
	$(CXX) $(CXXFLAGS) $(CXXDEBUGFLAGS) -O3 -S -std=c++14 $(OBJ) $(FUNC) $(LIBS)

intel: $(OBJ) $(FUNC)
	icc $(CXXFLAGS) $(CXXOPTIMFLAGS) -ip -ipo -fast -std=c++14 -Wcontext-limit=100 -o main $(OBJ) $(FUNC) $(LIBS)
	./main

intel_debug: $(OBJ) $(FUNC)
	icc $(CXXFLAGS) $(CXXDEBUGFLAGS) -std=c++14 -Wcontext-limit=100 -o main $(OBJ) $(FUNC) $(LIBS)
	valgrind --tool=memcheck --leak-check=yes ./main

clang: $(OBJ) $(FUNC)
	clang++ $(CXXFLAGS) $(CXXOPTIMFLAGS) -std=c++14 -o main $(OBJ) $(FUNC) $(LIBS)
	./main

clang_debug: $(OBJ) $(FUNC)
	clang++ $(CXXFLAGS) $(CXXDEBUGFLAGS) -std=c++14 -o main $(OBJ) $(FUNC) $(LIBS)
	valgrind --tool=memcheck --leak-check=yes ./main

clang_assembly: $(OBJ) $(FUNC)
	clang++ $(CXXFLAGS) $(CXXDEBUGFLAGS) -O3 -S -std=c++14 $(OBJ) $(FUNC) $(LIBS)

clean:
	rm -f *core main

lib: libtcg.a

libtcg.a: include/axpy.cpp include/gemv.cpp
	cd lib; \
	$(CXX) $(CXXOPTIMFLAGS) -std=c++14 -c ../include/axpy.cpp -llapack -lblas; \
	$(CXX) $(CXXOPTIMFLAGS) -std=c++14 -c ../include/gemv.cpp -llapack -lblas; \
	$(CXX) $(CXXOPTIMFLAGS) -std=c++14 -c ../include/gemm.cpp -llapack -lblas; \
	ar -cvq libtcg.a *.o;
