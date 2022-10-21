/**
 * ExpansionQuestObjectiveAIVIP.c
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
class ExpansionQuestObjectiveAIVIP
{
	protected string NPCLoadoutFile;

	void SetNPCLoadoutFile(string loadoutFile)
	{
		NPCLoadoutFile = loadoutFile;
	}

	string GetNPCLoadoutFile()
	{
		return NPCLoadoutFile;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		CF_Log.Debug("------------------------------------------------------------");
		CF_Log.Debug(ToString() + "::QuestDebug - NPCLoadoutFile: " + NPCLoadoutFile);
		CF_Log.Debug("------------------------------------------------------------");
	#endif
	}
};
#endif