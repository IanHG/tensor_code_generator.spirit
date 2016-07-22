#ifndef TCG_COMPILER_HPP_INCLUDED
#define TCG_COMPILER_HPP_INCLUDED

#include <functional>
#include <map>
#include <string>
#include <vector>
#include <tuple>
#include <stack>
#include <boost/assert.hpp>

#include "ast.hpp"
#include "error_handler.hpp"


namespace tcg
{
   namespace code_gen
   {
      namespace x3 = boost::spirit::x3;
      using multi_index_type = std::vector<char>;
      /***************************************************************************
       * 
       ***************************************************************************/
      struct tac_variable 
      {
         std::string name_;
         multi_index_type indices_;
      };

      struct tac 
      {
         tac
            ( const std::string& name
            , const tac_variable& lhs
            , const tac_variable& rhs
            , ast::optoken op
            )
            : name_(name)
            , lhs_(lhs)
            , rhs_(rhs)
            , op_(op)
         {
         }

         std::string name_;
         tac_variable lhs_;
         tac_variable rhs_;
         ast::optoken op_;
      };

      struct tac_program: private std::list<tac>
      {
         void op(ast::optoken);
         void add_variable(const tac_variable& v) { variable_stack_.push(v); }
         
         //std::map<std::string, tensor_intermed> intermed_table_;
         std::stack<tac_variable> variable_stack_;
      };

      struct tensor_intermed
      {
         static std::string create_guid();
         
         std::string operation_;
         std::string guid_;
      };

      bool operator==(const tensor_intermed&, const tensor_intermed&);

      bool is_permutation(const multi_index_type&, const multi_index_type&);

      multi_index_type overlap_indices(const multi_index_type&, const multi_index_type&);

      multi_index_type result_indices(const multi_index_type&, const multi_index_type&);

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
          ( const ErrorHandler& error_handler
          )
          : error_handler_
            ( [&](x3::position_tagged pos, const std::string& msg) { error_handler(pos, msg); }
            )
          {
          }

         //! handle nil node in ast
         bool operator()(const ast::nil& x ) const { BOOST_ASSERT(0); return false; }

         //! handle expression nodes in ast
         bool operator()(const double& x) const;
         bool operator()(const unsigned& x) const;
         bool operator()(const bool& x) const;
         bool operator()(const ast::variable& x) const;
         bool operator()(const ast::tensor_litteral& x) const;
         bool operator()(const ast::operation& x) const;
         bool operator()(const ast::unary& x) const;
         bool operator()(const ast::expression& x) const;

         
         //! handle statement nodes in ast
         bool operator()(const ast::statement& x) const;
         bool operator()(const ast::statement_list& x) const;
         bool operator()(const ast::variable_declaration& x) const;
         bool operator()(const ast::assignment& x) const;
         bool operator()(const ast::if_statement& x) const;
         bool operator()(const ast::while_statement& x) const;

         //! start compilation
         bool start(const ast::statement_list& x) const;

         error_handler_type error_handler_;
         tac_program tac_program_;
      };
   } /* namespace code_gen */
} /* namespace tcg */

#endif /* TCG_COMPILER_HPP_INCLUDED */
