OBJS=main.o expression.o statement.o compiler.o code_generator.o
CXX=g++
CXXSTD=--std=c++14
CXXFLAGS=-Wall
CXXOPTIMFLAGS=-O3
CXXDEBUGFLAGS=-g -O0
CXXFLAGS=$(CXXOPTIMFLAGS)
LIBS=-pthread -lboost_system -lblas -llapack

# link
bin/tcg.x: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o bin/tcg.x $(LIBS)

# pull dependencies for existing .o files
-include $(OBJS:.o=.d)

# compile and generate dependency info
%.o: %.cpp
	$(CXX) $(CXXSTD) -c $(CXXFLAGS) $*.cpp -o $*.o
	$(CXX) $(CXXSTD) -MM $(CXXFLAGS) $*.cpp > $*.d

clean:
	rm -f *core *.o *.d tcg.x

lib: libtcg.a

libtcg.a: include/axpy.cpp include/gemv.cpp
	cd lib; \
	$(CXX) $(CXXFLAGS) -std=c++14 -c ../include/axpy.cpp -llapack -lblas; \
	$(CXX) $(CXXFLAGS) -std=c++14 -c ../include/gemv.cpp -llapack -lblas; \
	$(CXX) $(CXXFLAGS) -std=c++14 -c ../include/gemm.cpp -llapack -lblas; \
	ar -cvq libtcg.a *.o;
