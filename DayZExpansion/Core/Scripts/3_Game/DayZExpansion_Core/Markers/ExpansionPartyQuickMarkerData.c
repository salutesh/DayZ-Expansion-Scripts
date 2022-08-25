/**
 * ExpansionPartyQuickMarkerData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODGROUPS
class ExpansionPartyQuickMarkerData : ExpansionMarkerData
{
	override ExpansionMapMarkerType GetType()
	{
		return ExpansionMapMarkerType.PARTY_QUICK;
	}

	override void OnSend( ParamsWriteContext ctx )
	{
		ctx.Write( m_Color );
		
		ctx.Write( m_Position );
	}

	override bool OnRecieve( ParamsReadContext ctx )
	{
		if ( !ctx.Read( m_Color ) )
			return false;

		if ( !ctx.Read( m_Position ) )
			return false;

		return true;
	}
};
#endif