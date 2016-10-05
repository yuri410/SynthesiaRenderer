#pragma once
#ifndef APOC3D_MATRIXSTACK_H
#define APOC3D_MATRIXSTACK_H

#include "Matrix.h"
#include "apoc3d/Collections/Stack.h"

namespace Apoc3D
{
	namespace Math
	{
		class APAPI MatrixStack
		{
		public:
			MatrixStack(int reserve);
			~MatrixStack();

			void PushMultply(const Matrix& mat);
			void PushMatrix(const Matrix& mat);
			bool PopMatrix();
			bool PopMatrix(Matrix& mat);

			Matrix& Peek();
			const Matrix& Peek() const;

			int getCount() const;

		private:
			Apoc3D::Collections::Stack<Matrix> m_stack;
		};

	}
}

#endif