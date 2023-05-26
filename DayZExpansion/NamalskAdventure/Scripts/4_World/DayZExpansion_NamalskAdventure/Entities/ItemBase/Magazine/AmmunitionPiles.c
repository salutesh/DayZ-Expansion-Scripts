/**
 * AmmunitionPiles.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_EffectBolt_Base: Bolt_Base
{
	override void EEParentedTo(EntityAI parent)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::EEParentedTo - Parent: " + parent.ToString() + " | Position: " + parent.GetPosition());

		super.EEParentedTo(parent);
		
		ContactEvent(parent);
	}
	
	void ContactEvent(EntityAI parent);
	
	protected void ExDebugPrint(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};

class Expansion_Ammo_BoltAnomaly: Expansion_EffectBolt_Base
{
	protected bool m_ContactEventProcessing;

	override void EEInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.EEInit();

		InitBolt();
	}

	protected void InitBolt()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		//! Client only
		if (!GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			InitBoltClient();
		}
		
		//! Server only
		if (GetGame().IsServer())
		{
			InitBoltServer();
		}
	}
	
	//! Client
	protected void InitBoltClient()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	}
	
	//! Server
	protected void InitBoltServer()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		//! Spawn the core item into the anomaly.
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SpawnCoreItem, 500, false);
	}
	
	//! Server
	protected void SpawnCoreItem()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExpansionItemSpawnHelper.SpawnInInventory(GetAmomalyCoreName(), this);
	}
	
	//! Server
	override void ContactEvent(EntityAI parent)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::ContactEvent - Parent: " + parent.ToString() + " | Position: " + parent.GetPosition());

		if (GetGame().IsServer() && !m_ContactEventProcessing)
		{
			m_ContactEventProcessing = true;
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(HandleBoltContact, 100, false);
			m_ContactEventProcessing = false;
		}
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::EEItemDetached - Item: " + item.ToString() + " | Slot:" + slot_name);
		
		//! Server only
		if (GetGame().IsServer())
		{
			Expansion_AnomalyCore_Base anomalyCore;
			if (Class.CastTo(anomalyCore, item))
			{
				HandleAnomalyCoreDetach();
			}
		}
	}
	
	//! Server
	protected void HandleAnomalyCoreDetach()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		vector pos = GetPosition();
		float surfaceY = GetGame().SurfaceY(pos[0], pos[2]);
		float offsetX;
		float offsetY;

		float resultHealth = GetHealth01("", "");
		offsetX = Math.RandomFloatInclusive(0.2, 0.7);
		offsetY = Math.RandomFloatInclusive(0.2, 0.7);
		Ammo_HuntingBolt newBolt = Ammo_HuntingBolt.Cast(GetGame().CreateObject("Ammo_HuntingBolt", Vector(pos[0] + offsetX, surfaceY, pos[2] + offsetY)));
		newBolt.SetHealth01("", "", resultHealth);
		
		Magazine boltMag;
		Magazine thisMag;
		if (Class.CastTo(boltMag, newBolt) && Class.CastTo(thisMag, this))
		{
			string cartTypeName;
			float health;
			int ammoCount = thisMag.GetAmmoCount();
			boltMag.ServerSetAmmoCount(0);
			thisMag.GetCartridgeAtIndex(0, health, cartTypeName);
			boltMag.ServerStoreCartridge(health, cartTypeName);
		}
		
		newBolt.Update();
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(HandleDisassemble, 100, false);
	}
	
	//! Server
	protected void HandleDisassemble()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		Magazine boltMag;
		if (Class.CastTo(boltMag, this))
		{
			int ammoCount = boltMag.GetAmmoCount();
			int newAmmoCount = ammoCount - 1;
			if (newAmmoCount == 0)
			{
				GetGame().ObjectDelete(boltMag);
			}
			else
			{
				boltMag.ServerSetAmmoCount(newAmmoCount);
			}
		}
	}
	
	//! Server
	protected void HandleBoltContact()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (GetGame().IsServer())
		{
			float resultHealth = GetHealth01("", "");
			vector pos = GetPosition();
			vector ori = GetOrientation();
			
			//! Create anomaly core explosion
			Expansion_AnomalyCore_Base anomalyCore = Expansion_AnomalyCore_Base.Cast(GetGame().CreateObject(GetAmomalyCoreName(), GetPosition()));
			anomalyCore.SetPosition(pos);
			anomalyCore.SetHealth01("", "", 0);

			//! Create new bolt on impact position.
			Ammo_HuntingBolt newBolt = Ammo_HuntingBolt.Cast(GetGame().CreateObject("Ammo_HuntingBolt", GetPosition()));
			newBolt.SetHealth01("", "", resultHealth);
			newBolt.SetPosition(pos);
			newBolt.SetOrientation(ori);
			
			//! Handle ammo
			Magazine boltMag;
			Magazine thisMag;
			if (Class.CastTo(boltMag, newBolt) && Class.CastTo(thisMag, this))
			{
				string cartTypeName;
				float health;
				int ammoCount = thisMag.GetAmmoCount();
				boltMag.ServerSetAmmoCount(0);
				thisMag.GetCartridgeAtIndex(0, health, cartTypeName);
				boltMag.ServerStoreCartridge(health, cartTypeName);
			}
			
			//! Attach new bot to parent
			EntityAI arrowParent;
			if (Class.CastTo(arrowParent, GetParent()))
			{
				ExDebugPrint("::HandleBoltContact - Arrow parent: " + arrowParent.ToString());
				ArrowManagerBase arrowManager = arrowParent.GetArrowManager();
				if (arrowManager)
				{
					ExDebugPrint("::HandleBoltContact - Parent arrow manager: " + arrowManager.ToString());
					arrowManager.AddArrow(newBolt);
					arrowManager.RemoveArrow(this);	
					//arrowParent.Update();
				}
			}
			
			GetGame().ObjectDelete(this);
		}
	}

	string GetAmomalyCoreName()
	{
		return "Expansion_AnomalyCore_Ice";
	}
	
	override void AfterStoreLoad()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		super.AfterStoreLoad();
	}
};
