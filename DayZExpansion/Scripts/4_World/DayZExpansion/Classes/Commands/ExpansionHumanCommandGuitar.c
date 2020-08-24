class ExpansionHumanCommandGuitar extends HumanCommandScript
{
	private DayZPlayerImplement m_Player;
	private Expansion_Guitar m_Guitar;
	private ExpansionHumanST m_Table;

	private bool m_ShouldFinish;

	private ref map< string, int > m_NoteAnimMap;
	private ref array< string > m_PossibleNotes;

	private ref ExpansionMIDI m_MIDI;

	void ExpansionHumanCommandGuitar( Human pHuman, Expansion_Guitar guitar, ExpansionHumanST table )
	{
		Class.CastTo( m_Player, pHuman );

		m_Guitar = guitar;
		m_Table = table;

		m_NoteAnimMap = new map< string, int >();
		m_PossibleNotes = new array< string >();

		m_MIDI = new ExpansionMIDI();
	}

	void ~ExpansionHumanCommandGuitar()
	{
		delete m_NoteAnimMap;
		delete m_PossibleNotes;
	}

	void SetNoteAnim( string note, int level, int animIdx )
	{
		m_PossibleNotes.Insert( note + "_" + level );
		m_NoteAnimMap.Set( note + "_" + level, animIdx );
	}

	int GetNoteAnim( string note, int level )
	{
		return GetNoteAnim( note + "_" + level );
	}

	int GetNoteAnim( string key )
	{
		if ( m_NoteAnimMap.Contains( key ) )
			return m_NoteAnimMap.Get( key );

		return 0;
	}

	override void OnActivate()
	{
		if ( m_MIDI )
			delete m_MIDI;
		
		m_MIDI = new ExpansionMIDI();
		m_MIDI.Read( "$profile:ExpansionMod/MIDI/SMASH MOUTH.All star.csv" );
		Print(m_MIDI);

		m_Table.CallGuitar( this );

		SetNoteAnim( "A", 2, 1 );
		SetNoteAnim( "A", 3, 1 );
		SetNoteAnim( "A", 4, 1 );
		SetNoteAnim( "A_S", 2, 1 );
		SetNoteAnim( "A_S", 3, 1 );
		SetNoteAnim( "A_S", 4, 1 );
		SetNoteAnim( "B", 2, 1 );
		SetNoteAnim( "B", 3, 1 );
		SetNoteAnim( "B", 4, 1 );
		SetNoteAnim( "C", 3, 1 );
		SetNoteAnim( "C", 4, 1 );
		SetNoteAnim( "C", 5, 1 );
		SetNoteAnim( "C_S", 3, 1 );
		SetNoteAnim( "C_S", 4, 1 );
		SetNoteAnim( "C_S", 5, 1 );
		SetNoteAnim( "D", 3, 1 );
		SetNoteAnim( "D", 4, 1 );
		SetNoteAnim( "D", 5, 1 );
		SetNoteAnim( "D_S", 3, 1 );
		SetNoteAnim( "D_S", 4, 1 );
		SetNoteAnim( "D_S", 5, 1 );
		SetNoteAnim( "E", 2, 1 );
		SetNoteAnim( "E", 3, 1 );
		SetNoteAnim( "E", 4, 1 );
		SetNoteAnim( "E", 5, 1 );
		SetNoteAnim( "F", 2, 1 );
		SetNoteAnim( "F", 3, 1 );
		SetNoteAnim( "F", 4, 1 );
		SetNoteAnim( "F_S", 2, 1 );
		SetNoteAnim( "F_S", 3, 1 );
		SetNoteAnim( "F_S", 4, 1 );
		SetNoteAnim( "G", 2, 1 );
		SetNoteAnim( "G", 3, 1 );
		SetNoteAnim( "G", 4, 1 );
		SetNoteAnim( "G_S", 2, 1 );
		SetNoteAnim( "G_S", 3, 1 );
		SetNoteAnim( "G_S", 4, 1 );
	}

	override void OnDeactivate()
	{
		m_Table.CallGuitarStop( this );
	}

	override void PreAnimUpdate( float pDt )
	{
		m_MIDI.OnUpdate( m_Guitar, pDt );
	}

	override void PrePhysUpdate( float pDt )
	{
		PrePhys_SetTranslation( vector.Zero );
	}

	override bool PostPhysUpdate( float pDt )
	{
		return m_ShouldFinish == false;
	}

	void Stop()
	{
		m_ShouldFinish = true;
	}
};