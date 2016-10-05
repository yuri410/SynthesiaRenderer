#pragma once

#include "apoc3d/Common.h"
#include "Matrix.h"

namespace Apoc3D
{
	namespace DoubleMath
	{
		class Vector3d;
		class Vector4d;
		class Matrixd;

		/**  Defines a four component vector. */
		class Vector4d
		{
		public:
			double X;			/** The X component of the vector. */
			double Y;			/** The Y component of the vector */
			double Z;			/** The Z component of the vector. */
			double W;			/** The W component of the vector. */

			Vector4d() { }

			Vector4d(const double (&ptr)[4])
				: X(ptr[0]), Y(ptr[1]), Z(ptr[2]), W(ptr[3]) { }

			Vector4d(double x, double y, double z, double w)
				: X(x), Y(y), Z(z), W(w) { }

			Vector4d(const Vector3d& v, double w);

			bool operator==(const Vector4d &other) const	{ return other.X == X && other.Y == Y && other.Z == Z && other.W == W; }
			bool operator!=(const Vector4d &other) const	{ return !(*this == other); }

			operator double*()								{ return &X; }
			operator const double*() const					{ return &X; }

			typedef double ArrayType[4];
			ArrayType& asArray()							{ return *(ArrayType*)(&X); }
			const ArrayType& asArray() const				{ return *(const ArrayType*)(&X); }


			void operator +=(const Vector4d& vec)			{ X += vec.X; Y += vec.Y; Z += vec.Z; W += vec.W; }
			void operator -=(const Vector4d& vec)			{ X -= vec.X; Y -= vec.Y; Z -= vec.Z; W -= vec.W; }

			void operator *=(const Vector4d& vec)			{ X *= vec.X; Y *= vec.Y; Z *= vec.Z; W *= vec.W; }
			void operator *=(double scalar)					{ X *= scalar; Y *= scalar; Z *= scalar; W *= scalar; }
			void operator /=(double scalar)					{ double inv = 1.0/scalar; X *= inv; Y *= inv; Z *= inv; W *= inv; }

			Vector4d operator -() const						{ return Vector4d(-X, -Y, -Z, -W); }
			Vector4d operator -(const Vector4d& vec) const	{ return Vector4d(X - vec.X, Y - vec.Y, Z - vec.Z, W - vec.W); }
			Vector4d operator +(const Vector4d& vec) const	{ return Vector4d(X + vec.X, Y + vec.Y, Z + vec.Z, W + vec.W); }
			Vector4d operator *(const Vector4d& vec) const	{ return Vector4d(X * vec.X, Y * vec.Y, Z * vec.Z, W * vec.W); }
			Vector4d operator *(double scalar) const		{ return Vector4d(X * scalar, Y * scalar, Z * scalar, W * scalar); }
			Vector4d operator /(double scalar) const		{ scalar = 1 / scalar; return Vector4d(X * scalar, Y * scalar, Z * scalar, W * scalar); }
			
			double operator[] (int idx) const				{ assert(idx < 4); return (&X)[idx]; }

			void Load(const double* ptr)					{ X = ptr[0]; Y = ptr[1]; Z = ptr[2]; W = ptr[3]; }
			void Store(double* dest)						{ dest[0] = X; dest[1] = Y; dest[2] = Z; dest[3] = W; }

			/** Calculates the length of the vector. */
			double Length() const							{ return sqrt(X*X + Y*Y + Z*Z + W*W); }

			/** Calculates the squared length of the vector. */
			double LengthSquared() const					{ return X*X + Y*Y + Z*Z + W*W; }

			/** Converts the vector into a unit vector. */
			void NormalizeInPlace()
			{
				double length = Length();
				if (length < EPSILON)					
				{
					X = Y = Z = W = 0;
					return;
				}
				double inv = 1.0 / length;
				X *= inv; Y *= inv;
				Z *= inv; W *= inv;
			}
			Vector4d Normalized() const						{ Vector4d copy = *this; copy.NormalizeInPlace(); return copy; }

			/** Reverses the direction of the vector. */
			void NegateInPlace()							{ X = -X; Y = -Y; Z = -Z; W = -W; }
			Vector4d Negate() const							{ Vector4d copy = *this; copy.NegateInPlace(); return copy; }

			/** Calculates the distance between two vectors. */
			double Distance(const Vector4d& vec) const
			{
				double x = X - vec.X;
				double y = Y - vec.Y;
				double z = Z - vec.Z;
				double w = W - vec.W;

				return sqrt(x * x + y * y + z * z + w * w);
			}

			double DistanceSquared(const Vector4d& vec) const
			{
				double x = X - vec.X;
				double y = Y - vec.Y;
				double z = Z - vec.Z;
				double w = W - vec.W;

				return x * x + y * y + z * z + w * w;
			}

			/** Calculates the dot product of two vectors. */
			double Dot(const Vector4d& right) const	{ return X * right.X + Y * right.Y + Z * right.Z + W * right.W; }

			/** Transforms a 3D vector by the given Matrix */
			static Vector4d Transform(const Vector4d& vector, const Matrixd& transform);
			
			static Vector4d Set(double v) { return Vector4d(v, v, v, v); }

			
			static const Vector4d Zero;				/** a Vector4 with all of its components set to zero. */
			
			static const Vector4d UnitX;			/** X unit Vector4 (1, 0, 0, 0). */
			static const Vector4d UnitY;			/** Y unit Vector4 (0, 1, 0, 0). */
			static const Vector4d UnitZ;			/** Z unit Vector4 (0, 0, 1, 0). */
			static const Vector4d UnitW;			/** W unit Vector4 (0, 0, 0, 1). */
			static const Vector4d One;

		};

		/** Defines a three component vector. */
		class Vector3d
		{
		public:
			double X;		/** the X component of the vector */
			double Y;		/** the Y component of the vector */
			double Z;		/** the Z component of the vector */

			Vector3d() { }
			Vector3d(const double (&ptr)[3])
				: X(ptr[0]), Y(ptr[1]), Z(ptr[2]) { }
			
			Vector3d(double x, double y, double z)
				: X(x), Y(y), Z(z) { }

			bool operator==(const Vector3d &other) const	{ return other.X == X && other.Y == Y && other.Z == Z;  }
			bool operator!=(const Vector3d &other) const	{ return !(*this == other); }

			operator double*()								{ return &X; }
			operator const double*() const					{ return &X; }

			typedef double ArrayType[3];
			ArrayType& asArray()							{ return *(ArrayType*)(&X); }
			const ArrayType& asArray() const				{ return *(const ArrayType*)(&X); }


			void operator +=(const Vector3d& vec)			{ X += vec.X; Y += vec.Y; Z += vec.Z; }
			void operator -=(const Vector3d& vec)			{ X -= vec.X; Y -= vec.Y; Z -= vec.Z; }

			void operator *=(const Vector3d& vec)			{ X *= vec.X; Y *= vec.Y; Z *= vec.Z; }
			void operator *=(double scalar)					{ X *= scalar; Y *= scalar; Z *= scalar; }
			void operator /=(double scalar)					{ double inv = 1.0f/scalar; X *= inv; Y *= inv; Z *= inv; }

			Vector3d operator -() const						{ return Vector3d(-X, -Y, -Z); }
			Vector3d operator -(const Vector3d& vec) const	{ return Vector3d(X - vec.X, Y - vec.Y, Z - vec.Z); }
			Vector3d operator +(const Vector3d& vec) const	{ return Vector3d(X + vec.X, Y + vec.Y, Z + vec.Z); }
			Vector3d operator *(const Vector3d& vec) const	{ return Vector3d(X * vec.X, Y * vec.Y, Z * vec.Z); }
			Vector3d operator *(double scalar) const		{ return Vector3d(X * scalar, Y * scalar, Z * scalar); }
			Vector3d operator /(double scalar) const		{ scalar = 1 / scalar; return Vector3d(X * scalar, Y * scalar, Z * scalar); }
			
			double operator[] (int idx) const				{ assert(idx < 3); return (&X)[idx]; }

			void Load(const double* ptr)					{ X = ptr[0]; Y = ptr[1]; Z = ptr[2]; }
			void Store(double* dest) const					{ dest[0] = X; dest[1] = Y; dest[2] = Z; }

			/** Calculates the length of the vector. */
			double Length() const							{ return sqrt(X*X + Y*Y + Z*Z); }

			/** Calculates the squared length of the vector. */
			double LengthSquared() const					{ return X*X + Y*Y + Z*Z; }

			/** Converts the vector into a unit vector. */
			void NormalizeInPlace()
			{
				double length = Length();
				if (length < EPSILON)					
				{
					X = Y = Z = 0;
					return;
				}
				double invLen = 1.0f / length;
				X *= invLen; Y *= invLen; Z *= invLen;
			}
			Vector3d Normalized() const						{ Vector3d copy = *this; copy.NormalizeInPlace(); return copy; }

			/** Reverses the direction of the vector. */
			void NegateInPlace()							{ X = -X; Y = -Y; Z = -Z; }
			Vector3d Negate() const							{ Vector3d copy = *this; copy.NegateInPlace(); return copy; }

			
			Vector3d Cross(const Vector3d& b) const 
			{
				Vector3d result;
				result.X = Y * b.Z - Z * b.Y;
				result.Y = Z * b.X - X * b.Z;
				result.Z = X * b.Y - Y * b.X;
				return result;
			}

			/** Calculates the distance between two vectors. */
			double Distance(const Vector3d& vec) const
			{
				double x = X - vec.X;
				double y = Y - vec.Y;
				double z = Z - vec.Z;

				return sqrt(x * x + y * y + z * z);
			}
			double DistanceSquared(const Vector3d& vec) const
			{
				double x = X - vec.X;
				double y = Y - vec.Y;
				double z = Z - vec.Z;

				return x * x + y * y + z * z;
			}

			/** Calculates the dot product of two vectors. */
			double Dot(const Vector3d& right) const			{ return X * right.X + Y * right.Y + Z * right.Z; }

			static Vector3d Set(double v)					{ return Vector3d(v, v, v); }


			/** Calculates the distance between two vectors. */
			static double Distance(const Vector3d& value1, const Vector3d& value2)
			{
				double x = value1.X - value2.X;
				double y = value1.Y - value2.Y;
				double z = value1.Z - value2.Z;

				return sqrt(x * x + y * y + z * z);
			}

			/** Calculates the squared distance between two vectors. */
			static double DistanceSquared(const Vector3d& value1, const Vector3d& value2)
			{
				double x = value1.X - value2.X;
				double y = value1.Y - value2.Y;
				double z = value1.Z - value2.Z;

				return x * x + y * y + z * z;
			}

			/** Calculates the dot product of two vectors. */
			static double Dot(const Vector3d& left, const Vector3d& right) { return left.X * right.X + left.Y * right.Y + left.Z * right.Z; }

			/** Calculates the cross product of two vectors. */
			static Vector3d Cross(const Vector3d& left, const Vector3d& right)
			{
				Vector3d result;
				result.X = left.Y * right.Z - left.Z * right.Y;
				result.Y = left.Z * right.X - left.X * right.Z;
				result.Z = left.X * right.Y - left.Y * right.X;
				return result;
			}


			/** Transforms a 3D vector by the given Matrix */
			static Vector4d Transform(const Vector3d& vector, const Matrixd& transform);
			
			/** Performs a coordinate transformation using the given Matrix. can not project */
			static Vector3d TransformSimple(const Vector3d& vector, const Matrixd& transform);
			
			/** Performs a coordinate transformation using the given Matrix */
			static Vector3d TransformCoordinate(const Vector3d& vector, const Matrixd& transform);
			
			/** Performs a normal transformation using the given Matrix */
			static Vector3d TransformNormal(const Vector3d& vector, const Matrixd& transform);



			static const Vector3d Zero;			/** a Vector3 with all of its components set to zero. */
			static const Vector3d UnitX;		/** X unit Vector3 (1, 0, 0). */
			static const Vector3d UnitY;		/** Y unit Vector3 (0, 1, 0). */
			static const Vector3d UnitZ;		/** Z unit Vector3 (0, 0, 1). */
			static const Vector3d One;
		};
		
		inline Vector4d::Vector4d(const Vector3d& v, double w)
			: X(v.X), Y(v.Y), Z(v.Z), W(w)
		{ }

		/** Defines a 4x4 matrix. */
		class Matrixd
		{
		public:
			const static Matrixd Identity;

			union
			{
				struct
				{
					double Elements[16];
				};
				struct
				{
					double M11, M12, M13, M14;
					double M21, M22, M23, M24;
					double M31, M32, M33, M34;
					double M41, M42, M43, M44;
				};
			};
			
			Matrixd() {  ZeroMatrix(); }
			explicit Matrixd(const double(&elements)[16])
			{		
				CopyArray(Elements, elements);
			}

			Matrixd(double f11, double f12, double f13, double f14,
				double f21, double f22, double f23, double f24,
				double f31, double f32, double f33, double f34,
				double f41, double f42, double f43, double f44)
				: M11(f11), M12(f12), M13(f13), M14(f14),
				M21(f21), M22(f22), M23(f23), M24(f24),
				M31(f31), M32(f32), M33(f33), M34(f34),
				M41(f41), M42(f42), M43(f43), M44(f44) 
			{ }

			operator double* ()								{ return reinterpret_cast<double*>(this); }
			operator const double*() const					{ return reinterpret_cast<const double*>(this); }
			
			Vector3d GetX() const { return Vector3d(M11, M12, M13); }
			Vector3d GetY() const { return Vector3d(M21, M22, M23); }
			Vector3d GetZ() const { return Vector3d(M31, M32, M33); }	
			Vector3d GetTranslation() const { return Vector3d(M41, M42, M43); }


			void SetX(const Vector3d& v) { M11 = v.X; M12 = v.Y; M13 = v.Z; }
			void SetY(const Vector3d& v) { M21 = v.X; M22 = v.Y; M23 = v.Z; }
			void SetZ(const Vector3d& v) { M31 = v.X; M32 = v.Y; M33 = v.Z; }


			void SetTranslation(const Vector3d& v) { M41 = v.X; M42 = v.Y; M43 = v.Z; }
			void SetTranslation(double x, double y, double z) { M41 = x; M42 = y; M43 = z; }

			void SetXYZ(const Vector3d& x, const Vector3d& y, const Vector3d& z)
			{
				SetX(x); SetY(y); SetZ(z);
			}

			void LoadIdentity()
			{
				M11 = 1; M12 = 0; M13 = 0; M14 = 0;
				M21 = 0; M22 = 1; M23 = 0; M24 = 0;
				M31 = 0; M32 = 0; M33 = 1; M34 = 0;
				M41 = 0; M42 = 0; M43 = 0; M44 = 1;
			}

			/** Transposes the matrix */
			void Transpose()
			{
				double e[16];
				e[ME(1, 1)] = Elements[ME(1, 1)];
				e[ME(1, 2)] = Elements[ME(2, 1)];
				e[ME(1, 3)] = Elements[ME(3, 1)];
				e[ME(1, 4)] = Elements[ME(4, 1)];
				e[ME(2, 1)] = Elements[ME(1, 2)];
				e[ME(2, 2)] = Elements[ME(2, 2)];
				e[ME(2, 3)] = Elements[ME(3, 2)];
				e[ME(2, 4)] = Elements[ME(4, 2)];
				e[ME(3, 1)] = Elements[ME(1, 3)];
				e[ME(3, 2)] = Elements[ME(2, 3)];
				e[ME(3, 3)] = Elements[ME(3, 3)];
				e[ME(3, 4)] = Elements[ME(4, 3)];
				e[ME(4, 1)] = Elements[ME(1, 4)];
				e[ME(4, 2)] = Elements[ME(2, 4)];
				e[ME(4, 3)] = Elements[ME(3, 4)];
				e[ME(4, 4)] = Elements[ME(4, 4)];
				memcpy(Elements, e, sizeof(e));
			}

			/** Inverts the matrix. */
			double Inverse();
			

			/** Calculates the determinant of the matrix. */
			double Determinant() const
			{
				double temp1 = (M33 * M44) - (M34 * M43);
				double temp2 = (M32 * M44) - (M34 * M42);
				double temp3 = (M32 * M43) - (M33 * M42);
				double temp4 = (M31 * M44) - (M34 * M41);
				double temp5 = (M31 * M43) - (M33 * M41);
				double temp6 = (M31 * M42) - (M32 * M41);

				return M11 * (M22 * temp1 - M23 * temp2 + M24 * temp3) - 
					M12 * (M21 * temp1 - M23 * temp4 + M24 * temp5) + 
					M13 * (M21 * temp2 - M22 * temp4 + M24 * temp6) -
					M14 * (M21 * temp3 - M22 * temp5 + M23 * temp6);
			}


			void ZeroMatrix()
			{
				for (int i = 0; i < 16; i++) 
					Elements[i] = 0;
			}

			/**
			 *  Determines the product of two matrices.
			 *  The result matrix cannot reference to the memory that ma or mb is referring to, or the result is not correct.
			*/
			static void Multiply(Matrixd& res, const Matrixd& ma, const Matrixd& mb)
			{
				assert(&ma != &res && &mb != &res);
		
	#if _DEBUG
				res.M11 = (ma.M11 * mb.M11) + (ma.M12 * mb.M21) + (ma.M13 * mb.M31) + (ma.M14 * mb.M41);
				res.M12 = (ma.M11 * mb.M12) + (ma.M12 * mb.M22) + (ma.M13 * mb.M32) + (ma.M14 * mb.M42);
				res.M13 = (ma.M11 * mb.M13) + (ma.M12 * mb.M23) + (ma.M13 * mb.M33) + (ma.M14 * mb.M43);
				res.M14 = (ma.M11 * mb.M14) + (ma.M12 * mb.M24) + (ma.M13 * mb.M34) + (ma.M14 * mb.M44);

				res.M21 = (ma.M21 * mb.M11) + (ma.M22 * mb.M21) + (ma.M23 * mb.M31) + (ma.M24 * mb.M41);
				res.M22 = (ma.M21 * mb.M12) + (ma.M22 * mb.M22) + (ma.M23 * mb.M32) + (ma.M24 * mb.M42);
				res.M23 = (ma.M21 * mb.M13) + (ma.M22 * mb.M23) + (ma.M23 * mb.M33) + (ma.M24 * mb.M43);
				res.M24 = (ma.M21 * mb.M14) + (ma.M22 * mb.M24) + (ma.M23 * mb.M34) + (ma.M24 * mb.M44);

				res.M31 = (ma.M31 * mb.M11) + (ma.M32 * mb.M21) + (ma.M33 * mb.M31) + (ma.M34 * mb.M41);
				res.M32 = (ma.M31 * mb.M12) + (ma.M32 * mb.M22) + (ma.M33 * mb.M32) + (ma.M34 * mb.M42);
				res.M33 = (ma.M31 * mb.M13) + (ma.M32 * mb.M23) + (ma.M33 * mb.M33) + (ma.M34 * mb.M43);
				res.M34 = (ma.M31 * mb.M14) + (ma.M32 * mb.M24) + (ma.M33 * mb.M34) + (ma.M34 * mb.M44);

				res.M41 = (ma.M41 * mb.M11) + (ma.M42 * mb.M21) + (ma.M43 * mb.M31) + (ma.M44 * mb.M41);
				res.M42 = (ma.M41 * mb.M12) + (ma.M42 * mb.M22) + (ma.M43 * mb.M32) + (ma.M44 * mb.M42);
				res.M43 = (ma.M41 * mb.M13) + (ma.M42 * mb.M23) + (ma.M43 * mb.M33) + (ma.M44 * mb.M43);
				res.M44 = (ma.M41 * mb.M14) + (ma.M42 * mb.M24) + (ma.M43 * mb.M34) + (ma.M44 * mb.M44);
	#else
				double r11 = (ma.M11 * mb.M11) + (ma.M12 * mb.M21) + (ma.M13 * mb.M31) + (ma.M14 * mb.M41);
				double r12 = (ma.M11 * mb.M12) + (ma.M12 * mb.M22) + (ma.M13 * mb.M32) + (ma.M14 * mb.M42);
				double r13 = (ma.M11 * mb.M13) + (ma.M12 * mb.M23) + (ma.M13 * mb.M33) + (ma.M14 * mb.M43);
				double r14 = (ma.M11 * mb.M14) + (ma.M12 * mb.M24) + (ma.M13 * mb.M34) + (ma.M14 * mb.M44);

				double r21 = (ma.M21 * mb.M11) + (ma.M22 * mb.M21) + (ma.M23 * mb.M31) + (ma.M24 * mb.M41);
				double r22 = (ma.M21 * mb.M12) + (ma.M22 * mb.M22) + (ma.M23 * mb.M32) + (ma.M24 * mb.M42);
				double r23 = (ma.M21 * mb.M13) + (ma.M22 * mb.M23) + (ma.M23 * mb.M33) + (ma.M24 * mb.M43);
				double r24 = (ma.M21 * mb.M14) + (ma.M22 * mb.M24) + (ma.M23 * mb.M34) + (ma.M24 * mb.M44);

				double r31 = (ma.M31 * mb.M11) + (ma.M32 * mb.M21) + (ma.M33 * mb.M31) + (ma.M34 * mb.M41);
				double r32 = (ma.M31 * mb.M12) + (ma.M32 * mb.M22) + (ma.M33 * mb.M32) + (ma.M34 * mb.M42);
				double r33 = (ma.M31 * mb.M13) + (ma.M32 * mb.M23) + (ma.M33 * mb.M33) + (ma.M34 * mb.M43);
				double r34 = (ma.M31 * mb.M14) + (ma.M32 * mb.M24) + (ma.M33 * mb.M34) + (ma.M34 * mb.M44);

				double r41 = (ma.M41 * mb.M11) + (ma.M42 * mb.M21) + (ma.M43 * mb.M31) + (ma.M44 * mb.M41);
				double r42 = (ma.M41 * mb.M12) + (ma.M42 * mb.M22) + (ma.M43 * mb.M32) + (ma.M44 * mb.M42);
				double r43 = (ma.M41 * mb.M13) + (ma.M42 * mb.M23) + (ma.M43 * mb.M33) + (ma.M44 * mb.M43);
				double r44 = (ma.M41 * mb.M14) + (ma.M42 * mb.M24) + (ma.M43 * mb.M34) + (ma.M44 * mb.M44);

				res.M11 = r11; res.M12 = r12; res.M13 = r13; res.M14 = r14;
				res.M21 = r21; res.M22 = r22; res.M23 = r23; res.M24 = r24;
				res.M31 = r31; res.M32 = r32; res.M33 = r33; res.M34 = r34;
				res.M41 = r41; res.M42 = r42; res.M43 = r43; res.M44 = r44;

	#endif
			}

			/** Scales a matrix by the given value. */
			static void Multiply(Matrixd& res, const Matrixd& ma, double mb)
			{
				res.M11 = ma.M11 * mb;
				res.M12 = ma.M12 * mb;
				res.M13 = ma.M13 * mb;
				res.M14 = ma.M14 * mb;
				res.M21 = ma.M21 * mb;
				res.M22 = ma.M22 * mb;
				res.M23 = ma.M23 * mb;
				res.M24 = ma.M24 * mb;
				res.M31 = ma.M31 * mb;
				res.M32 = ma.M32 * mb;
				res.M33 = ma.M33 * mb;
				res.M34 = ma.M34 * mb;
				res.M41 = ma.M41 * mb;
				res.M42 = ma.M42 * mb;
				res.M43 = ma.M43 * mb;
				res.M44 = ma.M44 * mb;
			}

			/** Creates a matrix that rotates around the x-axis. */
			static void CreateRotationX(Matrixd& res, double angle)
			{
				double _cos = cos(angle);
				double _sin = sin(angle);
				
				res.M11 = 1;	res.M12 = 0;		res.M13 = 0;		res.M14 = 0;
				res.M21 = 0;	res.M22 = _cos;		res.M23 = _sin;		res.M24 = 0;
				res.M31 = 0;	res.M32 = -_sin;	res.M33 = _cos;	res.M34 = 0;
				res.M41 = 0;	res.M42 = 0;		res.M43 = 0;		res.M44 = 1;

			}

			/** Creates a matrix that rotates around the y-axis. */
			static void CreateRotationY(Matrixd& res, double angle)
			{
				double _cos = cos(angle);
				double _sin = sin(angle);

				res.M11 = _cos;	res.M12 = 0;	res.M13 = -_sin;	res.M14 = 0;
				res.M21 = 0;	res.M22 = 1;	res.M23 = 0;		res.M24 = 0;
				res.M31 = _sin;	res.M32 = 0;	res.M33 = _cos;		res.M34 = 0;
				res.M41 = 0;	res.M42 = 0;	res.M43 = 0;		res.M44 = 1;
			}

			/** Creates a matrix that rotates around the z-axis. */
			static void CreateRotationZ(Matrixd& res, double angle)
			{
				double _cos = cos(angle);
				double _sin = sin(angle);
				
				res.M11 = _cos;	res.M12 = _sin;	res.M13 = 0;	res.M14 = 0;
				res.M21 = -_sin;	res.M22 = _cos;	res.M23 = 0;	res.M24 = 0;
				res.M31 = 0;	res.M32 = 0;	res.M33 = 1;	res.M34 = 0;
				res.M41 = 0;	res.M42 = 0;	res.M43 = 0;	res.M44 = 1;

			}

			/** Creates a matrix that rotates around an arbitary axis. */
			static void CreateRotationAxis(Matrixd& res, Vector3d axis, double angle)
			{
				if (axis.LengthSquared() != 1.0)
				{
					axis.NormalizeInPlace();
				}
				double x = axis.X;
				double y = axis.Y;
				double z = axis.Z;
				double _cos = cos(angle);
				double _sin = sin(angle);
				double xx = x * x;
				double yy = y * y;
				double zz = z * z;
				double xy = x * y;
				double xz = x * z;
				double yz = y * z;

				res.M11 = xx + _cos * (1 - xx);
				res.M12 = xy - _cos * xy + _sin * z;
				res.M13 = xz - _cos * xz - _sin * y;
				res.M14 = 0;

				res.M21 = xy - _cos * xy - _sin * z;
				res.M22 = yy + _cos * (1 - yy);
				res.M23 = yz - _cos * yz + _sin * x;
				res.M24 = 0;

				res.M31 = xz - _cos * xz + _sin * y;
				res.M32 = yz - _cos * yz - _sin * x;
				res.M33 = zz + _cos * (1 - zz);
				res.M34 = 0;

				res.M41 = res.M42 = res.M43 = 0.0f;
				res.M44 = 1;
			}

			static void CreateRotationYawPitchRoll(Matrixd& result, double yaw, double pitch, double roll);

			static void CreateTranslation(Matrixd& res, const Vector3d& pos)
			{
				CreateTranslation(res, pos.X, pos.Y, pos.Z);
			}

			/** Creates a translation matrix using the specified offsets. */
			static void CreateTranslation(Matrixd& res, double x, double y, double z)
			{
				res.M11 = 1; res.M12 = 0; res.M13 = 0; res.M14 = 0;
				res.M21 = 0; res.M22 = 1; res.M23 = 0; res.M24 = 0;
				res.M31 = 0; res.M32 = 0; res.M33 = 1; res.M34 = 0;
				res.M41 = x; res.M42 = y; res.M43 = z; res.M44 = 1;
			}

			/** Creates a matrix that scales along the x-axis, y-axis, and y-axis. */
			static void CreateScale(Matrixd& res, double x, double y, double z)
			{
				res.M11 = x; res.M12 = 0; res.M13 = 0; res.M14 = 0;
				res.M21 = 0; res.M22 = y; res.M23 = 0; res.M24 = 0;
				res.M31 = 0; res.M32 = 0; res.M33 = z; res.M34 = 0;
				res.M41 = 0; res.M42 = 0; res.M43 = 0; res.M44 = 1;
			}

			/** Creates a left-handed, look-at matrix. */
			static void CreateLookAtLH(Matrixd& res, Vector3d cameraPosition, Vector3d cameraTarget, Vector3d up)
			{
				Vector3d zaxis = cameraTarget - cameraPosition;
				zaxis.NormalizeInPlace();

				Vector3d xaxis = Vector3d::Cross(up, zaxis);
				xaxis.NormalizeInPlace();

				Vector3d yaxis = Vector3d::Cross(zaxis, xaxis);

				res.M11 = xaxis.X;
				res.M12 = yaxis.X;
				res.M13 = zaxis.X;
				res.M14 = 0;

				res.M21 = xaxis.Y;
				res.M22 = yaxis.Y;
				res.M23 = zaxis.Y;
				res.M24 = 0;

				res.M31 = xaxis.Z;
				res.M32 = yaxis.Z;
				res.M33 = zaxis.Z;
				res.M34 = 0;

				res.M41 = -Vector3d::Dot(xaxis, cameraPosition);
				res.M42 = -Vector3d::Dot(yaxis, cameraPosition);
				res.M43 = -Vector3d::Dot(zaxis, cameraPosition);
				res.M44 = 1;
			}

			/** Creates a right-handed, look-at matrix. */
			static void CreateLookAtRH(Matrixd& res, Vector3d cameraPosition, Vector3d cameraTarget, Vector3d up)
			{
				Vector3d zaxis = cameraPosition - cameraTarget;
				zaxis.NormalizeInPlace();

				Vector3d xaxis = Vector3d::Cross(up, zaxis);
				xaxis.NormalizeInPlace();

				Vector3d yaxis = Vector3d::Cross(zaxis, xaxis);

				res.M11 = xaxis.X;
				res.M12 = yaxis.X;
				res.M13 = zaxis.X;
				res.M14 = 0;

				res.M21 = xaxis.Y;
				res.M22 = yaxis.Y;
				res.M23 = zaxis.Y;
				res.M24 = 0;

				res.M31 = xaxis.Z;
				res.M32 = yaxis.Z;
				res.M33 = zaxis.Z;
				res.M34 = 0;

				res.M41 = -Vector3d::Dot(xaxis, cameraPosition);
				res.M42 = -Vector3d::Dot(yaxis, cameraPosition);
				res.M43 = -Vector3d::Dot(zaxis, cameraPosition);
				res.M44 = 1;
			}
			
			/**
			 *  Creates a left-handed, perspective projection matrix based on a field of view.
			 *  
			 *  This function computes the returned matrix as shown:
			 *  
			 *  xScale     0          0               0
			 *  0        yScale       0               0
			 *  0          0       zf/(zf-zn)         1
			 *  0          0       -zn*zf/(zf-zn)     0
			 *  where:
			 *  yScale = cot(fovY/2)
			 *  
			 *  xScale = yScale / aspect ratio
			 */
			static void CreatePerspectiveFovLH(Matrixd& res, double fieldOfView, double aspectRatio, double nearPlaneDistance, double farPlaneDistance)
			{
				double yScale = 1.0 / (tan(fieldOfView * 0.5));
				double xScale = yScale / aspectRatio;

				res.M11 = xScale;	res.M12 = 0;		res.M13 = 0; res.M14 = 0;
				res.M21 = 0;		res.M22 = yScale;	res.M23 = 0; res.M24 = 0;

				res.M31 = 0;
				res.M32 = 0;
				res.M33 = farPlaneDistance / (farPlaneDistance - nearPlaneDistance);
				res.M34 = 1;

				res.M41 = 0;
				res.M42 = 0;
				res.M43 = -(nearPlaneDistance * farPlaneDistance) / (farPlaneDistance - nearPlaneDistance);
				res.M44 = 0;
			}

			/**
			 *  Creates a right-handed, perspective projection matrix based on a field of view.
			 *  
			 *  This function computes the returned matrix as shown:
			 *  
			 *  xScale     0          0               0
			 *  0        yScale       0               0
			 *  0          0       zf/(zn-zf)        -1
			 *  0          0       zn*zf/(zn-zf)      0
			 *  where:
			 *  yScale = cot(fovY/2)
			 *  
			 *  xScale = yScale / aspect ratio
			 */
			static void CreatePerspectiveFovRH(Matrixd& res, double fieldOfView, double aspectRatio, double nearPlaneDistance, double farPlaneDistance)
			{
				double yScale = 1.0 / (tan(fieldOfView * 0.5));
				double xScale = yScale / aspectRatio;

				res.M11 = xScale;	res.M12 = 0;		res.M13 = 0; res.M14 = 0;
				res.M21 = 0;		res.M22 = yScale;	res.M23 = 0; res.M24 = 0;

				res.M31 = 0;
				res.M32 = 0;
				res.M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
				res.M34 = -1;

				res.M41 = 0;
				res.M42 = 0;
				res.M43 = (nearPlaneDistance * farPlaneDistance) / (nearPlaneDistance - farPlaneDistance);
				res.M44 = 0;
			}

			/**
			 *  Creates a left-handed, orthographic projection matrix.
			 *  
			 *  All the parameters of the function are distances in camera space. The parameters describe the dimensions of the view volume.
			 *   2/w  0    0           0
			 *   0    2/h  0           0
			 *   0    0    1/(zf-zn)   0
			 *   0    0   -zn/(zf-zn)  1
			 */
			static void CreateOrthoLH(Matrixd& res, double width, double height, double zNearPlane, double zFarPlane)
			{
				res.M11 = 2 / width;	res.M12 = 0;			res.M13 = 0;	res.M14 = 0;
				res.M21 = 0;			res.M22 = 2 / height;	res.M23 = 0;	res.M24 = 0;

				res.M31 = 0;
				res.M32 = 0;
				res.M33 = 1 / (zFarPlane - zNearPlane);
				res.M34 = 0;

				res.M41 = 0;
				res.M42 = 0;
				res.M43 = -zNearPlane / (zFarPlane - zNearPlane);
				res.M44 = 1;
			}

			/**
			 *  Creates a right-handed, orthographic projection matrix.
			 *  
			 *  All the parameters of the function are distances in camera space.
			 *  The parameters describe the dimensions of the view volume.
			 *  
			 *  This function uses the following formula to compute the result matrix. 
			 *   2/w  0    0           0
			 *   0    2/h  0           0
			 *   0    0    1/(zn-zf)   0
			 *   0    0    zn/(zn-zf)  l
			 */
			static void CreateOrthoRH(Matrixd& res, double width, double height, double zNearPlane, double zFarPlane)
			{
				res.M11 = 2 / width;	res.M12 = 0;			res.M13 = 0;	res.M14 = 0;
				res.M21 = 0;			res.M22 = 2 / height;	res.M23 = 0;	res.M24 = 0;

				res.M31 = 0;
				res.M32 = 0;
				res.M33 = 1 / (zNearPlane - zFarPlane);
				res.M34 = 0;

				res.M41 = 0;
				res.M42 = 0;
				res.M43 = zNearPlane / (zNearPlane - zFarPlane);
				res.M44 = 1;
			}

			static void Inverse(Matrixd& res, const Matrixd& matrix)
			{
				res = matrix;
				res.Inverse();
			}

			bool operator ==(const Matrixd& value) const
			{
				return (M11 == value.M11 && M12 == value.M12 && M13 == value.M13 && M14 == value.M14 &&
					M21 == value.M21 && M22 == value.M22 && M23 == value.M23 && M24 == value.M24 &&
					M31 == value.M31 && M32 == value.M32 && M33 == value.M33 && M34 == value.M34 &&
					M41 == value.M41 && M42 == value.M42 && M43 == value.M43 && M44 == value.M44);
			}
			bool operator!=(const Matrixd& rhs) const { return !(*this == rhs); }

		};

		inline Vector3d vec3ftod(Math::Vector3 v) { return Vector3d(v.X, v.Y, v.Z); }
		inline Math::Vector3 vec3dtof(Vector3d v) { return Math::Vector3((float)v.X, (float)v.Y, (float)v.Z); }
		
		inline Vector4d vec4ftod(Math::Vector4 v) { return Vector4d(v.X, v.Y, v.Z, v.W); }
		inline Math::Vector4 vec4dtof(Vector4d v) { return Math::Vector4((float)v.X, (float)v.Y, (float)v.Z, (float)v.W); }

		inline Matrixd matftod(Math::Matrix m)
		{
			Matrixd r;
			for (int32 i = 0; i < 16; i++)
				r.Elements[i] = m.Elements[i];
			return r;
		}
		inline Math::Matrix matdtof(Matrixd m)
		{
			Math::Matrix r;
			for (int32 i = 0; i < 16; i++)
				r.Elements[i] = (float)m.Elements[i];
			return r;
		}

		class RaySegmentd
		{
		public:
			Vector3d Start;
			Vector3d End;
		};

		RaySegmentd raysegdtof(const Math::RaySegment& m);
	}

}