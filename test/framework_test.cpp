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

   bool cleanup = true;
   
   bool success = true;
   std::cout << "*================================================================*" << std::endl;
   std::cout << " START FRAMEWORK TEST " << std::endl;
   while(std::getline(ifs, str))
   {
      std::cout << "*----------------------------- " + str + " -----------------------------*" << std::endl;
      int return_value;
      std::string dir = str + ".tc_test";
      
      // change dir
      return_value = chdir(dir.c_str());
      return_value = std::system("pwd");

      // run tcg
      std::string tcg_command = tcg + " -i " + str + ".tc -o " + str + "_generated";
      return_value = std::system(tcg_command.c_str());
      if(return_value)
      {
         success = false;
      }
      
      // compile tc
      std::string cc = cxx + " " + cxxflags + " -o main " + str + ".cpp " + str + "_generated.cpp " + libs;
      return_value = std::system(cc.c_str());
      if(return_value)
      {
         std::cout << " compilation failed : " << str << std::endl;
         success = false;
      }

      // run tc
      std::string run_command = "./main";
      return_value = std::system(run_command.c_str());
      if(return_value)
      {
         std::cout << " tc run failed : " << str << std::endl;
         success = false;
      }

      // output
      std::cout << "******************************************************************" << std::endl;
      std::cout << "* " << str << " " << (success ? " ! SUCCESS ! " : " ! FAILED ! ") << std::endl;
      std::cout << "******************************************************************" << std::endl << std::endl;
      
      // clean-up
      if(success && cleanup)
      {
         auto rm_command = "rm " + str + "_generated.cpp " + str + "_generated.hpp main";
         return_value = std::system(rm_command.c_str());
      }
      

      // change back to 'main' dir
      return_value = chdir("..");
      //return_value = std::system("pwd");
   }
   
   std::cout << "*================================================================*" << std::endl;
   if(success)
      std::cout << "* SUCCESS ! " << std::endl;
   else
      std::cout << "* FAILED ! " << std::endl;
   std::cout << "*================================================================*" << std::endl;

   return 0;
}
