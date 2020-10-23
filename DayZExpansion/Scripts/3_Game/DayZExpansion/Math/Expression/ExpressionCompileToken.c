/* TODO: Move to Community Framework */
#ifndef CF_COT_MOVE
class ExpressionCompileToken
{
	string token;
	ref array< float > parameters;
	
	void ExpressionCompileToken( string _token )
	{
		token = _token;
		parameters = new array< float >();
	}

	ref ExpressionInstruction ToOperation( int type, int token_i = -1 )
	{
		ref ExpressionInstruction op = new ExpressionInstruction( token, type, parameters, token_i );
		return op;
	}
};
#endif