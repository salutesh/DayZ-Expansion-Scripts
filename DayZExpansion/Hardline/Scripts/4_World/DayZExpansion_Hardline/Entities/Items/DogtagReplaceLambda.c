/**
 * DogtagReplaceLambda.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef WRDG_DOGTAGS
modded class DogtagReplaceLambda
{
	protected override void OnCopyProperties(Dogtag_Base old_tag, Dogtag_Base new_tag)
	{
		super.OnCopyProperties(old_tag, new_tag);
		
		if (GetExpansionSettings().GetHardline().UseHumanity)
			new_tag.SetHardlineHumanity(old_tag.GetHardlineHumanity());
	}
};
#endif