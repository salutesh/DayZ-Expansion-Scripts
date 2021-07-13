class ExpansionVehicleSound
{
	private ExpansionVehicleBase m_Vehicle;

	private bool m_HasVolume = true;
	private bool m_HasFrequency = true;

	private ref Expression m_Volume;
	private ref Expression m_Frequency;
	
	private float m_TargetVolume;
	private float m_TargetFrequency;
	
	private float m_CurrentVolume;
	private float m_CurrentFrequency;

	private string m_SoundSetName;

	private ref SoundParams m_SoundParams;
	private ref SoundObjectBuilder m_SoundObjectBuilder;
	private ref SoundObject m_SoundObject;
	private ref AbstractWave m_SoundWaveObject;

	void ExpansionVehicleSound( ExpansionVehicleBase vehicle, string soundSetName, ref array< string > variables )
	{
		m_Vehicle = vehicle;
		m_SoundSetName = soundSetName;

		array< string > soundShaders = new array<string>;
		GetGame().ConfigGetTextArray( "CfgSoundSets " + soundSetName + " soundShaders", soundShaders );

		m_Volume = new Expression();
		m_Frequency = new Expression();

		string path;

		path = "CfgSoundShaders " + soundShaders[0] + " ex_volume";
		m_HasVolume = GetGame().ConfigIsExisting( path );
		if ( m_HasVolume )
		{
			GetGame().ConfigGetText( path, m_Volume.value );

			m_Volume.Compile( variables );
		}
			
		path = "CfgSoundShaders " + soundShaders[0] + " ex_frequency";
		m_HasFrequency = GetGame().ConfigIsExisting( path );
		if ( m_HasFrequency )
		{
			GetGame().ConfigGetText( path, m_Frequency.value );

			m_Frequency.Compile( variables );
		}
	}

	bool Update( float pDt, ref array< float > variables )
	{
		if ( !m_SoundWaveObject )
		{
			m_SoundParams = new SoundParams( m_SoundSetName );
			if ( !m_SoundParams.IsValid() )
			{
				//Print( "[Error][Sound]: ExpansionVehicleSound::OnUpdate() -> m_SoundObject is null -> m_SoundSetName: " + m_SoundSetName );

				return false;
			}
			
			m_SoundObjectBuilder = new SoundObjectBuilder( m_SoundParams );
			m_SoundObjectBuilder.UpdateEnvSoundControllers( m_Vehicle.GetPosition() );
			
			m_SoundObject = m_SoundObjectBuilder.BuildSoundObject();
			
			if ( m_SoundObject )
			{
				m_SoundObject.SetPosition( m_Vehicle.GetPosition() );
				m_SoundObject.SetKind( WaveKind.WAVEATTALWAYS );
			} else
			{
				//Print( "[Error][Sound]: ExpansionVehicleSound::OnUpdate() -> m_SoundObject is null -> m_SoundSetName: " + m_SoundSetName );

				return false;
			}

			m_SoundWaveObject = GetGame().GetSoundScene().Play3D( m_SoundObject, m_SoundObjectBuilder );
			m_SoundWaveObject.Play();
			m_SoundWaveObject.Loop( true );
		}

		m_SoundWaveObject.SetPosition( m_Vehicle.GetPosition() );

		if ( m_HasFrequency )
		{
			m_TargetFrequency = m_Frequency.Evaluate( variables );
		} else
		{
			m_TargetFrequency = 1.0;
			m_CurrentFrequency = 1.0;
		}
		
		if ( m_HasVolume )
		{
			if ( m_TargetFrequency < 0.5 || m_TargetFrequency > 1.75 )
			{
				m_TargetVolume = 0.0;
			} else
			{
				m_TargetVolume = m_Volume.Evaluate( variables );
			}
		} else
		{
			m_TargetVolume = 1.0;
		}
		
		m_SoundWaveObject.SetVolume( m_CurrentVolume );
		m_SoundWaveObject.SetFrequency( m_CurrentFrequency );
				
		#ifdef EXPANSION_DEBUG_UI_VEHICLE
		CF_Debugger_Block dbg_Vehicle = CF.Debugger.Get("Vehicle", m_Vehicle);

		dbg_Vehicle.Set("Sound Set", m_SoundSetName );
		dbg_Vehicle.Set("Target", "(volume=" + m_TargetVolume + ", frequency=" + m_TargetFrequency + ")" );
		dbg_Vehicle.Set("Current", "(volume=" + m_CurrentVolume + ", frequency=" + m_CurrentFrequency + ")" );
		#endif
		
		m_CurrentVolume += Math.Clamp( m_TargetVolume - m_CurrentVolume, -1.0 * pDt, 1.0 * pDt );
		m_CurrentFrequency += Math.Clamp( m_TargetFrequency - m_CurrentFrequency, -1.0 * pDt, 1.0 * pDt );
		
		return true;
	}
};