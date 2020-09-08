/**
 * ExpansionLight.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionLight extends ItemBase
{
	ref Timer m_DeleteTimer;
	
	void ExpansionLight()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLight::ExpansionLight - Start");
		#endif

		SetPilotLight( true );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLight::ExpansionLight - End");
		#endif
	}
	
	void SetExpansionLifetime(float lifetime)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLight::SetExpansionLifetime - Start");
		#endif

		m_DeleteTimer = new Timer( CALL_CATEGORY_SYSTEM );
		m_DeleteTimer.Run( lifetime , this, "OnDeleteThis", NULL, false);

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLight::SetExpansionLifetime - End");
		#endif
	}
	
	void OnDeleteThis()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLight::OnDeleteThis - Start");
		#endif

		GetGame().ObjectDelete(this);

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLight::OnDeleteThis - End");
		#endif
	}
}