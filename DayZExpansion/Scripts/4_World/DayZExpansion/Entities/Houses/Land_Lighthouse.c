/**
 * Land_Lighthouse.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Land_Lighthouse extends House
{
	//! Expansion reference to point light class
	protected ScriptedLightBase m_Light;

	protected bool m_CanBeEnabled; // setting
	protected bool m_ShouldBeEnabled; // setting
	protected bool m_CastShadows; // setting

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void Land_Lighthouse()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.LIGHTHOUSE, this, "Land_Lighthouse");
#endif

		ExpansionSettings.SI_General.Insert( OnSettingsUpdated );

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( OnSettingsUpdated, 1000 );
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~Land_Lighthouse()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.LIGHTHOUSE, this, "~Land_Lighthouse");
#endif

		if ( GetGame() && m_Light )
		{
			GetGame().ObjectDelete( m_Light );
		}

		ExpansionSettings.SI_General.Remove( OnSettingsUpdated );
	}
	
	// ------------------------------------------------------------
	// OnSettingsUpdated
	// ------------------------------------------------------------
	override void OnSettingsUpdated()	
	{	
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.LIGHTHOUSE, this, "OnSettingsUpdated");
#endif

		if ( IsMissionClient() )
		{
			if ( GetExpansionSettings().GetGeneral() && GetExpansionClientSettings().CastLightShadows )
			{
				m_CastShadows = true;
			}
			else
			{
				m_CastShadows = false;
			}
			
			if ( GetExpansionSettings().GetGeneral() && GetExpansionSettings().GetGeneral().EnableLighthouses )
			{
				m_ShouldBeEnabled = true;
				m_CanBeEnabled = true;
				
				OnEnable();
			}
			else
			{
				m_CanBeEnabled = false;
			}
	
			if ( m_ShouldBeEnabled && m_CanBeEnabled )
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
		return GetMemoryPointPos( "cerveny pozicni blik" );
	}

	// ------------------------------------------------------------
	// Expansion Enable
	// ------------------------------------------------------------
	void Enable( vector position )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.LIGHTHOUSE, this, "Enable").Add(position);
#endif

		m_ShouldBeEnabled = true;
		
		OnEnable();
	}

	protected void OnEnable()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.LIGHTHOUSE, this, "OnEnable");
#endif

		if ( IsMissionClient() )
		{
			if ( !IsEnabled() && m_CanBeEnabled )
			{
				if ( !m_Light )
				{
					m_Light = ScriptedLightBase.CreateLight( ExpansionLighthouse, "0 0 0" );
					//vector colorRGB = Vector( 1.0, 1.0, 0.8 );
					//m_Light.SetDiffuseColor( colorRGB[0], colorRGB[1], colorRGB[2] );
					//m_Light.SetAmbientColor( colorRGB[0], colorRGB[1], colorRGB[2] );
					m_Light.SetRadiusTo( 500 );
					m_Light.SetBrightnessTo( 0.8 );
					m_Light.FadeIn(0.3);
					
					if ( m_CastShadows )
					{
						m_Light.SetCastShadow(true);
						m_Light.SetFlareVisible(true);
					}
					else
					{
						m_Light.SetCastShadow(false);
						m_Light.SetFlareVisible(false);
					}
	
					m_Light.AttachOnObject( this, GetLampPosition(), "0 0 0" );
	
					m_Light.SetEnabled( true );
				}
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion Disable
	// ------------------------------------------------------------
	void Disable( vector position )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.LIGHTHOUSE, this, "Disable").Add(position);
#endif

		m_ShouldBeEnabled = false;

		OnDisable();
	}

	protected void OnDisable()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.LIGHTHOUSE, this, "OnDisable");
#endif

		if ( IsMissionClient() )
		{
			if ( m_Light )
			{
				m_Light.FadeOut(0.3);
				m_Light.SetEnabled( false );
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion IsEnabled
	// ------------------------------------------------------------
	bool IsEnabled()
	{
		if ( m_Light && m_Light.IsEnabled() )
			return true;

		return false;
	}

	bool CanBeEnabled()
	{
		return m_CanBeEnabled;
	}	
};
