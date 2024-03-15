/**
 * ExpansionKit.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionKitBase
 * @brief		
 **/

class ExpansionKitBase: WatchtowerKit
{	
	private int m_Expansion_PlacingTypeChosen;
	private autoptr array< string > m_Expansion_PlacingTypes;
	private string m_Expansion_ToDeploy;
	
	// ------------------------------------------------------------
	void ExpansionKitBase()
	{
		m_Expansion_PlacingTypes = new array< string >;
		
		string path = "CfgVehicles " + GetType() + " placingTypes";
		if ( GetGame().ConfigIsExisting( path ) )
			GetGame().ConfigGetTextArray( path, m_Expansion_PlacingTypes );
	}
	
	// ------------------------------------------------------------
	void SetPlacingIndex( int placingTypeIdx )
	{
		m_Expansion_ToDeploy = GetType();
		m_Expansion_PlacingTypeChosen = placingTypeIdx;
		
		string path = "CfgVehicles " + m_Expansion_PlacingTypes[m_Expansion_PlacingTypeChosen] + " deployType";
		if ( !GetGame().ConfigIsExisting( path ) )
			return;
		
		m_Expansion_ToDeploy = GetGame().ConfigGetTextOut( path );
	}
	
	// ------------------------------------------------------------
	array< string > GetPlacingTypes()
	{
		return m_Expansion_PlacingTypes;
	}

	// ------------------------------------------------------------
	string GetDeployType()
	{
		return m_Expansion_ToDeploy;
	}

	// ------------------------------------------------------------
	int GetPlacingTypeChosen()
	{
		return m_Expansion_PlacingTypeChosen;
	}

	override void DisassembleKit(ItemBase item)
	{
	}

	Object ExpansionDeploy( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
	{
		if ( !IsMissionHost() )
			return NULL;

		Object obj = GetGame().CreateObject( GetDeployType(), GetPosition() );
		obj.SetPosition( position );
		obj.SetOrientation( orientation );

		ExpansionBaseBuilding bb;
		if (Class.CastTo(bb, obj))
		{
			//! Hologram. Set max lifetime to that of the kit
			bb.SetLifetimeMax(GetLifetimeMax());
			bb.MaxLifetimeRefreshCalc();
		}
		
		//! Make the kit invisible, so it can be destroyed from deploy UA when action ends
		HideAllSelections();
		
		SetIsDeploySound( true );

		return obj;
	}
}

/**@class		ExpansionKitSmall
 * @brief		
 **/
class ExpansionKitSmall: ExpansionKitBase {};

/**@class		ExpansionKitLarge
 * @brief		
 **/
class ExpansionKitLarge: ExpansionKitBase {};
