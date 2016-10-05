#pragma once
#ifndef CMN_EXPRESSION_H
#define CMN_EXPRESSION_H

#include "Common.h"
#include "Stack.h"

namespace os
{
	/**
	 *  Represents an expression that can be executed.
	 *  
	 *  The expression uses an evaluation stack similar to CLR. 
	 *  The instructions are similar to MSIL but simplified as it is interpreted at runtime, 
	 *  running at a relatively higher level.
	 *
	 *  Operations will pop values out of eval stack and do the work, finally pushing the result back
	 *  to the stack. If there is a problem, stack checking will break.
	 *
	 * @remarks
	 *  TODO: additional work may be required to allow vector math.
	 */
	class Expression
	{
		friend class ExpressionCompiler;
	public:
		/**
		 *  Defines the type of opcodes.
		 */
		enum OpCode
		{
			/**
			 *  Load a data value(Datum) on the evaluation stack
			 */
			OPC_Push,

			OPC_Add,
			OPC_Sub,
			OPC_Multiply,
			OPC_Divide,
			OPC_Negate,

			/**
			 *  Bitwise operations
			 */
			OPC_And,
			OPC_Or,
			OPC_Not,

			/**
			 *  This is specifically designed to access scope using index.
			 */
			OPC_Indexer,

			OPC_Call,

			/**
			 *  Set the value, leaving a reference to the result on the stack.
			 */
			OPC_Set
		};

		~Expression();

		/**
		 *  Invoke the expression and return the result as a Datum. 
		 *  If this is executed before, the stack including previous results will be reset.
		 *  @return The result in a datum. nullptr if no result. No additional memory handling needed.
		 */
		Datum* Invoke();

		/**
		 *  Return the execution result.
		 */
		Datum* GetResult();

	private:
		/**
		 *  Represent an operation instruction.
		 */
		class Instruction
		{
		public:
			OpCode Type;

			/**
			 *  Is the operand a immediate value? 
			 *  The operands are optional.
			 * 
			 * @remark The Instruction owns the memory if it is.
			 */
			bool IsLeftImmediate;
			Datum* OperandLeft;

			bool IsRightImmediate;
			Datum* OperandRight;
			
			Instruction(OpCode op, Datum* l, bool isLImm, Datum* r, bool isRImm);
			~Instruction();
		};
		struct EvaluationStackItem
		{
			/**
			 *  The datum is owned only if it is created during evaluation OR function returns.
			 *  
			 *  Immediate values are owned by instruction, not here.
			 */
			bool IsOwned;
			Datum* Data;
		};

		Expression();

		void ResetStack();

		Stack<EvaluationStackItem> m_evaluationStack;

		/** 
		 *  All instructions of this expressions.
		 */
		Vector<Instruction*> m_instructions;
		
	};
}

#endif