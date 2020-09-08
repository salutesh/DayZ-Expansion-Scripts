/**
 * ExpansionBinaryWriter.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

// Does not support floats
class ExpansionBinaryWriter : ExpansionBinary
{
	private ref array< int > m_data = new array< int >();
	private int m_remaining_block = 4;
	private int m_to_flush = 0;
	private bool m_has_been_flushed = false;

	void ExpansionBinaryWriter( ref ParamsWriteContext src )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBinaryWriter::ExpansionBinaryWriter - Start");
		#endif
		
		m_src = src;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBinaryWriter::ExpansionBinaryWriter - End");
		#endif
	}

	void WriteByte( int b )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBinaryWriter::WriteByte - Start");
		#endif
		
		switch ( m_remaining_block )
		{
			case 4:
				m_to_flush = m_to_flush | ( b << 24 );
				break;
			case 3:
				m_to_flush = m_to_flush | ( b << 16 );
				break;
			case 2:
				m_to_flush = m_to_flush | ( b << 8 );
				break;
			case 1:
				m_to_flush = m_to_flush | ( b );
				break;
		}

		FinishWriteOperation();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBinaryWriter::WriteByte - End");
		#endif
	}

	void WriteInt( int i )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBinaryWriter::WriteInt - Start");
		#endif
		
		if ( m_remaining_block == 4 )
		{
			m_data.Insert( i );

			return;
		}
		
		int b1 = (i >> 24) & 0xFF;
		int b2 = (i >> 16) & 0xFF;
		int b3 = (i >> 8) & 0xFF;
		int b4 = (i) & 0xFF;

		switch ( m_remaining_block )
		{
			case 3:
				m_to_flush = m_to_flush | ( b1 << 16 );
				m_to_flush = m_to_flush | ( b2 << 8 );
				m_to_flush = m_to_flush | ( b3 );

				FinishWriteOperation();

				m_to_flush = m_to_flush | ( b4 << 24 );
				return;
			case 2:
				m_to_flush = m_to_flush | ( b1 << 8 );
				m_to_flush = m_to_flush | ( b2 );

				FinishWriteOperation();

				m_to_flush = m_to_flush | ( b3 << 24 );
				m_to_flush = m_to_flush | ( b4 << 16 );
				return;
			case 1:
				m_to_flush = m_to_flush | ( b1 );

				FinishWriteOperation();

				m_to_flush = m_to_flush | ( b2 << 24 );
				m_to_flush = m_to_flush | ( b3 << 16 );
				m_to_flush = m_to_flush | ( b4 << 8 );
				return;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBinaryWriter::WriteInt - End");
		#endif
	}

	private void FinishWriteOperation()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBinaryWriter::FinishWriteOperation - Start");
		#endif
		
		if ( --m_remaining_block > 0 )
			return;

		m_data.Insert( m_to_flush );

		m_remaining_block = 4;
		m_to_flush = 0;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBinaryWriter::FinishWriteOperation - End");
		#endif
	}

	void Flush()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBinaryWriter::Flush - Start");
		#endif
		
		m_has_been_flushed = true;

		if ( m_remaining_block > 0 && m_remaining_block != 4 )
		{
			m_data.Insert( m_to_flush );
		}

		m_src.Write( m_data.Count() );
		for ( int i = 0; i < m_data.Count(); i++ )
		{
			m_src.Write( m_data[i] );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBinaryWriter::Flush - End");
		#endif
	}
}