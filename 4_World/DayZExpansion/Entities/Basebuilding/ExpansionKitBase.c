/**
 * ExpansionKit.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionKitBase
 * @brief		
 **/

class ExpansionKitBase extends WatchtowerKit
{	
	private int m_PlacingTypeChosen;
	private autoptr array< string > m_PlacingTypes;
	private string m_ToDeploy;
	
	// ------------------------------------------------------------
	void ExpansionKitBase()
	{
		m_PlacingTypes = new array< string >;
		
		string path = "CfgVehicles " + GetType() + " placingTypes";
		if ( GetGame().ConfigIsExisting( path ) )
			GetGame().ConfigGetTextArray( path, m_PlacingTypes );
	}
	
	// ------------------------------------------------------------
	void SetPlacingIndex( int placingTypeIdx )
	{
		m_ToDeploy = GetType();
		m_PlacingTypeChosen = placingTypeIdx;
		
		string path = "CfgVehicles " + m_PlacingTypes[m_PlacingTypeChosen] + " deployType";
		if ( !GetGame().ConfigIsExisting( path ) )
			return;
		
		m_ToDeploy = GetGame().ConfigGetTextOut( path );
	}
	
	// ------------------------------------------------------------
	array< string > GetPlacingTypes()
	{
		return m_PlacingTypes;
	}

	// ------------------------------------------------------------
	string GetDeployType()
	{
		return m_ToDeploy;
	}

	// ------------------------------------------------------------
	int GetPlacingTypeChosen()
	{
		return m_PlacingTypeChosen;
	}

	override void DisassembleKit(ItemBase item)
	{
	}
}

/**@class		ExpansionKitSmall
 * @brief		
 **/
class ExpansionKitSmall extends ExpansionKitBase {}

/**@class		ExpansionKitLarge
 * @brief		
 **/
class ExpansionKitLarge extends ExpansionKitBase {}
