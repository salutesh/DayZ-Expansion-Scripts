/* TODO: Move to Community Framework */

class ExpressionFunction
{
	int index;
	
	int params;
	
	int precedence;
	bool associative;
		
	void ExpressionFunction()
	{
		params = 0;
		
		precedence = 1;
		associative = false;
	}

	void Call( ref ExpressionInstruction instruction, inout float stack[16], inout int stackPointer );
};

class ExpressionFunctionPow: ExpressionFunction
{
	void ExpressionFunctionPow()
	{
		precedence = 4;
		associative = false;
	}

	override void Call( ref ExpressionInstruction instruction, inout float stack[16], inout int stackPointer )
	{
		float numA = stack[stackPointer--];
		float numB = stack[stackPointer];
		stack[stackPointer] = Math.Pow( numA, numB );
	}
};

class ExpressionFunctionMul: ExpressionFunction
{
	void ExpressionFunctionMul()
	{
		precedence = 3;
		associative = true;
	}

	override void Call( ref ExpressionInstruction instruction, inout float stack[16], inout int stackPointer )
	{
		float numA = stack[stackPointer--];
		float numB = stack[stackPointer];
		stack[stackPointer] = numA * numB;
	}
};

class ExpressionFunctionDiv: ExpressionFunction
{
	void ExpressionFunctionDiv()
	{
		precedence = 3;
		associative = true;
	}

	override void Call( ref ExpressionInstruction instruction, inout float stack[16], inout int stackPointer )
	{
		float numA = stack[stackPointer--];
		float numB = stack[stackPointer];
		stack[stackPointer] = numB / numA;
	}
};

class ExpressionFunctionAdd: ExpressionFunction
{
	void ExpressionFunctionAdd()
	{
		precedence = 2;
		associative = true;
	}

	override void Call( ref ExpressionInstruction instruction, inout float stack[16], inout int stackPointer )
	{
		float numA = stack[stackPointer--];
		float numB = stack[stackPointer];
		stack[stackPointer] = numA + numB;
	}
};

class ExpressionFunctionSub: ExpressionFunction
{
	void ExpressionFunctionSub()
	{
		precedence = 2;
		associative = true;
	}

	override void Call( ref ExpressionInstruction instruction, inout float stack[16], inout int stackPointer )
	{
		float numA = stack[stackPointer--];
		float numB = stack[stackPointer];
		stack[stackPointer] = numB - numA;
	}
};

class ExpressionFunctionFactor: ExpressionFunction
{
	void ExpressionFunctionFactor()
	{
		params = 2;
	}
	
	override void Call( ref ExpressionInstruction instruction, inout float stack[16], inout int stackPointer )
	{
		stack[stackPointer] = Math.Interpolate( stack[stackPointer], instruction.param1, instruction.param2, 0.0, 1.0 );
	}
};

class ExpressionFunctionCos: ExpressionFunction
{
	override void Call( ref ExpressionInstruction instruction, inout float stack[16], inout int stackPointer )
	{
		stack[stackPointer] = Math.Cos( stack[stackPointer] );
	}
};

class ExpressionFunctionSin: ExpressionFunction
{
	override void Call( ref ExpressionInstruction instruction, inout float stack[16], inout int stackPointer )
	{
		stack[stackPointer] = Math.Sin( stack[stackPointer] );
	}
};

class ExpressionFunctionMin: ExpressionFunction
{
	override void Call( ref ExpressionInstruction instruction, inout float stack[16], inout int stackPointer )
	{
		float numA = stack[stackPointer--];
		float numB = stack[stackPointer];
		stack[stackPointer] = Math.Min( numA, numB );
	}
};

class ExpressionFunctionMax: ExpressionFunction
{
	override void Call( ref ExpressionInstruction instruction, inout float stack[16], inout int stackPointer )
	{
		float numA = stack[stackPointer--];
		float numB = stack[stackPointer];
		stack[stackPointer] = Math.Max( numA, numB );
	}
};