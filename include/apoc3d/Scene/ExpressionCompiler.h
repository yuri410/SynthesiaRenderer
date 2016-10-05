#pragma once
#ifndef CMN_EXPRESSION_COMPILER_H
#define CMN_EXPRESSION_COMPILER_H

#include "Common.h"
#include "Vector.h"
#include "Stack.h"
#include "HashMap.h"
#include "RTTI.h"

namespace os
{
	/**
	 *  Definition information on the one type of operator
	 */
	struct OperatorDef
	{
		enum OperatorType
		{
			OP_Add = 0,
			OP_Sub,
			OP_Multiply,
			OP_Divide,
			OP_Negate,

			OP_And,
			OP_Or,
			OP_Not,

			OP_LeftBracket,
			OP_RightBracket,
			OP_LeftIndexer,
			OP_RightIndexer,

			/**
			 *  Currently this comma operator is designed for function arguments only.
			 *  No actually operation between the arguments will be done.
			 */
			OP_Comma,
			OP_Call,
			OP_CallIndexer,

			OP_Assignment
		};
		
		OperatorType Type;

		char OperatorChar;

		/**
		 *  Priority when this operator is compared on the left side
		 */
		int32 PriorityLeft;
		/**
		 *  Priority when this operator is compared on the right side
		 */
		int32 PriorityRight;

		/**
		 *  Is this binary operation?
		 */
		bool IsBinary;

		bool EmitOpcode;
	};

	/**
	 *  Represents a node in the expression tree
	 */
	class ExpressionNode
	{
	public:
		enum 
		{
			/**
			 *  Means the node will be an operation on the operands which are represented by children(sub tree).
			 */
			EN_Operator,
			/** 
			 *  The node will be a leaf node representing a immediate value or a reference
			 */
			EN_Operand
		} Type;

		/**
		 *  The operator the node represents, if the node is operator type.
		 */
		const OperatorDef* Operator;
		/**
		 *  The operator the node represents, if the node is operand type. 
		 */
		String VarableName;

		/**
		 *  Optional operand of the operator represented by a subtree
		 */
		ExpressionNode* Left;
		ExpressionNode* Right;

		ExpressionNode(const String& varName): Left(nullptr), Right(nullptr), Operator(nullptr), VarableName(varName), Type(EN_Operand) { }
		ExpressionNode(const OperatorDef& op, ExpressionNode* leftOperand, ExpressionNode* rightOperand)
			: Operator(&op), Left(leftOperand), Right(rightOperand), Type(EN_Operator) { }

		~ExpressionNode()
		{
			if (Left) delete Left;
			if (Right) delete Right;
		}
	};

	class Expression;

	/**
	 *  An compiler translating the expression string into lower level instructions which can be interpreted
	 */
	class ExpressionCompiler
	{
	public:
		ExpressionCompiler() { }
		~ExpressionCompiler() { }

		/**
		 *  Do the compilation
		 * @param runtime The local Scope the expression is running. 
		 * @remarks 
		 *   TODO: An alternative Scope may also be needed representing the global scope. So the expression code can access anywhere.
		 */
		Expression* Compile(const String& expression, Scope* runtime);
	private:
		/** 
		 *  Simple pass on the string, extracting all the tokens.
		 * @remarks
		 *   TODO: This may need some more work for certain data representation and syntax checking.
		 */
		void ParseTokens(const String& expression);

		ExpressionNode* CreateExpressionTree();

		/** 
		 *  Stack operation based on the priority of the 2 operators
		 */
		bool ProcessOperator(const OperatorDef* ol, const OperatorDef* or, 
			Stack<const OperatorDef*>& operatorStack, Stack<ExpressionNode*>& expressionStack);

		/** 
		 *  Post order traverse emitting the instruction code.
		 */
		bool Traverse(ExpressionNode* node, Scope* runtime, Expression* result);

		/**
		 *  Resolve a token name to immediate value or a reference to some datum.
		 */
		Datum* ResolveName(const String& token, Scope* runtime, bool& isImmediate);



		Vector<String> m_expressionTokens;
		int32 m_tokenPosition;
	};

	/**
	 *  Contains function object representing built in functions
	 */
	class ExpresssionScriptLibrary
	{
	public:
		/**
		 *  This will help a function name fit in to datum as RTTI*
		 */
		class SFFunction : public RTTI
		{
			RTTI_DECLARATIONS(SFFunction, RTTI);
		public:

			/**
			 *  Run the function. A return value is allocated if any.
			 */
			virtual Datum* Invoke(Datum** parameters) = 0;
			
			const String& getName() const { return m_name; }
			int32 getParamCount() const { return m_paramCount; }

			virtual ~SFFunction() { }
		protected:
			SFFunction(const String& name, int32 paramCount) : m_name(name), m_paramCount(paramCount) { }

		private:
			String m_name;
			int32 m_paramCount;
		};

		/** 
		 *  Call function using the given name and parameters
		 *  @return A datum representing the return value. It need to be RELEASED.
		 */
		static Datum* Invoke(const String& funcName, Datum** parameters);

		/** 
		 *  Get the function descriptor.
		 */
		static Datum* GetFunctionDescriptor(const String& funcName);
		
		static void Initialize();
		static void Finalize();

		/**
		 *  Register a custom function. It will be accessible by the expression system.
		 */
		static void RegisterFunction(SFFunction* func);
		static void UnregisterFunction(SFFunction* func);
	private:

		typedef HashMap<String, std::pair<SFFunction*, Datum*>> FunctionTable;
		static FunctionTable m_lookupTable;
	};
}

#endif