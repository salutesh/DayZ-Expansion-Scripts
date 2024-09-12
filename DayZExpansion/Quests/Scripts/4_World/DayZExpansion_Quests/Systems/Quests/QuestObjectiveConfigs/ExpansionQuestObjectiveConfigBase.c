/**
 * ExpansionQuestObjectiveConfigBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveConfigBase
{
	int ConfigVersion;
	int ID = -1; //! Unique objective ID
	ExpansionQuestObjectiveType ObjectiveType = ExpansionQuestObjectiveType.NONE; //! Quest obecjtive type.
	
	void SetID(int id)
	{
		ID = id;
	}

	int GetID()
	{
		return ID;
	}

	void SetObjectiveType(int type)
	{
		ObjectiveType = type;
	}

	ExpansionQuestObjectiveType GetObjectiveType()
	{
		return ObjectiveType;
	}

	vector GetPosition()
	{
		return vector.Zero;
	}
};

class ExpansionQuestObjectiveConfig: ExpansionQuestObjectiveConfigBase
{
	[NonSerialized()]
	static const int CONFIGVERSION = 28;
	
	[NonSerialized()]
	static string FILENAME = "NONE";

	[NonSerialized()]
	protected ref set<typename> m_ObjectiveAllocationTypeNames;

	string ObjectiveText = string.Empty;
	int TimeLimit = - 1;
	bool Active = true;

	void ExpansionQuestObjectiveConfig()
	{
		ConfigVersion = CONFIGVERSION;
	}

	void SetObjectiveText(string text)
	{
		ObjectiveText = text;
	}

	string GetObjectiveText()
	{
		return ObjectiveText;
	}

	void SetTimeLimit(int time)
	{
		TimeLimit = time;
	}

	int GetTimeLimit()
	{
		return TimeLimit;
	}
	
	bool IsActive()
	{
		return Active;
	}

	void CollectAllocationClasses()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = CF_Trace_0(EXTrace.QUESTS, this);
	#endif

		int objectiveType = GetObjectiveType();
		switch (objectiveType)
		{
			case ExpansionQuestObjectiveType.ACTION:
			{
				AddAllocationTypeName(ActionBase);
			}
			break;
			case ExpansionQuestObjectiveType.CRAFTING:
			{
				AddAllocationTypeName(RecipeBase);
				AddAllocationTypeName(ItemBase);

				ExpansionQuestObjectiveCraftingConfig craftingConfig;
				if (Class.CastTo(craftingConfig, this))
				{
					foreach (string itemName: craftingConfig.ItemNames)
					{
						PlayerBase.Expansion_RegisterInventoryItemType(itemName);
					}
				}
			}
			break;
			case ExpansionQuestObjectiveType.TARGET:
			{
				ExpansionQuestObjectiveTargetConfig targetConfig;
				if (Class.CastTo(targetConfig, this))
				{
					TStringArray classNames = targetConfig.GetClassNames();
					foreach (string className: classNames)
					{
						AddAllocationTypeName(className);
					}
				}
			}
			break;
			case ExpansionQuestObjectiveType.DELIVERY:
			case ExpansionQuestObjectiveType.COLLECT:
			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				AddAllocationTypeName(ItemBase);

				ExpansionQuestObjectiveDeliveryConfigBase deliveryConfig;
				if (Class.CastTo(deliveryConfig, this))
				{
					array<ref ExpansionQuestObjectiveDelivery> collections = deliveryConfig.GetCollections();
					foreach (auto collection: collections)
					{
						PlayerBase.Expansion_RegisterInventoryItemType(collection.GetClassName());
					}
				}
			}
			break;
		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AICAMP:
			case ExpansionQuestObjectiveType.AIPATROL:
			{
				AddAllocationTypeName(PlayerBase);
			}
			break;
			case ExpansionQuestObjectiveType.AIESCORT:
			{
				AddAllocationTypeName(PlayerBase);
				AddAllocationTypeName(ActionBase);
			}
			break;
		#endif
		}
	}

	void AddAllocationTypeName(string className)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = CF_Trace_0(EXTrace.QUESTS, this);
	#endif
	
		typename typeName = className.ToType();
		EXTrace.Print(EXTrace.QUESTS, this, "::AddAllocationTypeName - className: " + className + " typeName: " + typeName);

		if (ExpansionStatic.Is(className, "ZombieBase"))
		{
			AddAllocationTypeName(ZombieBase);
		}
		else if (ExpansionStatic.Is(className, "AnimalBase"))
		{
			AddAllocationTypeName(AnimalBase);
		}
		else if (ExpansionStatic.IsPlayer(className))
		{
			AddAllocationTypeName(PlayerBase);
		}
		else if (ExpansionStatic.IsInventoryItem(className))
		{
			AddAllocationTypeName(ItemBase);
		}
		else if (typeName.IsInherited(ActionBase))
		{
			AddAllocationTypeName(ActionBase);
		}
		else if (typeName.IsInherited(RecipeBase))
		{
			AddAllocationTypeName(RecipeBase);
		}
	}

	void AddAllocationTypeName(typename typeName)
	{
		if (!m_ObjectiveAllocationTypeNames)
			m_ObjectiveAllocationTypeNames = new set<typename>;

		if (EXTrace.QUESTS && m_ObjectiveAllocationTypeNames.Find(typeName) == -1)
			EXTrace.Print(true, this, "::AddAllocationTypeName - typeName: " + typeName);

		m_ObjectiveAllocationTypeNames.Insert(typeName);
	}

	set<typename> GetAllocationTypeNames()
	{
		return m_ObjectiveAllocationTypeNames;
	}

	void Save(string fileName);

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(ID);
		ctx.Write(ObjectiveType);
		ctx.Write(ObjectiveText);
		ctx.Write(TimeLimit);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(ID))
			return false;

		if (!ctx.Read(ObjectiveType))
			return false;

		if (!ctx.Read(ObjectiveText))
			return false;

		if (!ctx.Read(TimeLimit))
			return false;

		return true;
	}

	bool Validate()
	{
		if (ConfigVersion != CONFIGVERSION)
			return false;

		if (ID <= -1)
			return false;

		if (ObjectiveType == ExpansionQuestObjectiveType.NONE)
			return false;

		return true;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - Objective config version: " + ConfigVersion);
		Print(ToString() + "::QuestDebug - Objective ID: " + ID);
		Print(ToString() + "::QuestDebug - ObjectiveType: " + ObjectiveType);
		Print(ToString() + "::QuestDebug - ObjectiveText: " + ObjectiveText);
		Print(ToString() + "::QuestDebug - TimeLimit: " + TimeLimit);
		Print("------------------------------------------------------------");
	#endif
	}
};