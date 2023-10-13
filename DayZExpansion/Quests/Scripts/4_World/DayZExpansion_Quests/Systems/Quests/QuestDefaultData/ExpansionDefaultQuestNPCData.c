/**
 * ExpansionDefaultQuestNPCData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionDefaultQuestNPCData
{
	protected string m_WorldName;

	void ExpansionDefaultQuestNPCData(string worldName)
	{
		m_WorldName = worldName;
	}

	 ExpansionQuestNPCData QuestNPCData_1()
	{
		ExpansionQuestNPCData questNPC = new ExpansionQuestNPCData;
		questNPC.SetID(1); //! Unique NPC ID
		questNPC.SetClassName("ExpansionQuestNPCDenis"); //! Class name of the NPC entity

		if (m_WorldName == "namalsk")
		{
			questNPC.SetPosition(Vector(8584.27, 14.73, 10511.6)); //! Quest NPC position
			questNPC.SetOrientation(Vector(200.0, 0, 0)); //! Quest NPC orientation

		#ifdef EXPANSIONMODAI
			questNPC.SetClassName("ExpansionQuestNPCAIDenis"); //! Class name of the NPC entity
			questNPC.AddWaypoint(Vector(8584.27, 14.73, 10511.6));
		#endif
			questNPC.SetNPCType(ExpansionQuestNPCType.AI);
		}
		else if (m_WorldName == "chernarusplus" || m_WorldName == "chernarusplusgloom")
		{
			questNPC.SetPosition(Vector(3706.27, 402.012, 5987.08)); //! Quest NPC position
			questNPC.SetOrientation(Vector(282.0, 0, 0)); //! Quest NPC orientation

		#ifdef EXPANSIONMODAI
			questNPC.SetClassName("ExpansionQuestNPCAIDenis"); //! Class name of the NPC entity
			questNPC.AddWaypoint(Vector(3706.27, 402.012, 5987.08));
		#endif
			questNPC.SetNPCType(ExpansionQuestNPCType.AI);
		}

		questNPC.SetLoadoutFile("NBCLoadout"); //! Quest NPC gear

		questNPC.SetNPCName("Peter");
		questNPC.SetDefaultNPCText("Hmm?");

		return questNPC;
	}

	ExpansionQuestNPCData QuestNPCData_2()
	{
		ExpansionQuestNPCData questNPC = new ExpansionQuestNPCData;
		questNPC.SetID(2); //! Unique NPC ID
		questNPC.SetClassName("ExpansionQuestNPCElias"); //! Class name of the NPC entity
		questNPC.SetOrientation(Vector(-10.0, 0, 0)); //! Quest NPC orientation

		if (m_WorldName == "namalsk")
		{
			questNPC.SetPosition(Vector(8348.39, 15.1237, 10724.7)); //! Quest NPC position
			questNPC.SetOrientation(Vector(-10.0, 0, 0)); //! Quest NPC orientation

		#ifdef EXPANSIONMODAI
			questNPC.SetClassName("ExpansionQuestNPCAIElias"); //! Class name of the NPC entity
			questNPC.AddWaypoint(Vector(8348.39, 15.1237, 10724.7));
		#endif
			questNPC.SetNPCType(ExpansionQuestNPCType.AI);
		}
		else if (m_WorldName == "chernarusplus" || m_WorldName == "chernarusplusgloom")
		{
			questNPC.SetPosition(Vector(3192.2, 296.707, 6093.31)); //! Quest NPC position
			questNPC.SetOrientation(Vector(145.0, 0, 0)); //! Quest NPC orientation

		#ifdef EXPANSIONMODAI
			questNPC.SetClassName("ExpansionQuestNPCAIElias"); //! Class name of the NPC entity
			questNPC.AddWaypoint(Vector(3192.2, 296.707, 6093.31));
			questNPC.SetEmoteID(EmoteConstants.ID_EMOTE_SITA);
			questNPC.SetIsEmoteStatic(true);
		#endif
			questNPC.SetNPCType(ExpansionQuestNPCType.AI);
		}

		questNPC.SetLoadoutFile("NBCLoadout"); //! Quest NPC gear

		questNPC.SetNPCName("Steve");
		questNPC.SetDefaultNPCText("What do you want? I am busy...");

		return questNPC;
	}

	//! Static Quest Object
	ExpansionQuestNPCData QuestNPCData_3()
	{
		ExpansionQuestNPCData questNPC = new ExpansionQuestNPCData;
		questNPC.SetID(3); //! Unique NPC ID
		questNPC.SetClassName("ExpansionQuestBoardLarge"); //! Class name of the NPC entity
		questNPC.SetNPCName("Quest Board");
		questNPC.SetDefaultNPCText("There is nothing to do here for you...");

		if (m_WorldName == "chernarusplus" || m_WorldName == "chernarusplus")
		{
			questNPC.SetPosition(Vector(3706.61, 403.25, 5983.4)); //! Quest NPC position
			questNPC.SetOrientation(Vector(170.99, 0, 0)); //! Quest NPC orientation
		}
		else if (m_WorldName == "namalsk")
		{
			questNPC.SetPosition(Vector(8584.236328, 15.967732, 10515.956055)); //! Quest NPC position
			questNPC.SetOrientation(Vector(-58.060162, 0, 0)); //! Quest NPC orientation
		}
		
		questNPC.SetNPCType(ExpansionQuestNPCType.OBJECT);

		return questNPC;
	}
};