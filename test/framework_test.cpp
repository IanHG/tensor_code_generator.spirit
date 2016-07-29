#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

int main()
{
   std::ifstream ifs("framework_test.list");
   std::string str;
   std::string cxx = "g++";
   std::string cxxflags = "-O3 -std=c++14 -I ../../include";
   std::string libs = "../gen/general.cpp ../../lib/libtcg.a -llapack -lblas";

   std::string tcg = "../../bin/tcg.x";
   
   while(std::getline(ifs, str))
   {
      int return_value;
      
      // change dir
      return_value = chdir(str.c_str());
      return_value = std::system("pwd");

      // run tcg
      std::string tcg_command = tcg + " " + str + ".tc " + str + "_generated";
      return_value = std::system(tcg_command.c_str());
      
      // compile tc
      std::string cc = cxx + " " + cxxflags + " -o main " + str + ".cpp " + str + "_generated.cpp " + libs;
      return_value = std::system(cc.c_str());
      if(return_value)
      {
         std::cout << " compilation failed : " << str << std::endl;
      }

      // run tc
      std::string run_command = "./main";
      return_value = std::system(run_command.c_str());
      if(return_value)
      {
         std::cout << " tc run failed : " << str << std::endl;
      }

      // change back to 'main' dir
      return_value = chdir("..");
      return_value = std::system("pwd");
   }

   return 0;
}
