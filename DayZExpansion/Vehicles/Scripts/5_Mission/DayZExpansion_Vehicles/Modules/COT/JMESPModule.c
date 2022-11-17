/**
 * JMESPModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef JM_COT
modded class JMESPModule
{
	override private void Exec_SetPosition( vector position, Object target, PlayerIdentity ident, JMPlayerInstance instance = NULL )
	{
		auto trace = EXTrace.Start(EXTrace.ENABLE, this);

		CarScript car;
		if ( Class.CastTo( car, target ) )
		{
			car.Expansion_ForcePositionAndOrientation(position, car.GetOrientation());
		
			GetCommunityOnlineToolsBase().Log( ident, "ESP target=" + target + " action=position value=" + position );
			SendWebhook( "Position", instance, "Set \"" + target.GetDisplayName() + "\" (" + target.GetType() + ") position to " + position.ToString() );
		}
		else
		{
			super.Exec_SetPosition(position, target, ident, instance);
		}
	}
}
#endif
