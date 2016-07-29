#ifndef TCG_COMPILER_HPP_INCLUDED
#define TCG_COMPILER_HPP_INCLUDED

#include <functional>
#include <map>
#include <string>
#include <vector>
#include <tuple>
#include <stack>
#include <iostream>
#include <boost/assert.hpp>

#include "ast.hpp"
#include "error_handler.hpp"


namespace tcg
{
   namespace code_gen
   {
      namespace x3 = boost::spirit::x3;
      using multi_index_type = std::vector<char>;
      using permutation_type = std::vector<int>;

      inline std::ostream& operator<<(std::ostream& os, const multi_index_type& m)
      {
         os << "[";
         for(const auto& x : m)
         {
            os << x;
         }
         os << "]";
         return os;
      }
      
      /***************************************************************************
       * Symbol table
       ***************************************************************************/
      struct symbol
      {  
         std::string name_;
         char utype_;
         char ptype_;
         char pflag_;
      };

      inline std::string symbol_name(const symbol& sym)
      {
         return sym.name_;
      }

      inline std::string symbol_type(const symbol& sym)
      {
         std::string type;
         switch(sym.utype_)
         {
            case 'f': type += "float" ; break;
            case 'd': type += "double"; break;
            case 'c': type += "std::complex<float>" ; break;
            case 'z': type += "std::complex<double>"; break;
            case 'i': type += "int"; break;
            default: BOOST_ASSERT(0);
         }
         type += (sym.ptype_ == 'p' ? "*" : "");
         return type;
      }

      inline bool symbol_is_persistent(const symbol& sym)
      {
         return sym.pflag_ == 'p';
      }

      struct symbol_table
      {
         symbol_table(symbol_table* prev = nullptr)
            : enclosing_table_(prev)
         {
         }

         void put(const std::string& s, const symbol& sym)
         {
            symbols_.emplace(s, sym);
         }

         symbol* get(const std::string& s)
         {
            for(symbol_table* t = this; t != nullptr; t = t->enclosing_table_)
            {
               auto isym = t->symbols_.find(s);
               if(isym != t->symbols_.end()) return &((*isym).second);
            }
            return nullptr;
         }

         std::map<std::string, symbol> symbols_;
         symbol_table* enclosing_table_ = nullptr;
      };

      /***************************************************************************
       * TAC stuff
       ***************************************************************************/
      struct tac_variable
      {
         tac_variable(const std::string& name = "", const multi_index_type& indices = {}, char pflag = 't')
            : name_(name)
            , indices_(indices)
            , pflag_(pflag)
         {
         }

         std::string name_;
         multi_index_type indices_;
         char utype_ = 'd'; // type flag: f (float), d (double), c (complex float), z (complex double)
         char ptype_ = 'p'; // pointer type : p (pointer), v (value)
         char pflag_;       // persistence flag: p (persistent), t (temporary)
      };

      inline bool is_temporary(const tac_variable& t)
      {
         return t.pflag_ == 't';
      }

      inline std::string type_as_string(const tac_variable& t)
      {
         switch(t.utype_)
         {
            case 'f': return "float";
            case 'd': return "double";
            case 'c': return "std::complex<float>";
            case 'z': return "std::complex<double>";
            default: BOOST_ASSERT(0);
         }
      }

      inline std::ostream& operator<<(std::ostream& os, const tac_variable& t)
      {
         os << "<" << t.name_ << ":" << t.indices_ << ":" << "{" << t.utype_ << "," << t.pflag_ << "}" << ">";
         return os;
      }

      struct tac 
      {
         tac
            ( ast::optoken op
            , const tac_variable& arg1
            , const tac_variable& arg2
            , const tac_variable& result
            )
            : op_(op)
            , arg1_(arg1)
            , arg2_(arg2)
            , result_(result)
         {
         }

         ast::optoken op_;
         tac_variable arg1_;
         tac_variable arg2_;
         tac_variable result_;
      };

      inline std::ostream& operator<<(std::ostream& os, const tac& t)
      {
         os << t.result_ << " = " << t.arg1_ << " " << t.op_ << " " << t.arg2_;
         return os;
      }
      
      struct tac_function;

      struct intermediate_tac
         : x3::variant
           < tac
           , x3::forward_ast<tac_function>
           >
      {
         using base_type::base_type;
         using base_type::operator=;
      };

      struct intermediate_program: public std::list<intermediate_tac>
      {
         intermediate_program(symbol_table* prev = nullptr)
            : symbol_table_(prev)
         {
         }

         void op(ast::optoken);
         void add_variable(const tac_variable& v);
         
         //std::map<std::string, tensor_intermed> intermed_table_;

         std::stack<tac_variable> variable_stack_;
         symbol_table symbol_table_;
      };
      
      struct tac_function
      {
         tac_function(const std::string& name, symbol_table* prev)
            : name_(name)
            , program_(prev)
         {
         }

         std::string name_;
         intermediate_program program_;
      };

      inline std::ostream& operator<<(std::ostream& os, const intermediate_program& t)
      {
         for(const auto& x : t)
            os << x << std::endl;
         return os;
      }

      struct tensor_intermed
      {
         static std::string create_guid();
         
         std::string operation_;
         std::string guid_;
      };

      bool operator==(const tensor_intermed&, const tensor_intermed&);

      bool is_permutation(const multi_index_type&, const multi_index_type&);

      multi_index_type find_overlapping_indices(const multi_index_type&, const multi_index_type&);

      multi_index_type find_result_indices(const multi_index_type&, const multi_index_type&);

      multi_index_type remove_indices(const multi_index_type&, const multi_index_type&);

      multi_index_type catenate(const multi_index_type&, const multi_index_type&);

      permutation_type find_permutation(const multi_index_type&, const multi_index_type&);

      bool is_unit_permutation(const permutation_type&);

      multi_index_type create_permuted_indices(const multi_index_type&, const permutation_type&);


      /***************************************************************************
       * Compiler
       ***************************************************************************/
      struct compiler
      {
         //struct expr_result 
         //{ 
         //   expr_result(const std::string& name = "", const multi_index_type& indices = {})
         //      : name_(name)
         //      , indices_(indices)
         //   {
         //   }

         //   std::string name_;
         //   multi_index_type indices_;
         //};
         //
         //template<class base_type_t = std::tuple<bool, expr_result> >
         //struct return_type_impl
         //   : base_type_t
         //{
         //   using base_type = return_type_impl<base_type_t>;
         //   using base_type_t::base_type_t;
         //   using base_type_t::operator=;
         //};

         //struct return_type
         //   : return_type_impl<>
         //{
         //   using base_type::base_type;
         //   using base_type::operator=;

         //   operator bool() { return std::get<0>(*this); }
         //};
         //
         //template<class... Ts>
         //std::tuple<Ts...> maitai(Ts&&... ts) const { return std::tuple<Ts...>{std::forward<Ts>(ts)...}; }

         using error_handler_type = std::function<void(x3::position_tagged, std::string const&)>;

         //! constructor
         template<class ErrorHandler>
         compiler
          ( intermediate_program& tac
          , const ErrorHandler& error_handler
          )
          : intermediate_program_(tac)
          , error_handler_
            ( [&](x3::position_tagged pos, const std::string& msg) { error_handler(pos, msg); }
            )
          {
          }

         //! handle nil node in ast
         bool operator()(const ast::nil& x, intermediate_program& prog) const { BOOST_ASSERT(0); return false; }

         //! handle expression nodes in ast
         bool operator()(const double& x, intermediate_program& prog) const;
         bool operator()(const unsigned& x, intermediate_program& prog) const;
         bool operator()(const bool& x, intermediate_program& prog) const;
         bool operator()(const ast::variable& x, intermediate_program& prog) const;
         bool operator()(const ast::tensor_litteral& x, intermediate_program& prog) const;
         bool operator()(const ast::operation& x, intermediate_program& prog) const;
         bool operator()(const ast::unary& x, intermediate_program& prog) const;
         bool operator()(const ast::expression& x, intermediate_program& prog) const;

         
         //! handle statement nodes in ast
         bool operator()(const ast::statement& x, intermediate_program& prog) const;
         bool operator()(const ast::statement_list& x, intermediate_program& prog) const;
         bool operator()(const ast::variable_declaration& x, intermediate_program& prog) const;
         bool operator()(const ast::assignment& x, intermediate_program& prog) const;
         bool operator()(const ast::if_statement& x, intermediate_program& prog) const;
         bool operator()(const ast::while_statement& x, intermediate_program& prog) const;
         bool operator()(const ast::function_definition& x, intermediate_program& prog) const;

         //! start compilation
         bool start(const ast::statement_list& x) const;

         intermediate_program& intermediate_program_;
         error_handler_type error_handler_;
      };
   } /* namespace code_gen */
} /* namespace tcg */

#endif /* TCG_COMPILER_HPP_INCLUDED */
