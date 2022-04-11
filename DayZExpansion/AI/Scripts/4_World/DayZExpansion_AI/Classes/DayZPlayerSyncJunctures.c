/*
modded class DayZPlayerSyncJunctures
{
	static void SendDeath(DayZPlayer pPlayer, int pType, float pHitDir)
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);
		
		ctx.Write(pType);
		ctx.Write(pHitDir);
		ctx.Send(DayZPlayerSyncJunctures.SJ_DEATH);
	}

	static void SendDamageHit(DayZPlayer pPlayer, int pType, float pHitDir, bool pFullbody)
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);
		
		ctx.Write(pType);
		ctx.Write(pHitDir);
		ctx.Write(pFullbody);
		ctx.Send(DayZPlayerSyncJunctures.SJ_DAMAGE_HIT);
	}
	
	static void SendDamageHitEx(DayZPlayer pPlayer, int pType, float pHitDir, bool pFullbody, TotalDamageResult pDamageResult, int pDamageType, EntityAI pSource, string pComponent, string pAmmoType, vector pModelPos)
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);
		SyncHitInfo data = new SyncHitInfo;
		
		data.m_AnimType = pType;
		data.m_HitDir = pHitDir;
		data.m_Fullbody = pFullbody;
		data.m_HasSource = pSource != null;
		if ( !pDamageResult )
		{
			data.m_HealthDamage = -1.0;
		}
		else
		{
			data.m_HealthDamage = pDamageResult.GetHighestDamage("Health");
		}
		
		ctx.Write(data);
		ctx.Send(DayZPlayerSyncJunctures.SJ_DAMAGE_HIT);
	}
	
	static void SendInjury(DayZPlayer pPlayer, bool pEnable, eInjuryHandlerLevels level)
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);
		ctx.Write(pEnable);
		ctx.Write(level);

		ctx.Send(DayZPlayerSyncJunctures.SJ_INJURY);
	}
	
	static void SendPlayerUnconsciousness(DayZPlayer pPlayer, bool enable)
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);

		ctx.Write(enable);
		
		ctx.Send(DayZPlayerSyncJunctures.SJ_UNCONSCIOUSNESS);
	}
	
	static void SendPlayerFBModifier(PlayerBase pPlayer, int type)
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);
		ctx.Write(type);
		
		ctx.Send(DayZPlayerSyncJunctures.SJ_PLAYER_FB_MODIFIER);
	}
	
	static void SendPlayerSymptomADD(DayZPlayer pPlayer, int type, int state_type)
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);
		ctx.Write(state_type);
		ctx.Write(type);


		ctx.Send(DayZPlayerSyncJunctures.SJ_PLAYER_ADD_MODIFIER);
	}
	
	static void SendPlayerSymptomFB(DayZPlayer pPlayer, DayZPlayerConstants anim_id, int state_type, int stance_mask, float duration)
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);
		ctx.Write(state_type);
		ctx.Write(anim_id);
		ctx.Write(stance_mask);
		ctx.Write(duration);
		//ctx.Write(pPlayer);
		
		ctx.Send(DayZPlayerSyncJunctures.SJ_PLAYER_STATES);
	}
	
	static void SendActionInterrupt(DayZPlayer pPlayer)
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);
		ctx.Send(DayZPlayerSyncJunctures.SJ_ACTION_INTERRUPT);
	}

	static void SendActionAcknowledgment(DayZPlayer pPlayer, int AckID, bool accept)
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);
		ctx.Write(AckID);
		if (accept)
			ctx.Send(DayZPlayerSyncJunctures.SJ_ACTION_ACK_ACCEPT);
		else
			ctx.Send(DayZPlayerSyncJunctures.SJ_ACTION_ACK_REJECT);
	}
	
	static void SendWeaponActionAcknowledgment(DayZPlayer pPlayer, int AckID, bool accept)
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);
		ctx.Write(AckID);
		if (accept)
			ctx.Send(DayZPlayerSyncJunctures.SJ_WEAPON_ACTION_ACK_ACCEPT);
		else
			ctx.Send(DayZPlayerSyncJunctures.SJ_WEAPON_ACTION_ACK_REJECT);
	}
	
	static void SendKuruRequest(DayZPlayer pPlayer, float amount)
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);
		ctx.Write(amount);
		ctx.Send(DayZPlayerSyncJunctures.SJ_KURU_REQUEST);
	}
	
	static void SendQuickbarSetShortcut(DayZPlayer pPlayer, EntityAI item, int index, bool force = false )
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);
		ctx.Write(item);
		ctx.Write(index);
		ctx.Write(force);

		ctx.Send(DayZPlayerSyncJunctures.SJ_QUICKBAR_SET_SHORTCUT);
	}
	
	static void SendWeaponJamChance(DayZPlayer pPlayer, float jamChance )
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);
		ctx.Write(jamChance);

		ctx.Send(DayZPlayerSyncJunctures.SJ_WEAPON_SET_JAMMING_CHANCE);
	}
	
	static void SendDeleteItem( DayZPlayer pPlayer, EntityAI item )
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);
		ctx.Write(item);

		ctx.Send(DayZPlayerSyncJunctures.SJ_DELETE_ITEM);
	}

	static void SendBrokenLegs(DayZPlayer pPlayer, bool canPlaySound, eBrokenLegs currentState, eBrokenLegs localState)
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);
		ctx.Write(canPlaySound);
		ctx.Write(currentState);
		ctx.Write(localState);

		ctx.Send(DayZPlayerSyncJunctures.SJ_BROKEN_LEGS);
	}
	
	static void SendBrokenLegsEx(DayZPlayer pPlayer, int currentState)
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);
		ctx.Write(currentState);

		ctx.Send(DayZPlayerSyncJunctures.SJ_BROKEN_LEGS);
	}
			
	static void SendShock(DayZPlayer pPlayer, float shockValue)
	{
		auto ctx = new eAIScriptJunctureData(pPlayer);
		ctx.Write(shockValue);

		ctx.Send(DayZPlayerSyncJunctures.SJ_SHOCK);
	}
};
*/
