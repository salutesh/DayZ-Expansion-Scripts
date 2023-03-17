/**
 * ActionSkinning.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSkinningData
{
	string ClassName;
	int Count;
	ref array<float> QuantityMinMax = new array<float>;
	float Quantity;
	int ToolDamageCoef;
	
	void ExpansionSkinningData(string name, int count, array<float> quantityMinMax, float quantity = 0, int toolDamageCoef = 0)
	{
		ClassName = name;
		Count = count;
		QuantityMinMax = quantityMinMax;
		Quantity = quantity;
		ToolDamageCoef = toolDamageCoef;
	}
};

modded class ActionSkinning
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		EntityAI target_entity = EntityAI.Cast(action_data.m_Target.GetObject());
		if (target_entity.IsInherited(ZombieBase))
		{
			array<ref ExpansionSkinningData> skinningDatas = new array<ref ExpansionSkinningData>;
			skinningDatas.Insert(new ExpansionSkinningData("InfectedSteakMeat", 1, {0.5,1}));
			skinningDatas.Insert(new ExpansionSkinningData("Guts", 1, {0.5,0.80000001}));
			skinningDatas.Insert(new ExpansionSkinningData("Lard", 1, {0.5,1}));
			skinningDatas.Insert(new ExpansionSkinningData("Bone", 1, {0.35,0.75}));
			
			ZombieBase infected = ZombieBase.Cast(target_entity);
			vector body_pos = infected.GetPosition();
			string item_to_spawn;
			string cfg_skinning_organ_class;
			array<EntityAI> children = new array<EntityAI>;
			infected.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, children);
			int count = children.Count();
			for (int i = 0; i < count; ++i)
			{
				EntityAI child = children.Get(i);
				if (child)
				{
					infected.GetInventory().DropEntity(InventoryMode.SERVER, infected, child);
					child.SetLifetime(3600);
				}
			}
			
			GetGame().ObjectDelete(infected);

			string cfg_animal_class_path = "cfgVehicles " + infected.GetType() + " " + "Skinning ";
			
			// Parsing of the 'Skinning' class in the config of the dead body
			for (int i1 = 0; i1 < skinningDatas.Count(); i1++)
			{
				ExpansionSkinningData skinningData = skinningDatas[i1];
				item_to_spawn = skinningData.ClassName;	

				if (item_to_spawn != "") // Makes sure to ignore incompatible parameters in the Skinning class of the agent
				{
					// Spawning items in action_data.m_Player's inventory
					int item_count = skinningData.Count;
						
					for (int i2 = 0; i2 < item_count; i2++)
					{
						ItemBase spawn_result = Expansion_CreateOrgan(action_data.m_Player, body_pos, item_to_spawn, skinningData, action_data.m_MainItem);
						// handle fat/guts from human bodies
						if ((item_to_spawn == "Lard") || (item_to_spawn == "Guts"))
						{
							spawn_result.InsertAgent(eAgents.BRAIN, 1);
						}
					}
				}
			}
			
			MiscGameplayFunctions.DealAbsoluteDmg(action_data.m_MainItem, UADamageApplied.SKINNING);
			PluginLifespan module_lifespan = PluginLifespan.Cast(GetPlugin(PluginLifespan));
			module_lifespan.UpdateBloodyHandsVisibility(action_data.m_Player, true);
			action_data.m_Player.GetSoftSkillsManager().AddSpecialty(m_SpecialtyWeight);
		}
		else
		{
			super.OnFinishProgressServer(action_data);
		}
	}
	
	ItemBase Expansion_CreateOrgan( PlayerBase player, vector body_pos, string item_to_spawn, ExpansionSkinningData skinningData, ItemBase tool)
	{
		// Create item from config
		ItemBase added_item;
		vector pos_rnd = body_pos + Vector(Math.RandomFloat01() - 0.5, 0, Math.RandomFloat01() - 0.5);
		Class.CastTo(added_item,  GetGame().CreateObjectEx( item_to_spawn, pos_rnd, ECE_PLACE_ON_SURFACE ) );
		
		// Check if skinning is configured for this body
		if (!added_item)
			return NULL;
		
		// Set randomized position
		added_item.PlaceOnSurface();
		
		// Set item's quantity from config, if it's defined there.
		float item_quantity = 0;
		array<float> quant_min_max = new array<float>;
		array<float> quant_min_max_coef = new array<float>;
		
		quant_min_max = skinningData.QuantityMinMax;
		quant_min_max_coef = skinningData.QuantityMinMax;

		// Read config for quantity value
		if (quant_min_max.Count() > 0)
		{
			float soft_skill_manipulated_value = ( quant_min_max.Get(0)+ quant_min_max.Get(1) ) / 2;
			float min_quantity = player.GetSoftSkillsManager().AddSpecialtyBonus( soft_skill_manipulated_value, this.GetSpecialtyWeight() );
			item_quantity = Math.RandomFloat(min_quantity, quant_min_max.Get(1));
		}
		
		if (quant_min_max_coef.Count() > 0)
		{
			float coef = Math.RandomFloat(quant_min_max_coef.Get(0), quant_min_max_coef.Get(1));
			item_quantity = added_item.GetQuantityMax() * coef;
		}
		
		if ( skinningData.Quantity > 0 )
			item_quantity = skinningData.Quantity;
		
		if ( item_quantity > 0 )
		{
			item_quantity = Math.Round(item_quantity);
			added_item.SetQuantity( item_quantity, false );
		}
		
		// Transfer tool's damage to the item's condition
		float item_apply_tool_damage_coef = skinningData.ToolDamageCoef;
		
		if ( item_apply_tool_damage_coef > 0 )
		{
			float tool_dmg_coef = 1 - tool.GetHealth01();
			float organ_dmg_coef = tool_dmg_coef * item_apply_tool_damage_coef;
			added_item.DecreaseHealthCoef( organ_dmg_coef );
		}
		
		added_item.InsertAgent(eAgents.SALMONELLA, 1);
		return added_item;
	}
}