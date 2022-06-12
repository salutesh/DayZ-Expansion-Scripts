
/**
 * ExpansionDefaultQuestData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! We will need to handle all this data thrue a loaded config file for each quest template later
class ExpansionDefaultQuestData
{
	private string m_WorldName;
	
	void ExpansionDefaultQuestData(string worldName)
	{
		m_WorldName = worldName;
	}
	
	//! Quest example template for a quest chain of 3 quests with different objectives (move to location -> deliver item -> kill targets -> deliver item)
	ExpansionQuestConfig ExpansionQuestConfig001()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(1); //! Unique quest ID - Need to be the same as when registered in the quest module
		questConfig.SetType(ExpansionQuestType.DELIVERY);
		questConfig.SetTitle("A dirty rats job"); //! Quest title
		questConfig.SetObjectiveText("Deliver the Note and talk to Steve."); //! Quest objectives text
		array<string> desc = new array<string>;
		desc.Insert("Shit man who are you? %1 is your name? Oh shit i thought we'd already dealt with all of you. ** (He stares at you for a moment and then beginns to smile) ** Ah nevermind...today is your lucky day my friend! If you do me a little favor, I'll make sure you get off this island as soon as possible. I swear on my mother's beard! All you have to do is take this NOTE here to my useless colleague Steve and have him read it and then he will give something to you. Bring it to me! You find Steve in the little village down the street in the north west. Make sure he reads the note and dont come back with empty hands!"); //! 0 - Quest description on getting quest
		desc.Insert("I thought i made clear what you need to do?! Go to Steve now and don't come back with empty Hands!"); //! 1 - Quest description while quest is active
		desc.Insert("Look at that... an other little bambie in the forests of death..."); //! 2 - Quest description when take in quest
		questConfig.SetDescriptions(desc);

		questConfig.SetPreQuestID(-1); //! Pre-quest ID
		questConfig.SetFollowUpQuestID(2); //! Follow-up quest ID
		questConfig.SetQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(2); //! Quest NPC ID of the NPC that will take-in the quest

		//! Quest objectives
		ExpansionQuestObjectiveDeliveryConfig objective_1 = new ExpansionQuestObjectiveDeliveryConfig();
		objective_1.SetID(1);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.DELIVERY);
		questConfig.AddObjectiveConfig(objective_1);

		return questConfig;
	}

	ExpansionQuestConfig ExpansionQuestConfig002()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(2);
		questConfig.SetType(ExpansionQuestType.TARGET);
		questConfig.SetTitle("A favor for Steve...");
		questConfig.SetObjectiveText("Kill 10 civilian infected humans with the Sledge-Hammer.");
		array<string> desc = new array<string>;
		desc.Insert("So Peter sends you hm? Well i have what he wants but he still owes me something... But i am also not a bad guy. Lets say if you do me a favor, too, I'll give you what Steve wants and even more. I gotta keep watch around here and make sure no shit happens here and i want you to take this sledgehammer and clean up the village with it. There are a few infected people who have to be eliminated before they start moving into the camp."); //! 0 - Quest description on getting quest
		desc.Insert("You are not done yet? How hard can it be to smash some heads with that hammer... Come back when the job is done!"); //! 1 - Quest description while quest is active
		desc.Insert("Oh there you are! I thought the infected got your ass and killed you.. Well here is your reward."); //! 2 - Quest description when take in quest
		questConfig.SetDescriptions(desc);

		questConfig.SetPreQuestID(1);
		questConfig.SetFollowUpQuestID(3);
		questConfig.SetQuestGiverID(2);
		questConfig.SetQuestTurnInID(2);

		//! Quest objectives
		ExpansionQuestObjectiveTravelConfig objective_1 = new ExpansionQuestObjectiveTravelConfig();
		objective_1.SetID(2);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.TRAVEL);
		questConfig.AddObjectiveConfig(objective_1);
		
		ExpansionQuestObjectiveTargetConfig objective_2 = new ExpansionQuestObjectiveTargetConfig();
		objective_2.SetID(1);
		objective_2.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		questConfig.AddObjectiveConfig(objective_2);

		//! Quest items on quest start
		array <ExpansionQuestItemConfig> questItems = new array<ExpansionQuestItemConfig>;
		ExpansionQuestItemConfig questItem_1 = new ExpansionQuestItemConfig;
		questItem_1.SetClassName("SledgeHammer");
		questItem_1.SetAmount(1);
		questItems.Insert(questItem_1);
		questConfig.SetQuestItems(questItems);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("WaterBottle");
		reward_1.SetAmount(1);
		rewards.Insert(reward_1);

		ExpansionQuestRewardConfig reward_2 = new ExpansionQuestRewardConfig;
		reward_2.SetClassName("TunaCan");
		reward_2.SetAmount(1);
		rewards.Insert(reward_2);
		questConfig.SetRewards(rewards);

		return questConfig;
	}

	ExpansionQuestConfig ExpansionQuestConfig003()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(3);
		questConfig.SetType(ExpansionQuestType.DELIVERY);
		questConfig.SetTitle("Peter's package");
		questConfig.SetObjectiveText("Go back to peter and give him the package from Steve.");
		array<string> desc = new array<string>;
		desc.Insert("Oh right, here's what Peter wants. Bring it to him and he will be satisfied. Don't even think about opening the package! Just go and bring it to him.");
		desc.Insert("What are you doing still here? Go to Peter!");
		desc.Insert("Why did this take so long?..You go what I wanted? ** (He looks at the package in your hands with wide eyes and rips it out of your hands) ** Good!....What? Your way out? Well what should i say but i lied to you. There is no fucking way out from this hell and you will die here as we all. You can have this backpack and try your luck out there. I am sorry i lied to you but we all have to see how we somehow survive here. I wish you good luck out there...you will need it.");
		questConfig.SetDescriptions(desc);

		questConfig.SetPreQuestID(2); //! Pre-quest ID
		questConfig.SetFollowUpQuestID(-1); //! Follow-up quest ID
		questConfig.SetQuestGiverID(2); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(1); //! Quest NPC ID of the NPC that will take-in the quest

		//! Quest objectives
		ExpansionQuestObjectiveDeliveryConfig objective_1 = new ExpansionQuestObjectiveDeliveryConfig();
		objective_1.SetID(2);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.DELIVERY);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("TaloonBag_Blue");
		reward_1.SetAmount(1);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

#ifdef EXPANSIONMODMARKET
	//! Quest example template for a repeatable token quest
#ifdef WRDG_DOGTAGS
	ExpansionQuestConfig ExpansionQuestConfig004()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(4);
		questConfig.SetType(ExpansionQuestType.COLLECT);
		questConfig.SetTitle("Survivor - Dog Tag");
		questConfig.SetObjectiveText("Exchange a survivor dog tag");
		array<string> desc = new array<string>;
		desc.Insert("If you kill a bambi player for me i will give you something when you come back to me and give me the dog tag.");
		desc.Insert("Obtain a dog tag from a player with the rank: Survivor.");
		desc.Insert("Turn-in quest to get reward.");
		questConfig.SetDescriptions(desc);
		questConfig.SetQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(1); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsDaylieQuest(true); //! Set if the quest is a daylie quest and has as 24h cooldown if the quest is also repeatable.

		//! Quest objectives
		ExpansionQuestObjectiveCollectionConfig objective_1 = new ExpansionQuestObjectiveCollectionConfig();
		objective_1.SetID(1);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionSilverNugget");
		reward_1.SetAmount(10);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}
#endif

	//! Quest example template for a treasure hunt quest
	ExpansionQuestConfig ExpansionQuestConfig005()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(5);
		questConfig.SetType(ExpansionQuestType.TREASUREHUNT);
		questConfig.SetTitle("Treasure Hunt - Currency");
		questConfig.SetObjectiveText("Find the treasure.");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		questConfig.SetDescriptions(desc);
		questConfig.SetQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(1); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsDaylieQuest(true); //! Set if the quest is a daylie quest and has as 24h cooldown if the quest is also repeatable.
		questConfig.SetAutocomplet(true); //! Set if the quest is autocpmpleted

		//! Quest items on quest start
		array <ExpansionQuestItemConfig> questItems = new array<ExpansionQuestItemConfig>;
		ExpansionQuestItemConfig questItem_1 = new ExpansionQuestItemConfig;
		questItem_1.SetClassName("Shovel");
		questItem_1.SetAmount(1);
		questItems.Insert(questItem_1);
		questConfig.SetQuestItems(questItems);

		//! Quest objectives
		ExpansionQuestObjectiveTreasureHuntConfig objective_1 = new ExpansionQuestObjectiveTreasureHuntConfig();
		objective_1.SetID(1);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.TREASUREHUNT);
		questConfig.AddObjectiveConfig(objective_1);

		return questConfig;
	}

#ifdef EXPANSIONMODAI
	//! Quest example template for a ai patrol target quest
	ExpansionQuestConfig ExpansionQuestConfig006()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(6);
		questConfig.SetType(ExpansionQuestType.AIPATROL);
		questConfig.SetTitle("Clear Patrol - Bandits");
		questConfig.SetObjectiveText("Kill all units of the bandit patrol group at the marked location.");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		questConfig.SetDescriptions(desc);
		questConfig.SetQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(1); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed

		//! Quest objectives
		ExpansionQuestObjectiveAIPatrolConfig objective_1 = new ExpansionQuestObjectiveAIPatrolConfig();
		objective_1.SetID(1);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.AIPATROL);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionSilverNugget");
		reward_1.SetAmount(500);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	//! Quest example template for a ai camp target quest
	ExpansionQuestConfig ExpansionQuestConfig007()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(7);
		questConfig.SetType(ExpansionQuestType.AICAMP);
		questConfig.SetTitle("Clear Village - Bandits");
		questConfig.SetObjectiveText("Kill all bandits at the marked location.");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		questConfig.SetDescriptions(desc);
		questConfig.SetQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(1); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed

		//! Quest objectives
		ExpansionQuestObjectiveTravelConfig objective_1 = new ExpansionQuestObjectiveTravelConfig();
		objective_1.SetID(1);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.TRAVEL);
		questConfig.AddObjectiveConfig(objective_1);
		
		ExpansionQuestObjectiveAICampConfig objective_2 = new ExpansionQuestObjectiveAICampConfig();
		objective_2.SetID(1);
		objective_2.SetObjectiveType(ExpansionQuestObjectiveType.AICAMP);
		questConfig.AddObjectiveConfig(objective_2);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionSilverNugget");
		reward_1.SetAmount(500);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}
#endif

#ifdef EXPANSIONMODGROUPS
	//! Quest example template for a simple group quest
	ExpansionQuestConfig ExpansionQuestConfig008()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(8);
		questConfig.SetType(ExpansionQuestType.TARGET);
		questConfig.SetTitle("Kill - Civilian Infected");
		questConfig.SetObjectiveText("Kill 100 civilian infected.");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		questConfig.SetDescriptions(desc);
		questConfig.SetQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(1); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsGroupQuest(true); //! Set if the quest is a group quest and can only accepted in a group by the group owner.
		questConfig.SetIsWeeklyQuest(true); //! Set if the quest is a weekly quest and has as 7 day cooldown if the quest is also repeatable.

		ExpansionQuestObjectiveTargetConfig objective_2 = new ExpansionQuestObjectiveTargetConfig();
		objective_2.SetID(2);
		objective_2.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		questConfig.AddObjectiveConfig(objective_2);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionSilverNugget");
		reward_1.SetAmount(1000);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}
#endif

#ifdef WRDG_DOGTAGS
#ifdef EXPANSIONMODHARDLINE
	//! Bandit Token Quests
	ExpansionQuestConfig ExpansionQuestConfig009()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(9);
		questConfig.SetType(ExpansionQuestType.COLLECT);
		questConfig.SetTitle("[Dog Tag] Scout");
		questConfig.SetObjectiveText("Obtain a Scout dog tag.");
		array<string> desc = new array<string>;
		desc.Insert("If you kill a Player with the Scout rank for me i will give you something when you come back to me and give me the dog tag.");
		desc.Insert("Obtain a dog tag from a player with the rank: Scout.");
		desc.Insert("Turn-in quest to get reward.");
		questConfig.SetDescriptions(desc);
		questConfig.SetQuestGiverID(3); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(3); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsWeeklyQuest(true); //! Set if the quest is a weekly quest and has as 7 day cooldown if the quest is also repeatable.
	#ifdef EXPANSIONMODHARDLINE
		questConfig.SetIsBanditQuest(true);
	#endif

		//! Quest objectives
		ExpansionQuestObjectiveCollectionConfig objective_1 = new ExpansionQuestObjectiveCollectionConfig();
		objective_1.SetID(3);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionSilverNugget");
		reward_1.SetAmount(100);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	ExpansionQuestConfig ExpansionQuestConfig010()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(10);
		questConfig.SetType(ExpansionQuestType.COLLECT);
		questConfig.SetTitle("[Dog Tag] Pathfinder");
		questConfig.SetObjectiveText("Obtain a Pathfinder dog tag.");
		array<string> desc = new array<string>;
		desc.Insert("If you kill a Player with the Pathfinder rank for me i will give you something when you come back to me and give me the dog tag.");
		desc.Insert("Obtain a dog tag from a player with the rank: Pathfinder.");
		desc.Insert("Turn-in quest to get reward.");
		questConfig.SetDescriptions(desc);
		questConfig.SetQuestGiverID(3); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(3); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsWeeklyQuest(true); //! Set if the quest is a weekly quest and has as 7 day cooldown if the quest is also repeatable.
	#ifdef EXPANSIONMODHARDLINE
		questConfig.SetIsBanditQuest(true);
	#endif

		//! Quest objectives
		ExpansionQuestObjectiveCollectionConfig objective_1 = new ExpansionQuestObjectiveCollectionConfig();
		objective_1.SetID(4);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionSilverNugget");
		reward_1.SetAmount(200);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	ExpansionQuestConfig ExpansionQuestConfig011()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(11);
		questConfig.SetType(ExpansionQuestType.COLLECT);
		questConfig.SetTitle("[Dog Tag] Superhero");
		questConfig.SetObjectiveText("Obtain a Superhero dog tag.");
		array<string> desc = new array<string>;
		desc.Insert("If you kill a Player with the Superhero rank for me i will give you something when you come back to me and give me the dog tag.");
		desc.Insert("Obtain a dog tag from a player with the rank: Superhero.");
		desc.Insert("Turn-in quest to get reward.");
		questConfig.SetDescriptions(desc);
		questConfig.SetQuestGiverID(3); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(3); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsWeeklyQuest(true); //! Set if the quest is a weekly quest and has as 7 day cooldown if the quest is also repeatable.
	#ifdef EXPANSIONMODHARDLINE
		questConfig.SetIsBanditQuest(true);
	#endif

		//! Quest objectives
		ExpansionQuestObjectiveCollectionConfig objective_1 = new ExpansionQuestObjectiveCollectionConfig();
		objective_1.SetID(5);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionSilverNugget");
		reward_1.SetAmount(1000);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	ExpansionQuestConfig ExpansionQuestConfig012()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(12);
		questConfig.SetType(ExpansionQuestType.COLLECT);
		questConfig.SetTitle("[Dog Tag] Legend");
		questConfig.SetObjectiveText("Obtain a Legend dog tag.");
		array<string> desc = new array<string>;
		desc.Insert("If you kill a Player with the Legend rank for me i will give you something when you come back to me and give me the dog tag.");
		desc.Insert("Obtain a dog tag from a player with the rank: Legend.");
		desc.Insert("Turn-in quest to get reward.");
		questConfig.SetDescriptions(desc);
		questConfig.SetQuestGiverID(3); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(3); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsWeeklyQuest(true); //! Set if the quest is a weekly quest and has as 7 day cooldown if the quest is also repeatable.
	#ifdef EXPANSIONMODHARDLINE
		questConfig.SetIsBanditQuest(true);
	#endif

		//! Quest objectives
		ExpansionQuestObjectiveCollectionConfig objective_1 = new ExpansionQuestObjectiveCollectionConfig();
		objective_1.SetID(6);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionSilverNugget");
		reward_1.SetAmount(2000);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	//! Hero Token Quests
	ExpansionQuestConfig ExpansionQuestConfig013()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(13);
		questConfig.SetType(ExpansionQuestType.COLLECT);
		questConfig.SetTitle("[Dog Tag] Kleptomaniac");
		questConfig.SetObjectiveText("Obtain a Kleptomaniac dog tag.");
		array<string> desc = new array<string>;
		desc.Insert("If you kill a Player with the Kleptomaniac rank for me i will give you something when you come back to me and give me the dog tag.");
		desc.Insert("Obtain a dog tag from a player with the rank: Kleptomaniac.");
		desc.Insert("Turn-in quest to get reward.");
		questConfig.SetDescriptions(desc);
		questConfig.SetQuestGiverID(4); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(4); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsWeeklyQuest(true); //! Set if the quest is a weekly quest and has as 7 day cooldown if the quest is also repeatable.
	#ifdef EXPANSIONMODHARDLINE
		questConfig.SetIsHeroQuest(true);
	#endif

		//! Quest objectives
		ExpansionQuestObjectiveCollectionConfig objective_1 = new ExpansionQuestObjectiveCollectionConfig();
		objective_1.SetID(7);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionSilverNugget");
		reward_1.SetAmount(100);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	ExpansionQuestConfig ExpansionQuestConfig014()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(14);
		questConfig.SetType(ExpansionQuestType.COLLECT);
		questConfig.SetTitle("[Dog Tag] Bully");
		questConfig.SetObjectiveText("Obtain a Bully dog tag.");
		array<string> desc = new array<string>;
		desc.Insert("If you kill a Player with the Bully rank for me i will give you something when you come back to me and give me the dog tag.");
		desc.Insert("Obtain a dog tag from a player with the rank: Bully.");
		desc.Insert("Turn-in quest to get reward.");
		questConfig.SetDescriptions(desc);
		questConfig.SetQuestGiverID(4); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(4); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsWeeklyQuest(true); //! Set if the quest is a weekly quest and has as 7 day cooldown if the quest is also repeatable.
	#ifdef EXPANSIONMODHARDLINE
		questConfig.SetIsHeroQuest(true);
	#endif

		//! Quest objectives
		ExpansionQuestObjectiveCollectionConfig objective_1 = new ExpansionQuestObjectiveCollectionConfig();
		objective_1.SetID(8);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionSilverNugget");
		reward_1.SetAmount(200);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	ExpansionQuestConfig ExpansionQuestConfig015()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(15);
		questConfig.SetType(ExpansionQuestType.COLLECT);
		questConfig.SetTitle("[Dog Tag] Killer");
		questConfig.SetObjectiveText("Obtain a Killer dog tag.");
		array<string> desc = new array<string>;
		desc.Insert("If you kill a Player with the Killer rank for me i will give you something when you come back to me and give me the dog tag.");
		desc.Insert("Obtain a dog tag from a player with the rank: Killer.");
		desc.Insert("Turn-in quest to get reward.");
		questConfig.SetDescriptions(desc);
		questConfig.SetQuestGiverID(4); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(4); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsWeeklyQuest(true); //! Set if the quest is a weekly quest and has as 7 day cooldown if the quest is also repeatable.
	#ifdef EXPANSIONMODHARDLINE
		questConfig.SetIsHeroQuest(true);
	#endif

		//! Quest objectives
		ExpansionQuestObjectiveCollectionConfig objective_1 = new ExpansionQuestObjectiveCollectionConfig();
		objective_1.SetID(9);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionSilverNugget");
		reward_1.SetAmount(1000);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	ExpansionQuestConfig ExpansionQuestConfig016()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(16);
		questConfig.SetType(ExpansionQuestType.COLLECT);
		questConfig.SetTitle("[Dog Tag] Madman");
		questConfig.SetObjectiveText("Obtain a Madman dog tag.");
		array<string> desc = new array<string>;
		desc.Insert("If you kill a Player with the Madman rank for me i will give you something when you come back to me and give me the dog tag.");
		desc.Insert("Obtain a dog tag from a player with the rank: Madman.");
		desc.Insert("Turn-in quest to get reward.");
		questConfig.SetDescriptions(desc);
		questConfig.SetQuestGiverID(4); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(4); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsWeeklyQuest(true); //! Set if the quest is a weekly quest and has as 7 day cooldown if the quest is also repeatable.
	#ifdef EXPANSIONMODHARDLINE
		questConfig.SetIsHeroQuest(true);
	#endif

		//! Quest objectives
		ExpansionQuestObjectiveCollectionConfig objective_1 = new ExpansionQuestObjectiveCollectionConfig();
		objective_1.SetID(10);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionSilverNugget");
		reward_1.SetAmount(2000);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	ExpansionQuestConfig ExpansionQuestConfig017()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(17);
		questConfig.SetType(ExpansionQuestType.COLLECT);
		questConfig.SetTitle("[Dog Tag] Hero");
		questConfig.SetObjectiveText("Obtain a Hero dog tag.");
		array<string> desc = new array<string>;
		desc.Insert("If you kill a Player with the Hero rank for me i will give you something when you come back to me and give me the dog tag.");
		desc.Insert("Obtain a dog tag from a player with the rank: Hero.");
		desc.Insert("Turn-in quest to get reward.");
		questConfig.SetDescriptions(desc);
		questConfig.SetQuestGiverID(4); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(4); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsWeeklyQuest(true); //! Set if the quest is a weekly quest and has as 7 day cooldown if the quest is also repeatable.
	#ifdef EXPANSIONMODHARDLINE
		questConfig.SetIsBanditQuest(true);
	#endif

		//! Quest objectives
		ExpansionQuestObjectiveCollectionConfig objective_1 = new ExpansionQuestObjectiveCollectionConfig();
		objective_1.SetID(11);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("MoneyRuble");
		reward_1.SetAmount(400);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	ExpansionQuestConfig ExpansionQuestConfig018()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(18);
		questConfig.SetType(ExpansionQuestType.COLLECT);
		questConfig.SetTitle("[Dog Tag] Bandit");
		questConfig.SetObjectiveText("Obtain a Bandit dog tag.");
		array<string> desc = new array<string>;
		desc.Insert("If you kill a Player with the Bandit rank for me i will give you something when you come back to me and give me the dog tag.");
		desc.Insert("Obtain a dog tag from a player with the rank: Bandit.");
		desc.Insert("Turn-in quest to get reward.");
		questConfig.SetDescriptions(desc);
		questConfig.SetQuestGiverID(4); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(4); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsWeeklyQuest(true); //! Set if the quest is a weekly quest and has as 7 day cooldown if the quest is also repeatable.
	#ifdef EXPANSIONMODHARDLINE
		questConfig.SetIsHeroQuest(true);
	#endif

		//! Quest objectives
		ExpansionQuestObjectiveCollectionConfig objective_1 = new ExpansionQuestObjectiveCollectionConfig();
		objective_1.SetID(12);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionSilverNugget");
		reward_1.SetAmount(400);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}
#endif
#endif

	//! Achievement example quests
	ExpansionQuestConfig ExpansionQuestConfig019()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(19);
		questConfig.SetType(ExpansionQuestType.ACHIVEMENT);
		questConfig.SetTitle("Killed 10 Infected");
		questConfig.SetObjectiveText("You have killed 10 infected and completed a server achievement! Reward: 100 Ruble");
		questConfig.SetAutocomplet(true);
		questConfig.SetIsAchivement(true);
		
		//! Quest objectives
		ExpansionQuestObjectiveTargetConfig objective_1 = new ExpansionQuestObjectiveTargetConfig();
		objective_1.SetID(3);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionSilverNugget");
		reward_1.SetAmount(100);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	ExpansionQuestConfig ExpansionQuestConfig020()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(20);
		questConfig.SetType(ExpansionQuestType.ACHIVEMENT);
		questConfig.SetTitle("Killed 100 Civilian Infected");
		questConfig.SetObjectiveText("You have killed 100 civilian infected and completed a server achievement! Reward: 500 Ruble");
		questConfig.SetAutocomplet(true);
		questConfig.SetIsAchivement(true);

		//! Quest objectives
		ExpansionQuestObjectiveTargetConfig objective_1 = new ExpansionQuestObjectiveTargetConfig();
		objective_1.SetID(2);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionSilverNugget");
		reward_1.SetAmount(500);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}
	
	ExpansionQuestConfig ExpansionQuestConfig021()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(21); //! Unique quest ID - Need to be the same as when registered in the quest module
		questConfig.SetType(ExpansionQuestType.CLASS);
		questConfig.SetTitle("PLACEHOLDER"); //! Quest title
		questConfig.SetObjectiveText("PLACEHOLDER"); //! Quest objectives text
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER"); //! 0 - Quest description on getting quest
		desc.Insert("PLACEHOLDER"); //! 1 - Quest description while quest is active
		desc.Insert("PLACEHOLDER"); //! 2 - Quest description when take in quest
		questConfig.SetDescriptions(desc);

		questConfig.SetPreQuestID(-1); //! Pre-quest ID
		questConfig.SetFollowUpQuestID(22); //! Follow-up quest ID
		questConfig.SetQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(2); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetQuestClassName("ExpansionQuestNamalsk001");

		return questConfig;
	}

#ifdef EXPANSIONMODAI
	ExpansionQuestConfig ExpansionQuestConfig022()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(22); //! Unique quest ID - Need to be the same as when registered in the quest module
		questConfig.SetType(ExpansionQuestType.AIVIP);
		questConfig.SetTitle("[VIP] Test quest"); //! Quest title
		questConfig.SetObjectiveText("PLACEHOLDER"); //! Quest objectives text
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER"); //! 0 - Quest description on getting quest
		desc.Insert("PLACEHOLDER"); //! 1 - Quest description while quest is active
		desc.Insert("PLACEHOLDER"); //! 2 - Quest description when take in quest
		questConfig.SetDescriptions(desc);
		questConfig.SetQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.SetQuestTurnInID(2); //! Quest NPC ID of the NPC that will take-in the quest
		
		//! Quest objectives
		ExpansionQuestObjectiveAIVIPConfig objective_1 = new ExpansionQuestObjectiveAIVIPConfig();
		objective_1.SetID(1);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.AIVIP);
		
		questConfig.AddObjectiveConfig(objective_1);
		
		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionSilverNugget");
		reward_1.SetAmount(100);
		rewards.Insert(reward_1);
		
		questConfig.SetRewards(rewards);

		return questConfig;
	}
#endif
#endif
};