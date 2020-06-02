/**
 * ExpansionLampLightBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionLampLightBase
 * @brief		
 **/
class ExpansionLampLightBase: Static
{
	//! Expansion reference to point light class
	protected ScriptedLightBase m_Light;

	protected bool m_CanBeEnabled; //! Setting
	protected bool m_ShouldBeEnabled; //! Setting
	protected bool m_CastShadow; //! Setting
	
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionLampLightBase()
	{
		if ( IsMissionClient() )
		{
			ExpansionSettings.SI_General.Insert( OnSettingsUpdated );
			GetExpansionClientSettings().SI_UpdateSetting.Insert( OnSettingsUpdated );
			ExpansionWorldMappingModule.SI_LampEnable.Insert( Enable );
			ExpansionWorldMappingModule.SI_LampDisable.Insert( Disable );
		}
		
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionLampLightBase()
	{
		if ( IsMissionClient() )
		{
			ExpansionSettings.SI_General.Remove( OnSettingsUpdated );
			GetExpansionClientSettings().SI_UpdateSetting.Remove( OnSettingsUpdated );
			ExpansionWorldMappingModule.SI_LampEnable.Remove( Enable );
			ExpansionWorldMappingModule.SI_LampDisable.Remove( Disable );
		}
	}
	
	// ------------------------------------------------------------
	// OnSettingsUpdated
	// ------------------------------------------------------------
	void OnSettingsUpdated()
	{
		if ( IsMissionClient() )
		{
			m_CastShadow = GetExpansionClientSettings().CastLightShadows;
			
			m_CanBeEnabled = (GetExpansionSettings().GetGeneral() && GetExpansionSettings().GetGeneral().EnableLamps > 0);
	
			if ( m_CanBeEnabled )
			{
				OnEnable();
			} 
			else
			{
				OnDisable();
			}
		}
	}

	// ------------------------------------------------------------
	// GetLampPosition
	// ------------------------------------------------------------
	vector GetLampPosition()
	{
		return "0 5 0";
	}

	// ------------------------------------------------------------
	// GetLampColor
	// ------------------------------------------------------------
	vector GetLampColor()
	{
		return "1.0 0.7 0.4";
	}

	protected bool LampInGenerator( vector generator )
	{
		float R = 500;

		float dx = Math.AbsFloat( GetPosition()[0] - generator[0] );
		float dz = Math.AbsFloat( GetPosition()[2] - generator[2] );

		if ( dx > R )
			return false;

		if ( dz > R )
			return false;

		float Rs = R * R;
		float dxs = dx * dx;
		float dzs = dz * dz;
		float ab = dxs + dzs;

		if ( ab <= Rs )
		{
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Expansion Enable
	// ------------------------------------------------------------
	void Enable( vector position )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionLampLightBase::Enable - Start - " + position );
		#endif

		if ( !LampInGenerator( position ) && GetExpansionSettings().GetGeneral().EnableLamps != 3 )
		{
			return;
		}

		m_ShouldBeEnabled = true;
		
		OnEnable();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionLampLightBase::Enable - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnEnable
	// ------------------------------------------------------------
	protected void OnEnable()
	{
		if ( IsMissionClient() )
		{
			if ( !IsEnabled() && m_CanBeEnabled )
			{
				if ( !m_Light )
				{
					m_Light = ScriptedLightBase.CreateLight( ExpansionStreetLight, "0 0 0" );

					m_Light.SetDiffuseColor( GetLampColor()[0], GetLampColor()[1], GetLampColor()[2] );
					m_Light.SetAmbientColor( GetLampColor()[0], GetLampColor()[1], GetLampColor()[2] );
					//! m_Light.SetRadiusTo( 10 );
					//! m_Light.SetBrightnessTo( 5 );

					m_Light.FadeIn(0.3);
					
					m_Light.SetCastShadow(m_CastShadow);
					
					m_Light.SetFlareVisible(true);

					if ( MemoryPointExists( "light" ) )
					{	
						ExpansionAttachOnMemoryPoint( "light", "0 -1 0" ); 

						//! m_Light.AttachOnMemoryPoint( this, "light" );
					} 
					else 
					{
						ExpansionAttachOnObject( GetLampPosition(), "0 -1 0" ); 

						//! m_Light.AttachOnObject( this, GetLampPosition(), "0 0 0" );
					}
	
					m_Light.SetEnabled( true );
				}
			}
			else if ( IsEnabled() )
			{
				m_Light.SetCastShadow( m_CastShadow );
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion Disable
	// ------------------------------------------------------------
	void ExpansionAttachOnMemoryPoint(string memory_point_start, vector memory_point_target = "0 0 0")
	{
		vector m_LocalPos = this.GetMemoryPointPos(memory_point_start);
		vector local_ori;
		if (memory_point_target != vector.Zero)
		{
			vector target_pos = vector.Direction(m_LocalPos, memory_point_target);
			local_ori = target_pos.VectorToAngles();
		}
		
		m_Light.AttachOnObject(this, m_LocalPos, local_ori);
	}

	// ------------------------------------------------------------
	// Expansion Disable
	// ------------------------------------------------------------
	void ExpansionAttachOnObject(vector memory_point_start, vector memory_point_target = "0 0 0")
	{
		vector m_LocalPos = memory_point_start;
		vector local_ori;
		if (memory_point_target != vector.Zero)
		{
			vector target_pos = vector.Direction(m_LocalPos, memory_point_target);
			local_ori = target_pos.VectorToAngles();
		}
		
		m_Light.AttachOnObject(this, m_LocalPos, local_ori);
	}

	// ------------------------------------------------------------
	// Expansion Disable
	// ------------------------------------------------------------
	void Disable( vector position )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionLampLightBase::Disable - Start - " + position );
		#endif

		if ( !LampInGenerator( position ) && GetExpansionSettings().GetGeneral().EnableLamps != 3 )
		{
			return;
		}

		m_ShouldBeEnabled = false;

		OnDisable();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionLampLightBase::Disable - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnDisable
	// ------------------------------------------------------------
	protected void OnDisable()
	{
		if ( IsMissionClient() )
		{
			if ( m_Light )
			{
				m_Light.Destroy();
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion IsEnabled
	// ------------------------------------------------------------
	bool IsEnabled()
	{
		return (m_Light && m_Light.IsEnabled());
	}
	
	// ------------------------------------------------------------
	// Expansion CanBeEnabled
	// ------------------------------------------------------------
	bool CanBeEnabled()
	{
		return m_CanBeEnabled;
	}	
}