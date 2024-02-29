
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
	protected string m_WorldName;

	void ExpansionDefaultQuestData(string worldName)
	{
		m_WorldName = worldName;
	}

	//! Quest example template for a quest chain of 3 quests with different objectives (move to location -> deliver item -> kill targets -> deliver item)
	ExpansionQuestConfig ExpansionQuestConfig001()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(1); //! Unique quest ID - Need to be the same as when registered in the quest module
		questConfig.SetTitle("A dirty rat's job"); //! Quest title
		questConfig.SetObjectiveText("Deliver the note and talk to Steve."); //! Quest objectives text
		array<string> desc = new array<string>;
		desc.Insert("Shit man, who are you? Is your name %1? I thought we already dealt with you. ** (He stares at you for a moment and then begins to smile) ** Ah nevermind... Today is your lucky day, my friend! If you do me a little favor, I'll make sure you get off this island as soon as possible. I swear by my mother's beard! All you have to do is take this NOTE here to my useless colleague Steve and have him read it, then he will give something to you. Bring it to me! You can find Steve in the little village down the street in the north west. Make sure he reads the note and don't come back empty handed!"); //! 0 - Quest description on getting quest
		desc.Insert("I thought I made clear what you need to do?! Go to Steve now and don't come back empty handed!"); //! 1 - Quest description while quest is active
		desc.Insert("Look at that... Another little bambi in the forests of death..."); //! 2 - Quest description when take in quest
		questConfig.SetDescriptions(desc);
		questConfig.SetFollowUpQuestID(2); //! Follow-up quest ID
		questConfig.AddQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.AddQuestTurnInID(2); //! Quest NPC ID of the NPC that will take-in the quest

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
		questConfig.SetTitle("A favor for Steve...");
		questConfig.SetObjectiveText("Kill 10 civilian Infected with Steve's sledgehammer.");
		array<string> desc = new array<string>;
		desc.Insert("So, Peter sends you, hmm? Well I have what he wants, although he still owes me something... But I'm also not a bad guy. Let's say if you do me a favor, too, I'll give you what Peter wants and even more. I gotta keep watch and make sure no shit happens around here. I want you to take this sledgehammer and clean up the village with it. There are a few Infected that have to be eliminated before they start moving into the camp."); //! 0 - Quest description on getting quest
		desc.Insert("You are not done yet? How hard can it be to smash some heads with that hammer... Come back when the job is done!"); //! 1 - Quest description while quest is active
		desc.Insert("Oh there you are! I thought the Infected got your ass and killed you... Well, here is your reward."); //! 2 - Quest description when take in quest
		questConfig.SetDescriptions(desc);

		questConfig.AddPreQuestID(1);
		questConfig.SetFollowUpQuestID(3);
		questConfig.AddQuestGiverID(2);
		questConfig.AddQuestTurnInID(2);

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
		questConfig.SetTitle("Peter's package");
		questConfig.SetObjectiveText("Go back to Peter and give him the package from Steve.");
		array<string> desc = new array<string>;
		desc.Insert("Oh right, here's what Peter wants. Bring it to him and he will be satisfied. Don't even think about opening the package! Just go and bring it to him.");
		desc.Insert("What are you still doing here? Go to Peter!");
		desc.Insert("Why did this take so long?... You got what I wanted? ** (He looks at the package with wide eyes and rips it out of your hands) ** Good!... What? Your way out? Well what should I say, but I lied to you. There is no fucking way out of this hell, and you will die here like all of us. You can have this backpack and try your luck out there. I am sorry I lied to you, but we all have to survive here somehow. I wish you good luck out there... You will need it.");
		questConfig.SetDescriptions(desc);

		questConfig.AddPreQuestID(2); //! Pre-quest ID
		questConfig.AddQuestGiverID(2); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.AddQuestTurnInID(1); //! Quest NPC ID of the NPC that will take-in the quest

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

		ExpansionQuestRewardConfig reward_2 = new ExpansionQuestRewardConfig;
		reward_2.SetClassName("TaloonBag_Green");
		reward_2.SetAmount(1);
		rewards.Insert(reward_2);

		ExpansionQuestRewardConfig reward_3 = new ExpansionQuestRewardConfig;
		reward_3.SetClassName("TaloonBag_Orange");
		reward_3.SetAmount(1);
		rewards.Insert(reward_3);

		ExpansionQuestRewardConfig reward_4 = new ExpansionQuestRewardConfig;
		reward_4.SetClassName("TaloonBag_Violet");
		reward_4.SetAmount(1);
		rewards.Insert(reward_4);

		questConfig.SetRewards(rewards);
		questConfig.SetNeedToSelectReward(true);

		return questConfig;
	}

	//! Quest example template for a treasure hunt quest
	ExpansionQuestConfig ExpansionQuestConfig004()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(4);
		questConfig.SetTitle("Treasure Hunt - Currency");
		questConfig.SetObjectiveText("Find the treasure.");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		questConfig.SetDescriptions(desc);
		questConfig.AddQuestGiverID(3); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.AddQuestTurnInID(3); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsDailyQuest(true); //! Set if the quest is a daily quest and has as 24h cooldown if the quest is also repeatable.
		questConfig.SetAutocomplete(true); //! Set if the quest is autocompleted

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
	ExpansionQuestConfig ExpansionQuestConfig005()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(5);
		questConfig.SetTitle("Clear Patrol - Bandits");
		questConfig.SetObjectiveText("Kill all members of the bandit patrol at the marked location.");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		questConfig.SetDescriptions(desc);
		questConfig.AddQuestGiverID(3); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.AddQuestTurnInID(3); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed

		//! Quest objectives
		ExpansionQuestObjectiveTravelConfig objective_1 = new ExpansionQuestObjectiveTravelConfig();
		objective_1.SetID(4);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.TRAVEL);
		questConfig.AddObjectiveConfig(objective_1);

		ExpansionQuestObjectiveAIPatrolConfig objective_2 = new ExpansionQuestObjectiveAIPatrolConfig();
		objective_2.SetID(1);
		objective_2.SetObjectiveType(ExpansionQuestObjectiveType.AIPATROL);
		questConfig.AddObjectiveConfig(objective_2);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionBanknoteHryvnia");
		reward_1.SetAmount(500);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);
		questConfig.SetQuestColor(ARGB(200, 142, 68, 173));

		return questConfig;
	}

	//! Quest example template for a ai camp target quest
	ExpansionQuestConfig ExpansionQuestConfig006()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(6);
		questConfig.SetTitle("Clear Village - Bandits");
		questConfig.SetObjectiveText("Kill all bandits at the marked location.");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		questConfig.SetDescriptions(desc);
		questConfig.AddQuestGiverID(3); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.AddQuestTurnInID(3); //! Quest NPC ID of the NPC that will take-in the quest
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
		reward_1.SetClassName("ExpansionBanknoteHryvnia");
		reward_1.SetAmount(500);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);
		questConfig.SetQuestColor(ARGB(200, 142, 68, 173));

		return questConfig;
	}
#endif

	//! Quest example template for a simple group quest
	ExpansionQuestConfig ExpansionQuestConfig007()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(7);
		questConfig.SetTitle("Kill - Civilian Infected");
		questConfig.SetObjectiveText("Kill 100 civilian Infected.");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		questConfig.SetDescriptions(desc);
		questConfig.AddQuestGiverID(3); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.AddQuestTurnInID(3); //! Quest NPC ID of the NPC that will take-in the quest
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
		reward_1.SetClassName("ExpansionBanknoteHryvnia");
		reward_1.SetAmount(1000);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	//! Achievement example quests
	ExpansionQuestConfig ExpansionQuestConfig008()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(8);
		questConfig.SetTitle("Killed 10 Infected");
		questConfig.SetObjectiveText("You have killed 10 Infected! Reward: 100 Hryvnia");
		questConfig.SetAutocomplete(true);
		questConfig.SetIsAchievement(true);

		//! Quest objectives
		ExpansionQuestObjectiveTargetConfig objective_1 = new ExpansionQuestObjectiveTargetConfig();
		objective_1.SetID(3);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionBanknoteHryvnia");
		reward_1.SetAmount(100);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	ExpansionQuestConfig ExpansionQuestConfig009()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(9);
		questConfig.SetTitle("Killed 100 civilian Infected");
		questConfig.SetObjectiveText("You have killed 100 civilian Infected! Reward: 500 Hryvnia");
		questConfig.SetAutocomplete(true);
		questConfig.SetIsAchievement(true);

		//! Quest objectives
		ExpansionQuestObjectiveTargetConfig objective_1 = new ExpansionQuestObjectiveTargetConfig();
		objective_1.SetID(2);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionBanknoteHryvnia");
		reward_1.SetAmount(500);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

#ifdef EXPANSIONMODAI
	//! AI Escort quest template
	ExpansionQuestConfig ExpansionQuestConfig010()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(10); //! Unique quest ID - Need to be the same as when registered in the quest module
		questConfig.SetTitle("[VIP] Test quest"); //! Quest title
		questConfig.SetObjectiveText("PLACEHOLDER"); //! Quest objectives text
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER"); //! 0 - Quest description on getting quest
		desc.Insert("PLACEHOLDER"); //! 1 - Quest description while quest is active
		desc.Insert("PLACEHOLDER"); //! 2 - Quest description when take in quest
		questConfig.SetDescriptions(desc);
		questConfig.AddQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.AddQuestTurnInID(2); //! Quest NPC ID of the NPC that will take-in the quest

		//! Quest objectives
		ExpansionQuestObjectiveAIEscortConfig objective_1 = new ExpansionQuestObjectiveAIEscortConfig();
		objective_1.SetID(1);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.AIESCORT);
		objective_1.SetTimeLimit(180);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionBanknoteHryvnia");
		reward_1.SetAmount(100);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);
		questConfig.SetQuestColor(ARGB(200, 142, 68, 173));

		return questConfig;
	}
#endif

	//! Achievement action quest to lockpick a vehicle
	ExpansionQuestConfig ExpansionQuestConfig011()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(11);
		questConfig.SetTitle("Lockpick your first vehicle");
		questConfig.SetObjectiveText("You have lockpicked your first vehicle! Reward: 500 Hryvnia");
		questConfig.SetAutocomplete(true);
		questConfig.SetIsAchievement(true);

		//! Quest objectives
		ExpansionQuestObjectiveActionConfig objective_1 = new ExpansionQuestObjectiveActionConfig();
		objective_1.SetID(1);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.ACTION);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionBanknoteHryvnia");
		reward_1.SetAmount(500);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	//! Daily quest template
	ExpansionQuestConfig ExpansionQuestConfig012()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(12);
		questConfig.SetTitle("Apple Juice");
		questConfig.SetObjectiveText("Get 5 apples");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER");
		desc.Insert("Obtain 5 apples.");
		desc.Insert("Turn in quest to get reward.");
		questConfig.SetDescriptions(desc);

		questConfig.AddQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.AddQuestTurnInID(1); //! Quest NPC ID of the NPC that will take-in the quest

		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsDailyQuest(true); //! Set if the quest is a daily quest and has as 24h cooldown if the quest is also repeatable.

		//! Quest objectives
		ExpansionQuestObjectiveCollectionConfig objective_1 = new ExpansionQuestObjectiveCollectionConfig();
		objective_1.SetID(1);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionBanknoteHryvnia");
		reward_1.SetAmount(10);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	//! Crafting quest template.
	ExpansionQuestConfig ExpansionQuestConfig013()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(13);
		questConfig.SetTitle("Catch a Fish");
		questConfig.SetObjectiveText("Craft an improvised fishing rod and catch a mackerel!");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER");
		desc.Insert("Craft an improvised fishing rod and catch a mackerel.");
		desc.Insert("Turn in quest to get reward.");
		questConfig.SetDescriptions(desc);

		questConfig.AddQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.AddQuestTurnInID(1); //! Quest NPC ID of the NPC that will take-in the quest

		//! Quest objectives
		ExpansionQuestObjectiveCraftingConfig objective_1 = new ExpansionQuestObjectiveCraftingConfig();
		objective_1.SetID(1);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.CRAFTING);
		questConfig.AddObjectiveConfig(objective_1);

		ExpansionQuestObjectiveCollectionConfig objective_2 = new ExpansionQuestObjectiveCollectionConfig();
		objective_2.SetID(2);
		objective_2.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		questConfig.AddObjectiveConfig(objective_2);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionBanknoteHryvnia");
		reward_1.SetAmount(10);
		rewards.Insert(reward_1);

		ExpansionQuestRewardConfig reward_2 = new ExpansionQuestRewardConfig;
		reward_2.SetClassName("ImprovisedFishingRod");
		reward_2.SetAmount(1);
		rewards.Insert(reward_2);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	//! Template for a normal auto-start quest that get added to players on first connection
	ExpansionQuestConfig ExpansionQuestConfig014()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(14); //! Unique quest ID - Need to be the same as when registered in the quest module
		questConfig.SetTitle("Welcome to Hell"); //! Quest title
		questConfig.SetObjectiveText("Visit the camp at the marked location."); //! Quest objectives text
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER"); //! 0 - Quest description on getting quest
		desc.Insert("PLACEHOLDER"); //! 1 - Quest description while quest is active
		desc.Insert("PLACEHOLDER"); //! 2 - Quest description when take in quest
		questConfig.SetDescriptions(desc);
		questConfig.AddQuestTurnInID(1); //! Quest NPC ID of the NPC that will take-in the quest
		questConfig.SetAutocomplete(true); //! Quest completes automaticaly

		//! Quest objectives
		ExpansionQuestObjectiveTravelConfig objective_1 = new ExpansionQuestObjectiveTravelConfig();
		objective_1.SetID(3);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.TRAVEL);
		questConfig.AddObjectiveConfig(objective_1);

		return questConfig;
	}

	//! Template for a action quests to plant a special seed
	ExpansionQuestConfig ExpansionQuestConfig015()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(15);
		questConfig.SetTitle("Plant a Pumpkin");
		questConfig.SetObjectiveText("Plant a pumpkin seed.");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER"); //! 0 - Quest description on getting quest
		desc.Insert("PLACEHOLDER"); //! 1 - Quest description while quest is active
		desc.Insert("PLACEHOLDER"); //! 2 - Quest description when take in quest
		questConfig.SetDescriptions(desc);

		questConfig.AddQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.AddQuestTurnInID(1); //! Quest NPC ID of the NPC that will take-in the quest

		//! Quest objectives
		ExpansionQuestObjectiveActionConfig objective_1 = new ExpansionQuestObjectiveActionConfig();
		objective_1.SetID(2);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.ACTION);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionBanknoteHryvnia");
		reward_1.SetAmount(500);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	//! Template for a quest that unlocks other quests
	ExpansionQuestConfig ExpansionQuestConfig016()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(16);
		questConfig.SetTitle("For the Lich... I mean Yellow King!");
		questConfig.SetObjectiveText("Find the yellow king's helmet.");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER"); //! 0 - Quest description on getting quest
		desc.Insert("PLACEHOLDER"); //! 1 - Quest description while quest is active
		desc.Insert("PLACEHOLDER"); //! 2 - Quest description when take in quest
		questConfig.SetDescriptions(desc);

		questConfig.AddQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.AddQuestTurnInID(1); //! Quest NPC ID of the NPC that will take-in the quest

		//! Quest objectives
		ExpansionQuestObjectiveCollectionConfig objective_1 = new ExpansionQuestObjectiveCollectionConfig();
		objective_1.SetID(3);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.COLLECT);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionBanknoteHryvnia");
		reward_1.SetAmount(500);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	//! Template for a unlocked weekly quest.
	ExpansionQuestConfig ExpansionQuestConfig017()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(17);
		questConfig.SetTitle("In Service of the Yellow King");
		questConfig.SetObjectiveText("PLACEHOLDER");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER"); //! 0 - Quest description on getting quest
		desc.Insert("PLACEHOLDER"); //! 1 - Quest description while quest is active
		desc.Insert("PLACEHOLDER"); //! 2 - Quest description when take in quest
		questConfig.SetDescriptions(desc);

		questConfig.AddPreQuestID(16);

		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsWeeklyQuest(true); //! Set if the quest is a daily quest and has as 24h cooldown if the quest is also repeatable.

		questConfig.AddQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.AddQuestTurnInID(1); //! Quest NPC ID of the NPC that will take-in the quest

		//! Quest objectives
		ExpansionQuestObjectiveTargetConfig objective_1 = new ExpansionQuestObjectiveTargetConfig();
		objective_1.SetID(4);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionBanknoteHryvnia");
		reward_1.SetAmount(500);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	//! Template for a unlocked weekly quest.
	ExpansionQuestConfig ExpansionQuestConfig018()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(18);
		questConfig.SetTitle("The Endless Hunger");
		questConfig.SetObjectiveText("PLACEHOLDER");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER"); //! 0 - Quest description on getting quest
		desc.Insert("PLACEHOLDER"); //! 1 - Quest description while quest is active
		desc.Insert("PLACEHOLDER"); //! 2 - Quest description when take in quest
		questConfig.SetDescriptions(desc);

		questConfig.AddPreQuestID(16);

		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsWeeklyQuest(true); //! Set if the quest is a daily quest and has as 24h cooldown if the quest is also repeatable.

		questConfig.AddQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.AddQuestTurnInID(1); //! Quest NPC ID of the NPC that will take-in the quest

		//! Quest objectives
		ExpansionQuestObjectiveTargetConfig objective_1 = new ExpansionQuestObjectiveTargetConfig();
		objective_1.SetID(3);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionBanknoteHryvnia");
		reward_1.SetAmount(100);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

	//! Template for a unlocked weekly quest that gets unlocked when all the pre-quests are completed.
	ExpansionQuestConfig ExpansionQuestConfig019()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(19);
		questConfig.SetTitle("Where Kings Walk");
		questConfig.SetObjectiveText("PLACEHOLDER");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER"); //! 0 - Quest description on getting quest
		desc.Insert("PLACEHOLDER"); //! 1 - Quest description while quest is active
		desc.Insert("PLACEHOLDER"); //! 2 - Quest description when take in quest
		questConfig.SetDescriptions(desc);

		questConfig.AddPreQuestID(17);
		questConfig.AddPreQuestID(18);

		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed
		questConfig.SetIsWeeklyQuest(true); //! Set if the quest is a daily quest and has as 24h cooldown if the quest is also repeatable.

		questConfig.AddQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.AddQuestTurnInID(1); //! Quest NPC ID of the NPC that will take-in the quest

		//! Quest objectives
		ExpansionQuestObjectiveTargetConfig objective_1 = new ExpansionQuestObjectiveTargetConfig();
		objective_1.SetID(4);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		array<ExpansionQuestRewardConfig> rewards = new array<ExpansionQuestRewardConfig>;
		ExpansionQuestRewardConfig reward_1 = new ExpansionQuestRewardConfig;
		reward_1.SetClassName("ExpansionBanknoteHryvnia");
		reward_1.SetAmount(500);
		rewards.Insert(reward_1);

		questConfig.SetRewards(rewards);

		return questConfig;
	}

#ifdef EXPANSIONMODHARDLINE
	//! Template for a repeatable quest that has a reputation reward
	ExpansionQuestConfig ExpansionQuestConfig020()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(20);
		questConfig.SetTitle("For the Honor");
		questConfig.SetObjectiveText("PLACEHOLDER");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER"); //! 0 - Quest description on getting quest
		desc.Insert("PLACEHOLDER"); //! 1 - Quest description while quest is active
		desc.Insert("PLACEHOLDER"); //! 2 - Quest description when take in quest
		questConfig.SetDescriptions(desc);

		questConfig.AddQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.AddQuestTurnInID(1); //! Quest NPC ID of the NPC that will take-in the quest

		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed

		//! Quest objectives
		ExpansionQuestObjectiveTargetConfig objective_1 = new ExpansionQuestObjectiveTargetConfig();
		objective_1.SetID(3);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		questConfig.SetReputationReward(100);

		return questConfig;
	}

	//! Template for a repeatable quest that has a reputation requirement
	ExpansionQuestConfig ExpansionQuestConfig021()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(21);
		questConfig.SetTitle("You need the Honor");
		questConfig.SetObjectiveText("PLACEHOLDER");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER"); //! 0 - Quest description on getting quest
		desc.Insert("PLACEHOLDER"); //! 1 - Quest description while quest is active
		desc.Insert("PLACEHOLDER"); //! 2 - Quest description when take in quest
		questConfig.SetDescriptions(desc);

		questConfig.AddQuestGiverID(1); //! Quest NPC ID of the NPC that will head out the quest
		questConfig.AddQuestTurnInID(1); //! Quest NPC ID of the NPC that will take-in the quest

		questConfig.SetIsRepeatable(true); //! Set if the quest is repeatable and can be accapted again after it has been completed

		questConfig.SetReputationRequirement(100);

		//! Quest objectives
		ExpansionQuestObjectiveTargetConfig objective_1 = new ExpansionQuestObjectiveTargetConfig();
		objective_1.SetID(3);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.TARGET);
		questConfig.AddObjectiveConfig(objective_1);

		//! Quest rewards on quest completion
		questConfig.SetReputationReward(100);

		return questConfig;
	}
#endif
	
	ExpansionQuestConfig ExpansionQuestConfig022()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(22);
		questConfig.SetTitle("Quest chain example [Part 1]");
		questConfig.SetObjectiveText("Go to location A");
		array<string> desc = new array<string>;
		desc.Insert("This is a simple quest chain example quest where the player can process thrue a quests without visiting a quest NPC.");
		desc.Insert("Go to location A.");
		desc.Insert("You reached location A and now you can accept the next quest when you complete this one!");
		questConfig.SetDescriptions(desc);

		questConfig.AddQuestGiverID(1);
		questConfig.SetFollowUpQuestID(23);

		//! Quest objectives
		ExpansionQuestObjectiveTravelConfig objective_1 = new ExpansionQuestObjectiveTravelConfig();
		objective_1.SetID(5);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.TRAVEL);
		questConfig.AddObjectiveConfig(objective_1);

		return questConfig;
	}
	
	ExpansionQuestConfig ExpansionQuestConfig023()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(23);
		questConfig.SetTitle("Quest chain example [Part 2]");
		questConfig.SetObjectiveText("Go to location B");
		array<string> desc = new array<string>;
		desc.Insert("You can now always cancle this quest in the quest log and accept it there again if needed! You can now go to location B to contine the quest chain example.");
		desc.Insert("Go to location B.");
		desc.Insert("You reached location B and now you can accept the last quest when you complete this one!");
		questConfig.SetDescriptions(desc);

		questConfig.AddPreQuestID(22);
		questConfig.SetFollowUpQuestID(24);

		//! Quest objectives
		ExpansionQuestObjectiveTravelConfig objective_1 = new ExpansionQuestObjectiveTravelConfig();
		objective_1.SetID(6);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.TRAVEL);
		questConfig.AddObjectiveConfig(objective_1);

		return questConfig;
	}
	
	ExpansionQuestConfig ExpansionQuestConfig024()
	{
		ExpansionQuestConfig questConfig = new ExpansionQuestConfig();

		questConfig.SetID(24);
		questConfig.SetTitle("Quest chain example [Part 3]");
		questConfig.SetObjectiveText("PLACEHOLDER");
		array<string> desc = new array<string>;
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		desc.Insert("PLACEHOLDER");
		questConfig.SetDescriptions(desc);

		questConfig.AddQuestTurnInID(2);
		questConfig.AddPreQuestID(23);

		//! Quest objectives
		ExpansionQuestObjectiveTravelConfig objective_1 = new ExpansionQuestObjectiveTravelConfig();
		objective_1.SetID(7);
		objective_1.SetObjectiveType(ExpansionQuestObjectiveType.TRAVEL);
		questConfig.AddObjectiveConfig(objective_1);

		return questConfig;
	}
};