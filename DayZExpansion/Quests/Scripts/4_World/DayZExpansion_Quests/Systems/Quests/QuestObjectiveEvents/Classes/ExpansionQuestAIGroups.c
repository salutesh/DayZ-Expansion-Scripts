/**
 * ExpansionQuestAIGroups.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODAI
class ExpansionQuestAIGroups
{
	float RespawnTime = -1;					// Time in seconds before the dead group will respawn. If set to -1, they won't respawn
	float MinDistRadius = 300;			   		// If the player is closer than MinDistRadius from the spawn point, the group won't spawn
	float MaxDistRadius = 1200;			    // Same but if the player is further away than MaxDistRadius, the bots won't spawn
	ref ExpansionQuestAIGroup Group;
};
#endif