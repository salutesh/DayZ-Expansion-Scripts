class ExpansionMusicKey
{
	private ref EffectSound m_Audio;

	private ExpansionMusic m_Item;
	private string m_Note;

	void ExpansionMusicKey( ExpansionMusic item, string note )
	{
		m_Item = item;
		m_Note = note;

		string sndClass = "Expansion_Instrument_" + m_Item.GetSoundType() + "_" + m_Note + "_SoundSet";
		//Print( sndClass );
		
		m_Audio = SEffectManager.PlaySound( sndClass, m_Item.GetPosition() );
		m_Audio.SetSoundAutodestroy( true );
		m_Audio.SetSoundFadeOut( 0.05 );
		m_Audio.SetSoundLoop( false );
	}

	void ~ExpansionMusicKey()
	{
		if ( m_Audio )
			delete m_Audio;
	}

	void Stop()
	{
		if ( m_Audio )
			m_Audio.SoundStop();
	}

	EffectSound GetAudio()
	{
		return m_Audio;
	}

	string GetNote()
	{
		return m_Note;
	}
};

class ExpansionNote
{
	string key;
	int track_id;
	int midi_start;
	int midi_length;

	[NonSerialized()]
	private ExpansionMusic m_Item;

	void Start( ExpansionMusic item, int trackid, inout array< ExpansionNote > notes )
	{
		if (trackid != -1 && trackid != track_id)
			return;
		
		m_Item = item;
		
		notes.Insert( this );
		
		m_Item.PressKey( key );
	}

	bool OnUpdate( float pDt, int pTime )
	{
		if ( midi_start + midi_length >= pTime )
		{
			End();
			return true;
		}

		return false;
	}

	void End()
	{
		m_Item.ReleaseKey( key );
	}
}

class ExpansionMusic extends ItemBase
{
	private ref map< string, ExpansionMusicKey > m_KeyPressMap;
	private ref array< ref ExpansionMusicKey > m_KeyPress;

	private ref map< int, string > m_MidiToKey;

	void ExpansionMusic()
	{
		m_KeyPressMap = new map< string, ExpansionMusicKey >();
		m_KeyPress = new array< ref ExpansionMusicKey >();
		m_MidiToKey = new map< int, string >();

		array< string > notes = { "E", "F", "F_S", "G", "G_S", "C", "C_S", "D", "D_S", "A", "A_S", "B" };

		for ( int j = 0; j < 12; ++j )
		{
			for ( int i = 0; i < notes.Count(); ++i )
			{
				m_MidiToKey.Insert( i + ( j * 11 ), notes[i] + "_" + j );
				m_KeyPressMap.Insert( notes[i] + "_" + j, NULL );
			}
		}
	}

	void ~ExpansionMusic()
	{
		delete m_KeyPress;
	}

	string GetSoundType()
	{
		return "Piano";
	}

	string GetKeyFromMIDI( int midi )
	{
		return m_MidiToKey.Get( midi );
	}

	void ReleaseAllKeys()
	{
		for ( int i = 0; i < m_KeyPressMap.Count(); ++i )
		{
			ref ExpansionMusicKey note = m_KeyPressMap.GetElement( i );

			if ( note )
			{
				ReleaseKey( note.GetNote() );
			}
		}
	}

	void ReleaseKey( string key, int level )
	{
		ReleaseKey( key + "_" + level );
	}

	void ReleaseKey( string note )
	{
		ExpansionMusicKey key = m_KeyPressMap.Get( note );
		if ( key )
		{
			key.Stop();
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( RemoveKey, 50, false, key );
		}

		m_KeyPressMap.Set( note, NULL );
	}

	private void RemoveKey( ref ExpansionMusicKey key )
	{
		int noteSoundIdx = m_KeyPress.Find( key );
		if ( noteSoundIdx != -1 )
			m_KeyPress.Remove( noteSoundIdx );
	}

	ExpansionMusicKey PressKey( string key, int level )
	{
		return PressKey( key + "_" + level );
	}

	ExpansionMusicKey PressKey( string key )
	{
		if ( m_KeyPressMap.Contains( key ) )
		{
			ReleaseKey( key );
		}

		ref ExpansionMusicKey keyData = new ExpansionMusicKey( this, key );

		m_KeyPressMap.Set( key, keyData );
		m_KeyPress.Insert( keyData );

		return keyData;
	}
};

class Expansion_Guitar extends ExpansionMusic
{
	override bool IsOpen()
	{
		return false;
	}
	
	void AnimateGuitar( bool state )
	{
		SetAnimationPhase( "AnimateGuitar", state );
	}

	override void SetActions()
	{
		super.SetActions();
	}
};