/* TODO: Move to Community Framework */
#ifndef CF_COT_MOVE
class Expression
{
	string value;

	private int _position = -1;
	
	private ref array< ref ExpressionInstruction > _compiled;
	
	private ref map< string, ref ExpressionFunction > _functions;
	private ref array< ExpressionFunction > _quickLookUp;

	void Expression( string val = "" )
	{
		value = val;
		
		_compiled = new array< ref ExpressionInstruction >();
		
		_functions = new map< string, ref ExpressionFunction >();
		_quickLookUp = new array< ExpressionFunction >();
		
		AddFunction( "^", new ExpressionFunctionPow() );
		AddFunction( "*", new ExpressionFunctionMul() );
		AddFunction( "/", new ExpressionFunctionDiv() );
		AddFunction( "+", new ExpressionFunctionAdd() );
		AddFunction( "-", new ExpressionFunctionSub() );
		AddFunction( "factor", new ExpressionFunctionFactor() );
		AddFunction( "cos", new ExpressionFunctionCos() );
		AddFunction( "sin", new ExpressionFunctionSin() );
		AddFunction( "min", new ExpressionFunctionMin() );
		AddFunction( "max", new ExpressionFunctionMax() );
	}
	
	void ~Expression()
	{
		while ( _compiled.Count() > 0 )
		{
			delete _compiled[0];
			_compiled.Remove(0);
		}
		
		delete _compiled;
				
		while ( _functions.Count() > 0 )
		{
			string key = _functions.GetKey( 0 );
			delete _functions[key];
			_functions.Remove( key );
		}
		
		delete _functions;
	}
	
	void AddFunction( string name, ref ExpressionFunction function )
	{
		_functions[name] = function;
		function.index = _quickLookUp.Insert( function );
	}

	string BackChar()
	{
		_position--;
		
		if ( _position < 0 )
			_position = 0;
		
		string c = value.Substring( _position, 1 );
		
		return c;
	}

	string GetChar()
	{
		_position++;
		
		if ( _position >= value.Length() )
			_position = value.Length();
		
		string c = value.Substring( _position, 1 );
		
		return c;
	}

	bool EOF()
	{
		return _position >= value.Length() - 1;
	}

	bool IsWhitespace( string c )
	{
		if ( c == " " )
			return true;
		if ( c == "\t" )
			return true;
		if ( c == "\n" )
			return true;
		if ( c == "\r" )
			return true;

		//if ( c == "" )
		//	Error( "Unexpected end of string" );

		return false;
	}

	bool IsAlphanumeric( string c )
	{
		int i = c.Hash();
			
		if ( i >= 48 && i <= 57 )
			return true;
		
		if ( i == 46 )
			return true;
		
		if ( i == 42 )
			return true;
		
		if ( i == 43 )
			return true;

		if ( i == 44 )
			return true;
		
		if ( i == 45 )
			return true;
		
		if ( i == 47 )
			return true;

		if ( i > 90 )
			i -= 32;

		if ( i >= 65 && i <= 90 )
			return true;

		return false;
	}

	bool IsLetterOrDigit( string c )
	{
		int i = c.Hash();
			
		if ( i >= 48 && i <= 57 )
			return true;
		
		if ( i == 46 )
			return true;

		if ( i > 90 )
			i -= 32;

		if ( i >= 65 && i <= 90 )
			return true;

		return false;
	}

	string SkipWhitespace()
	{
		if ( EOF() )
			return "\0";
		
		string c = GetChar();
		while ( IsWhitespace( c ) && !EOF() )
			c = GetChar();

		return c;
	}

	string GetToken()
	{
		string word = "";
		
		string c = SkipWhitespace();
		
		if ( c == "[" || c == "]" || c == "(" || c == ")" )
			return c;
		
		if ( IsAlphanumeric( c ) && !IsLetterOrDigit( c ) )
			return c;
		
		while ( IsLetterOrDigit( c ) )
		{
			word = word + c;
			if ( EOF() )
				return word;
			
			c = GetChar();
			
			if ( c == "[" || c == "]" || c == "(" || c == ")" || word == "/" || word == "*" )
				break;
		}

		_position--;

		return word;
	}

	bool IsNumber( string word )
	{
		bool decimal = false;

		for ( int i = 0; i < word.Length(); i++ )
		{
			int c = word[i].Hash();
			if ( c == 46 )
			{
				if ( decimal )
					return false;
				
				decimal = true;
			} else if ( !( c >= 48 && c <= 57 ) )
			{
				return false;
			}
		}

		return true;
	}

	float CompileEvaluate( ref map< string, float > variables )
	{
		if ( Compile( variables.GetKeyArray() ) )
			return Evaluate( variables.GetValueArray() );
		
		return 0;
	}

	float CompileEvaluateTest( ref map< string, float > variables, bool test )
	{
		if ( CompileTest( variables.GetKeyArray(), test ) )
			return EvaluateTest( variables.GetValueArray(), test );
		
		return 0;
	}
	
	bool Compile( ref array< string > variables )
	{
		_Compile( variables );
		
		return true;
	}
	
	bool CompileTest( ref array< string > variables, bool test )
	{
		int start = TickCount( 0 );
		
		bool success = Compile( variables );
		
		int time = TickCount( start );
		if ( test )
			Print( "Expression::Compile took " + time + " ticks to execute." ); 
		
		return success;
	}

	float Evaluate( ref array< float > variables )
	{
		_position = -1;
		
		return _Evaluate( variables );
	}

	float EvaluateTest( ref array< float > variables, bool test )
	{
		int start = TickCount( 0 );
		
		float num = Evaluate( variables );
		
		int time = TickCount( start );
		Print( "Expression::Evaluate took " + time + " ticks to execute." ); 
		
		return num;
	}

	private float _Evaluate( ref array< float > variables )
	{
		float stack[16];
		int stackPointer = 0;
		
		for ( int i = 0; i < _compiled.Count(); i++ )
		{
			ref ExpressionInstruction instruction = _compiled[i];
			if ( instruction.type == 2 )
			{
				_quickLookUp[instruction.token_i].Call( instruction, stack, stackPointer );		
			} else if ( instruction.type == 1 )
			{
				stack[++stackPointer] = variables[instruction.token_i];
			} else
			{
				stack[++stackPointer] = instruction.token_f;
			}
		}
		
		return stack[stackPointer];
	}
	
	private int _Compile( ref array< string > variables )
	{
		array< ref ExpressionCompileToken > dataStackStore();
		__Stack< ExpressionCompileToken > stack();
		
		_compiled.Clear();
		
		while ( !EOF() )
		{
			string token = GetToken();
			
			token = token.Trim();
			
			if ( token == "]" || token == "," )
				break;
						
			ExpressionFunction op1;
			ExpressionFunction op2;
			
			if ( _functions.Find( token, op1 ) )
			{
				while ( stack.Count() > 0 )
				{
					string tok = stack.Peek().token;
					if ( !_functions.Find( tok, op2 ) )
						break;
						
					int c = op1.precedence - op2.precedence;
										
					if ( c < 0 || ( !op1.associative && c <= 0 ) )
						_compiled.Insert( stack.Pop().ToOperation( 2, _functions[tok].index ) );
					else
						break;
				}
				
				ExpressionCompileToken ct = stack.Push( new ref ExpressionCompileToken( token ) );
				dataStackStore.Insert( ct );
				
				int startPosition = _position;
								
				token = SkipWhitespace();
				if ( token == "[" )
				{
					token = SkipWhitespace();
					_position--;
					while ( token != "]" )
					{
						Expression parameter = new Expression();
						
						parameter.value = value + "";
						parameter._position = _position + 0;

						//! there should be no variables/functions inside a parameter for a function call
						int nPos = parameter._Compile( variables );
						float nVal = parameter._Evaluate( new array<float>() );
						
						ct.parameters.Insert( nVal );

						_position = nPos - 1;
						token = SkipWhitespace();
					}
					
					//! factor function optimization
					if ( ct.token == "factor" )
					{
						if ( ct.parameters[1] < ct.parameters[0] )
						{
							float temp = ct.parameters[1];
							ct.parameters[1] = ct.parameters[0];
							ct.parameters[0] = temp;
						}
					}
				} else
				{
					_position = startPosition;
				}
			} else if ( token == "(" )
			{
				dataStackStore.Insert( stack.Push( new ExpressionCompileToken( token ) ) );
			} else if ( token == ")" )
			{
				string topToken = "";
                while ( stack.Count() > 0 )
				{
					ref ExpressionCompileToken top = stack.Pop();
					topToken = top.token;
					if ( top.token == "(" )
						break;
					
                    _compiled.Insert( top.ToOperation( 2, _functions[topToken].index ) );
                }
				
				if ( topToken != "(" )
					Error( "No matching left parenthesis" );
			} else
			{
				if ( IsNumber( token ) )
				{
					_compiled.Insert( new ExpressionInstruction( token, 0, null, -1 ) );
				} else
				{
					_compiled.Insert( new ExpressionInstruction( token, 1, null, variables.Find( token ) ) );
				}
			}
		}
		
		while ( stack.Count() > 0 )
		{
			ref ExpressionCompileToken o = stack.Pop();
            if ( !_functions.Contains( o.token ) )
				Error( "No matching right parenthesis" );
			
            _compiled.Insert( o.ToOperation( 2, _functions[o.token].index ) );
        }
		
		while ( dataStackStore.Count() > 0 )
		{
			delete dataStackStore[0];
			dataStackStore.Remove( 0 );
		}

		return _position;
	}
	
	string ToRPN()
	{
		return _ToRPN();
	}
	
	string ToRPN(bool test)
	{
		int start = TickCount( 0 );

		string rpn = _ToRPN();
		
		int time = TickCount( start );
		Print( "Expression::ToRPN took " + time + " ticks to execute." ); 
		
		return rpn;
	}
	
	// @brief	Converts the compiled expression into Reverse Polish Notation 
	// @return string
	private string _ToRPN()
	{
		string rpn = string.Empty;
		
		if ( _compiled.Count() == 0 )
			return "0"; //! edge case because I managed to accidentally optimize this out

		for ( int i = 0; i < _compiled.Count(); i++ )
		{
			if ( i > 0 )
				rpn = rpn + " ";

			rpn = rpn + _compiled[i].token;
			
			if ( _compiled[i].type == 2 )
			{
	           	ExpressionFunction function = _quickLookUp.Get( _compiled[i].token_i );
				
				if ( function.params != 0 )
				{
					rpn = rpn + " [";
					
					if ( function.params >= 4 )
					{
						rpn = rpn + _compiled[i].param1;
						rpn = rpn + ", ";
						rpn = rpn + _compiled[i].param2;
						rpn = rpn + ", ";
						rpn = rpn + _compiled[i].param3;
						rpn = rpn + ", ";
						rpn = rpn + _compiled[i].param4;
					} else if ( function.params >= 3 )
					{
						rpn = rpn + _compiled[i].param1;
						rpn = rpn + ", ";
						rpn = rpn + _compiled[i].param2;
						rpn = rpn + ", ";
						rpn = rpn + _compiled[i].param3;
					} else if ( function.params >= 2 )
					{
						rpn = rpn + _compiled[i].param1;
						rpn = rpn + ", ";
						rpn = rpn + _compiled[i].param2;
					} else if ( function.params >= 1 )
					{
						rpn = rpn + _compiled[i].param1;
					}
					
					rpn = rpn + "]";
				}
			}
		}

		return rpn;
	}
};
#endif