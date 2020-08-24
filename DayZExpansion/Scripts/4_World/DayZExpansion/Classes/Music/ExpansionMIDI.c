class ExpansionMIDI
{
	private ref map< int, ExpansionNote > m_LastNote; //! file loading
	private ref map< int, string > m_MidiToKey;

	private ref array< ref ExpansionNote > m_AllNotes;
	private ref map< int, ref array< ExpansionNote > > m_NotesTimeMapped;
	private ref array< ExpansionNote > m_PlayingNotes;

	private float m_PreviousTime;
	private float m_CurrentTime;
	
	private float m_Tempo;

	void ExpansionMIDI()
	{
		m_LastNote = new map< int, ExpansionNote >();
		m_MidiToKey = new map< int, string >();

		m_AllNotes = new array< ref ExpansionNote >();
		m_NotesTimeMapped = new map< int, ref array< ExpansionNote > >();
		m_PlayingNotes = new array< ExpansionNote >();

		array< string > notes = { "G", "G_S", "A", "A_S", "B", "C", "C_S", "D", "D_S", "E", "F", "F_S" };

		for ( int j = 1; j < 12; ++j )
		{
			for ( int i = 0; i < notes.Count(); ++i )
			{
				int idx = i + ( j * 11 );
				m_MidiToKey.Insert( idx, notes[i] + "_" + (j - 1) );
				//Print( "[" + idx + "] " + notes[i] + "_" + (j - 1) );
				m_LastNote.Insert( idx, NULL );
			}
		}
	}

	ExpansionNote InsertKey( int trackid, int time, int key )
	{
		ExpansionNote note = new ExpansionNote();
		note.key = m_MidiToKey.Get( key );
		note.midi_start = time;
		note.track_id = trackid;
		m_AllNotes.Insert( note );

		array< ExpansionNote > notes = m_NotesTimeMapped.Get( time );
		if ( !notes )
		{
			notes = new array< ExpansionNote >;
			m_NotesTimeMapped.Insert( time, notes );
		}

		notes.Insert( note );

		m_LastNote.Set( key, note );
		
		return note;
	}

	void FinishKey( int trackid, int time, int key )
	{
		ExpansionNote note = m_LastNote.Get( key );
		if (note.track_id != trackid)
			return;
		
		note.midi_length = time - note.midi_start;
	}

	bool Read( string file_path )
	{
		FileHandle file_handle = OpenFile( file_path, FileMode.READ );
		Print( file_handle );
		Print( file_path );
				
		if ( file_handle )
		{
			string line_content;
			while ( FGets( file_handle, line_content ) >= 0 )
			{
				if ( !ParseLine( line_content ) )
					return false;
			}

			CloseFile( file_handle );

			return true;
		}

		return false;
	}

	private bool ParseLine( string line )
	{
		array<string> tokens();
		line.Split( ", ", tokens );

		int trackId = tokens[0].ToInt();
		int time = tokens[1].ToInt();
		string control = tokens[2].Trim();

		int key;
		
		switch ( control )
		{
			case "Note_on_c":
				key = tokens[4].ToInt();
				InsertKey( trackId, time, key );
				return true;
			case "Note_off_c":
				key = tokens[4].ToInt();
				FinishKey( trackId, time, key );
				return true;
			case "Tempo":
				m_Tempo = tokens[3].ToInt();
				return true;
		}

		return true;
	}

	void OnUpdate( ExpansionMusic item, float pDt )
	{
		int start = Math.Floor( m_PreviousTime );
		int end = Math.Floor( m_CurrentTime );

		for ( int idx = start; idx < end; ++idx )
		{
			array< ExpansionNote > notes = m_NotesTimeMapped.Get( idx );
			if ( notes )
				for ( int j = 0; j < notes.Count(); ++j )
				{
					notes[j].Start( item, 2, m_PlayingNotes );
				}
		}

		for ( int i = m_PlayingNotes.Count() - 1; i >= 0; --i )
		{
			if ( m_PlayingNotes[i].OnUpdate( pDt, m_CurrentTime ) )
			{
				m_PlayingNotes.Remove( i );
			}
		}

		m_PreviousTime = m_CurrentTime;
		m_CurrentTime += m_Tempo * pDt * 0.001;
	}
};