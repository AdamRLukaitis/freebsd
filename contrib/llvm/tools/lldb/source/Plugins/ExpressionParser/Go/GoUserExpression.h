//===-- GoUserExpression.h --------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef liblldb_GoUserExpression_h_
#define liblldb_GoUserExpression_h_

// C Includes
// C++ Includes
#include <memory>

// Other libraries and framework includes
// Project includes
#include "lldb/lldb-forward.h"
#include "lldb/lldb-private.h"
#include "lldb/Expression/UserExpression.h"
#include "lldb/Expression/ExpressionVariable.h"
#include "lldb/Target/ExecutionContext.h"

namespace lldb_private
{
class GoParser;

class GoPersistentExpressionState : public PersistentExpressionState
{
  public:
    GoPersistentExpressionState();

    ConstString GetNextPersistentVariableName() override;

    void RemovePersistentVariable(lldb::ExpressionVariableSP variable) override;

    lldb::addr_t
    LookupSymbol(const ConstString &name) override
    {
        return LLDB_INVALID_ADDRESS;
    }

    static bool
    classof(const PersistentExpressionState *pv)
    {
        return pv->getKind() == PersistentExpressionState::eKindGo;
    }

  private:
    uint32_t m_next_persistent_variable_id; ///< The counter used by GetNextResultName().
};

//----------------------------------------------------------------------
/// @class GoUserExpression GoUserExpression.h "lldb/Expression/GoUserExpression.h"
/// @brief Encapsulates a single expression for use with Go
///
/// LLDB uses expressions for various purposes, notably to call functions
/// and as a backend for the expr command.  GoUserExpression encapsulates
/// the objects needed to parse and interpret an expression.
//----------------------------------------------------------------------
class GoUserExpression : public UserExpression
{
  public:
    GoUserExpression(ExecutionContextScope &exe_scope, const char *expr, const char *expr_prefix,
                     lldb::LanguageType language, ResultType desired_type, const EvaluateExpressionOptions &options);

    bool
    Parse(Stream &error_stream, ExecutionContext &exe_ctx, lldb_private::ExecutionPolicy execution_policy,
          bool keep_result_in_memory, bool generate_debug_info) override;

    lldb::ExpressionResults
    Execute(Stream &error_stream, ExecutionContext &exe_ctx,
            const EvaluateExpressionOptions &options,
            lldb::UserExpressionSP &shared_ptr_to_me,
            lldb::ExpressionVariableSP &result) override;

    bool
    CanInterpret() override
    {
        return true;
    }
    bool
    FinalizeJITExecution(Stream &error_stream, ExecutionContext &exe_ctx, lldb::ExpressionVariableSP &result,
                         lldb::addr_t function_stack_bottom = LLDB_INVALID_ADDRESS,
                         lldb::addr_t function_stack_top = LLDB_INVALID_ADDRESS) override
    {
        return true;
    }

  private:
    class GoInterpreter;
    std::unique_ptr<GoInterpreter> m_interpreter;
};

} // namespace lldb_private

#endif // liblldb_GoUserExpression_h_
