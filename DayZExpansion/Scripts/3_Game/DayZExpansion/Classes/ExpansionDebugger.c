/**
 * ExpansionDebugger.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

typedef int ExpansionDebuggerType;

class ExpansionDebuggerBlock
{
	ExpansionDebuggerType _type;
	ref array< string > _text;

	ref array< string > _readyBuffer;

	void ExpansionDebuggerBlock( ExpansionDebuggerType type )
	{
		_type = type;
		_text = new array< string >;

		_readyBuffer = new array< string >;
	}
}

class ExpansionDebugger
{
	// () -> ( ExpansionDebuggerType type, array< string > text )
	static ref ScriptInvoker OnPush = new ScriptInvoker();

	static bool GlobalEnable = false;
	static bool Printing = false;
	
	private static ref array< ref ExpansionDebuggerBlock > _blocks = new array< ref ExpansionDebuggerBlock >;

	private static ref map< ExpansionDebuggerType, ExpansionDebuggerBlock > _enabledBlocks = new map< ExpansionDebuggerType, ExpansionDebuggerBlock >;

	private static ExpansionDebuggerType _lastBlockType = -1;
	private static ExpansionDebuggerBlock _lastBlock = NULL;

	static void AddType( ExpansionDebuggerType type )
	{
		for ( int i = 0; i < _blocks.Count(); ++i )
		{
			if ( _blocks[i]._type == type )
			{
				Error( "Duplicate type added to the Expansion Debugger!" );
				return;
			}
		}

		_blocks.Insert( new ExpansionDebuggerBlock( type ) );
	}

	static void GetEnabledBlocks( out array< ExpansionDebuggerBlock > blocks )
	{
		for ( int i = 0; i < _enabledBlocks.Count(); i++ )
		{
			blocks.Insert( _enabledBlocks.GetElement( i ) );
		}
	}

	static void Enable( ExpansionDebuggerType type )
	{
		if ( type < 0 )
			return;

		if ( _enabledBlocks.Find( type, _lastBlock ) )
			return;

		for ( int i = 0; i < _blocks.Count(); ++i )
		{
			if ( _blocks[i]._type == type )
			{
				_lastBlockType = type;
				_lastBlock = _blocks[i];

				_enabledBlocks.Insert( type, _blocks[i] );
				break;
			}
		}
	}

	static void Disable( ExpansionDebuggerType type )
	{
		_enabledBlocks.Remove( type );

		if ( _lastBlockType == type )
		{
			_lastBlockType = -1;
			_lastBlock = NULL;
		}
	}

	static void Display( int type, string text )
	{
		if ( !GlobalEnable )
			return;

		if ( type < 0 )
			return;

		if ( type != _lastBlockType && _enabledBlocks.Contains( type ) )
		{
			_lastBlock = _enabledBlocks.Get( type );
			_lastBlockType = type;
		}

		if ( type == _lastBlockType )
		{
			if ( Printing )
				Print( "Display[" + type + "]: " + text );

			_lastBlock._text.Insert( text );
		}
	}
	
	static void Push( int type )
	{
		if ( !GlobalEnable )
			return;
			
		if ( type < 0 )
			return;
			
		if ( _enabledBlocks.Contains( type ) )
		{
			if ( Printing )
				Print( "Push: " + type );

			ExpansionDebuggerBlock block = _enabledBlocks.Get( type );
			block._readyBuffer.Clear();
			block._readyBuffer.Copy( block._text );
			block._text.Clear();
		}
	}
}