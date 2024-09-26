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
	protected ref ProjectileStoppedInfo m_Expansion_ProjectileStoppedInfo;

	override void SetFromProjectile(ProjectileStoppedInfo info)
	{
		super.SetFromProjectile(info);

		m_Expansion_ProjectileStoppedInfo = info;
	}
};

class Expansion_Ammo_BoltAnomaly_Base: Expansion_EffectBolt_Base
{
	override void EEInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.EEInit();

		InitBolt();
	}

	protected void InitBolt()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		//! Server only
		#ifdef SERVER
		InitBoltServer();
		#endif
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
	
	override void EEParentedTo(EntityAI parent)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::EEParentedTo - Parent: " + parent.ToString() + " | Position: " + parent.GetPosition());

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(HandleBoltParented, 100, false);
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::EEItemDetached - Item: " + item.ToString() + " | Slot:" + slot_name);
		
		//! Server only
		#ifdef SERVER
		Expansion_AnomalyCore_Base anomalyCore;
		if (Class.CastTo(anomalyCore, item))
		{
			HandleAnomalyCoreDetach();
		}
		#endif
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
		
		Rope rope = Rope.Cast(GetGame().CreateObject("Rope", Vector(pos[0] + offsetX, surfaceY, pos[2] + offsetY)));
		rope.SetHealth01("", "", resultHealth);
		
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
			
			boltMag.Update();
		}
	}
	
	//! Server
	protected void HandleBoltParented()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (GetGame().IsServer())
		{
			vector pos = GetPosition();
			vector ori = GetOrientation();
			
			//! Create anomaly core explosion
			Expansion_AnomalyCore_Base anomalyCore = Expansion_AnomalyCore_Base.Cast(GetGame().CreateObject(GetAmomalyCoreName(), pos));
			anomalyCore.SetPosition(pos);
			anomalyCore.SetHealth01("", "", 0);

			//! Create new bolt on impact position.
			Ammo_HuntingBolt newBolt = Ammo_HuntingBolt.Cast(GetGame().CreateObject("Ammo_HuntingBolt", pos));
			newBolt.SetOrientation(ori);
			newBolt.SetFromProjectile(m_Expansion_ProjectileStoppedInfo);
			
			//! Attach new bolt to parent
			Object arrowParent;
			if (Class.CastTo(arrowParent, GetParent()))
			{
				ExDebugPrint("::HandleBoltParented - Arrow parent: " + arrowParent.ToString());
				arrowParent.RemoveChild(this);  //! Don't think this is necessary because we call ObjectDelete later anyway - lava76
				ObjectCollisionInfo collisionInfo;
				if (Class.CastTo(collisionInfo, m_Expansion_ProjectileStoppedInfo))
					arrowParent.AddArrow(newBolt, collisionInfo.GetComponentIndex(), collisionInfo.GetHitObjPos(), collisionInfo.GetHitObjRot());
			}
			
			GetGame().ObjectDelete(this);
		}
	}

	string GetAmomalyCoreName()
	{
		return "";
	}
};

class Expansion_Ammo_BoltAnomaly_Ice: Expansion_Ammo_BoltAnomaly_Base
{
	override string GetAmomalyCoreName()
	{
		return "Expansion_AnomalyCore_Ice";
	}
};
class Expansion_Ammo_BoltAnomaly_Warper: Expansion_Ammo_BoltAnomaly_Base
{
	override string GetAmomalyCoreName()
	{
		return "Expansion_AnomalyCore_Warper";
	}
};
