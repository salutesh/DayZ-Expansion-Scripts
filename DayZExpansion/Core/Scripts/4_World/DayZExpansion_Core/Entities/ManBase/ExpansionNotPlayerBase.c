/**
 * ExpansionNotPlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Ligtweight base class for NPCs etc without additional overhead from PlayerBase
//! Minimum amount of stuff mostly copied verbatim from vanilla PlayerBase to make character work properly visually (textures/clipping)
//! + ability to set actions (always as target, so no need for separate action maps like in PlayerBase)
class ExpansionNotPlayerBase: ManBase
{
	// 5		
	private int m_LifeSpanState;
	// 14
	protected bool m_PlayerLoaded;
	// 21
	protected int m_LifespanLevelLocal; //control variable for change calls
	// 58
	int m_FliesIndex;
	// 70
	ref EffectParticle m_FliesEff;
	// 71
	protected ref TInputActionMap m_InputActionMap;
	// 74
	bool m_ActionsInitialize;
	// 76
	ref HiddenSelectionsData m_EmptyGloves;
	// 85
	ref EffectSound m_SoundFliesEffect;
	// 97
	int m_CorpseState;
	protected int m_CorpseStateLocal;
	// 130
	Head_Default m_CharactersHead;
	// 135
	bool m_HideHairAnimated;
	// 136
	string m_DecayedTexture;

	void ExpansionNotPlayerBase()
	{
		Init();
	}

	// 295
	void Init()
	{
		m_EmptyGloves = new HiddenSelectionsData(ConfigGetString("emptyGloves"));

		RegisterNetSyncVariableInt("m_LifeSpanState", LifeSpanState.BEARD_NONE, LifeSpanState.COUNT);
		RegisterNetSyncVariableInt("m_CorpseState", -PlayerConstants.CORPSE_STATE_DECAYED, PlayerConstants.CORPSE_STATE_DECAYED);

		m_DecayedTexture = ConfigGetString("decayedTexture");

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(OnPlayerLoaded);
	}
	
	// 1137
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		ItemBase itemIB = ItemBase.Cast(item);
		SwitchItemSelectionTexture(item, slot_name);
		Param1<ExpansionNotPlayerBase> p = new Param1<ExpansionNotPlayerBase>(this);
		itemIB.Expansion_SwitchItemSelectionTextureEx(EItemManipulationContext.ATTACHING, p);
		/*
		m_QuickBarBase.updateSlotsCount();
		CalculateVisibilityForAI();
		*/
		UpdateShoulderProxyVisibility(item, slot_name);
		
		HideHairSelections(itemIB,true);
		
		g_Game.GetAnalyticsClient().OnItemAttachedAtPlayer(item, slot_name);
		Clothing clothing = Clothing.Cast(item);
		if (clothing)
		{
			if (!g_Game.IsDedicatedServer())
			{
				/*
				if (clothing.GetEffectWidgetTypes())
				{
					QueueAddEffectWidget(clothing.GetEffectWidgetTypes());
				}
				
				if (clothing.GetGlassesEffectID() > -1)
				{
					QueueAddGlassesEffect(clothing.GetGlassesEffectID());
				}
				*/
				
				g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(UpdateCorpseStateVisual, 200, false);//sometimes it takes a while to load in
				UpdateCorpseStateVisual();//....but if possible, we don't want a delay
			}
			/*
			else if (g_Game.IsServer())
			{
				if (clothing.IsGasMask())
				{
					GetModifiersManager().ActivateModifier(eModifiers.MDF_MASK);
				}
			}
			
			clothing.UpdateNVGStatus(this,true);
			*/
		}
		
		AdjustBandana(item,slot_name);
		AdjustShemag(item,slot_name);
	}
	
	// 1184
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		ItemBase item_base = ItemBase.Cast(item);
		SwitchItemSelectionTexture(item, slot_name);
		item.SwitchItemSelectionTextureEx(EItemManipulationContext.DETACHING);
		/*
		m_QuickBarBase.updateSlotsCount();
		CalculateVisibilityForAI();
		*/
		
		HideHairSelections(item_base,false);
		
		Clothing clothing = Clothing.Cast(item);
		
		if (clothing)
		{
			/*
			if (!g_Game.IsDedicatedServer())
			{
				if (clothing.GetEffectWidgetTypes())
				{
					QueueRemoveEffectWidget(clothing.GetEffectWidgetTypes());
				}
				
				if (clothing.GetGlassesEffectID() > -1)
				{
					QueueRemoveGlassesEffect(clothing.GetGlassesEffectID());
				}
			}
			
			if (g_Game.IsServer())
			{
				if (clothing.IsGasMask())
				{
					GetModifiersManager().DeactivateModifier(eModifiers.MDF_MASK);
				}
			}
			
			clothing.UpdateNVGStatus(this);
			*/
			g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(UpdateCorpseStateVisual, 200, false);//sometimes it takes a while to load in
			UpdateCorpseStateVisual();//....but if possible, we don't want a delay
		}
	}
	
	// 1252
	void UpdateShoulderProxyVisibility(EntityAI item, string slot_name)
	{
		string slot = slot_name;
		bool boo;
		boo = item.IsWeapon();
		
		if (slot == "Melee")
		{
			SetSimpleHiddenSelectionState(SIMPLE_SELECTION_MELEE_RIFLE,boo);
			SetSimpleHiddenSelectionState(SIMPLE_SELECTION_MELEE_MELEE,!boo);
		}
		else if (slot == "Shoulder")
		{
			SetSimpleHiddenSelectionState(SIMPLE_SELECTION_SHOULDER_RIFLE,boo);
			SetSimpleHiddenSelectionState(SIMPLE_SELECTION_SHOULDER_MELEE,!boo);
		}
	}
	
	// 1270
	override void SwitchItemSelectionTexture(EntityAI item, string slot_name)
	{
		super.SwitchItemSelectionTexture(item,slot_name);
		
		//shirt + armband
		ItemBase armband = ItemBase.Cast(FindAttachmentBySlotName("Armband"));
		if (slot_name == "Body" && armband)
		{
			Param1<ExpansionNotPlayerBase> p = new Param1<ExpansionNotPlayerBase>(this);
			armband.Expansion_SwitchItemSelectionTextureEx(EItemManipulationContext.UPDATE,p);
		}
	}
	
	// 1306
	bool IsMale()
	{
		if (ConfigGetBool("woman") != 1)
		{
			return true;
		}
		return false;
	}
	
	// 1350
	void SetActions()
	{
	}

	// 1395
	void InitializeActions()
	{
		m_InputActionMap = new TInputActionMap;
		SetActions();
	}

	// 1417
	override void GetActions(typename action_input_type, out array<ActionBase_Basic> actions)
	{
		if (!m_ActionsInitialize)
		{
			m_ActionsInitialize = true;
			InitializeActions();
		}
		
		actions = m_InputActionMap.Get(action_input_type);
	}

	// 1448
	void AddAction(typename actionName)
	{
		ActionBase action = ActionManagerBase.GetAction(actionName);

		typename ai = action.GetInputType();
		if (!ai)
		{
			m_ActionsInitialize = false;
			return;
		}

		array<ActionBase_Basic> action_array = m_InputActionMap.Get(ai);
		
		if (!action_array)
		{
			action_array = new array<ActionBase_Basic>;
			m_InputActionMap.Insert(ai, action_array);
		}

		action_array.Insert(action); 
	}

	// 1474
	void RemoveAction(typename actionName)
	{
		ActionBase action = ActionManagerBase.GetAction(actionName);
		typename ai = action.GetInputType();
		array<ActionBase_Basic> action_array = m_InputActionMap.Get(ai);
		
		if (action_array)
		{
			for (int i = 0; i < action_array.Count(); i++)
			{
				if (action == action_array.Get(i))
				{
					action_array.Remove(i);
				}
			}
			action_array = new array<ActionBase_Basic>;
			m_InputActionMap.Insert(ai, action_array);
		}
		action_array.Insert(action); 
	}

	// 2063
	void OnPlayerLoaded()
	{
		int slot_id = InventorySlots.GetSlotIdFromString("Head");
		m_CharactersHead = Head_Default.Cast(GetInventory().FindPlaceholderForSlot(slot_id));
		CheckHairClippingOnCharacterLoad();
		UpdateHairSelectionVisibility();
		PreloadDecayTexture();

		m_PlayerLoaded = true;
	}
	
	// 5577
	void UpdateCorpseState()
	{
		UpdateCorpseStateVisual();
		if (m_CorpseState > 0 && !GetIsFrozen())
			SetDecayEffects(Math.AbsInt(m_CorpseState));
		else
			SetDecayEffects();//no params means remove the effects
		m_CorpseStateLocal = m_CorpseState;
	}

	// 5588
	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);
		if (!g_Game.IsDedicatedServer())
		{
			if (m_CorpseState != 0)
			{
				g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(UpdateCorpseState, 0, false);
			}
		}
	}
	
	// 5607
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (m_LifespanLevelLocal != m_LifeSpanState) //client solution, lifespan level changed
		{
			m_LifespanLevelLocal = m_LifeSpanState;
			UpdateHairSelectionVisibility();
		}	

		if (m_CorpseStateLocal != m_CorpseState && (IsPlayerLoaded() || IsControlledPlayer()))
		{
			UpdateCorpseState();
		}
	}

	// 6569
	void SetLifeSpanStateVisible(int show_state)
	{
		bool state_changed;
		if (show_state != m_LifeSpanState)
			state_changed = true;
		m_LifeSpanState = show_state;
		SetSynchDirty();
		
		if (state_changed) //server only, client solution in OnVariablesSynchronized()
		{
			//SendLifespanSyncEvent(m_LifeSpanState);
			
			UpdateHairSelectionVisibility();
		}
	}
	
	// 6585
	int GetLifeSpanState()
	{
		return m_LifeSpanState;
	}
	
	// 8182
	bool IsPlayerLoaded()
	{
		return m_PlayerLoaded;
	}
	
	// 8479
	//! Dynamic hair hiding
	void SetHairLevelToHide(int level, bool state, bool was_debug = false)
	{
		if (was_debug && GetInstanceType() != DayZPlayerInstanceType.INSTANCETYPE_CLIENT)
			return;
		
		if (!m_CharactersHead)
		{
			ErrorEx("No valid head detected on character!");
			return;
		}
		
		if (level == -1) //hide/show ALL
		{
			m_HideHairAnimated = !m_HideHairAnimated;
			for (int i = 0; i < m_CharactersHead.m_HeadHairSelectionArray.Count(); ++i)
			{
				//m_CharactersHead.SetSimpleHiddenSelectionState(i,m_HideHairAnimated);
				SelectionTranslation stt = SelectionTranslation.Cast(m_CharactersHead.m_HeadHairHidingStateMap.Get(i));
				stt.SetSelectionState(m_HideHairAnimated);
				m_CharactersHead.m_HeadHairHidingStateMap.Set(i, stt);
#ifdef DIAG_DEVELOPER
#ifndef SERVER
				PluginDiagMenuClient diagmenu = PluginDiagMenuClient.Cast(GetPlugin(PluginDiagMenuClient));
				diagmenu.m_HairHidingStateMap.Set(i, m_HideHairAnimated);
#endif
#endif
			}
		}
		else //hide/show selected level only
		{
			bool switchState;
			if (was_debug)
			{
#ifdef DIAG_DEVELOPER
#ifndef SERVER
				PluginDiagMenuClient diagmenuu = PluginDiagMenuClient.Cast(GetPlugin(PluginDiagMenuClient));
				switchState = !diagmenuu.m_HairHidingStateMap.Get(level);
				diagmenuu.m_HairHidingStateMap.Set(level, switchState);
#endif
#endif
			}
			else
			{
				switchState = !state;
			}
			//m_CharactersHead.SetSimpleHiddenSelectionState(level,switchState);
			stt = SelectionTranslation.Cast(m_CharactersHead.m_HeadHairHidingStateMap.Get(level));
			stt.SetSelectionState(switchState);
			m_CharactersHead.m_HeadHairHidingStateMap.Set(level, stt); //nescessary?
		}
	}
	
	// 8531
	void HideHairSelections(ItemBase item, bool state)
	{
		if (!item || !item.GetHeadHidingSelection() || !m_CharactersHead)
			return;
		
		int slot_id; //item currently attached (or detaching from) here
		string slot_name; //item currently attached (or detaching from) here
		string str
		int idx = 0;
		int i;
		int count;
		item.GetInventory().GetCurrentAttachmentSlotInfo(slot_id,slot_name);
		
		if (item.HidesSelectionBySlot())
		{
			count = item.GetInventory().GetSlotIdCount();
			for (i = 0; i < count; i++)
			{
				if (item.GetInventory().GetSlotId(i) == slot_id)
				{
					str = item.GetHeadHidingSelection().Get(i);
					idx = m_CharactersHead.m_HeadHairSelectionArray.Find(str);
					if (idx != -1)
						SetHairLevelToHide(idx,state);
					#ifdef DEVELOPER
					else
						Debug.Log("No valid selection '" + str + "' found on head of " + GetType() + ". Verify the p3d, model config, and the 'HAIR_HIDING_SELECTIONS' macro in basicDefines.");
					#endif
				}
			}
		}
		else
		{
			count = item.GetHeadHidingSelection().Count();
			for (i = 0; i < count; i++)
			{
				str = item.GetHeadHidingSelection().Get(i);
				idx = m_CharactersHead.m_HeadHairSelectionArray.Find(str);
				if (idx != -1)
					SetHairLevelToHide(idx,state);
				#ifdef DEVELOPER
				else
					Debug.Log("No valid selection '" + str + "' found on head of " + GetType() + ". Verify the p3d, model config, and the 'HAIR_HIDING_SELECTIONS' macro in basicDefines.");
				#endif
			}
		}
		UpdateHairSelectionVisibility();
	}

	// 8580
	void UpdateHairSelectionVisibility(bool was_debug = false)
	{
		if (!m_CharactersHead)
			return;
		bool shown;
		bool exception_hidden = false;
		int i;
		int count = m_CharactersHead.m_HeadHairHidingStateMap.Count();
		SelectionTranslation stt;
		
		//hide/show beard
		if (IsMale() && m_CharactersHead.GetBeardIndex() > -1 && !was_debug)
		{
			SetHairLevelToHide(m_CharactersHead.GetBeardIndex(),GetLifeSpanState() != LifeSpanState.BEARD_EXTRA);
		}
		
		//show all first
		for (i = 0; i < count; i++)
		{
			m_CharactersHead.SetSimpleHiddenSelectionState(i,true);
		}
		//then carve it up
		for (i = 0; i < count; i++)
		{
			stt = m_CharactersHead.m_HeadHairHidingStateMap.Get(i);
			shown = stt.GetSelectionState();
			if (!shown)
			{
				if (/*IsMale() && */!m_CharactersHead.IsHandlingException())
				{
					m_CharactersHead.SetSimpleHiddenSelectionState(i,shown);
					UpdateTranslatedSelections(stt);
					//Print("hidden idx: " + i);
				}
				else
				{
					exception_hidden = true;
				}
			}
		}
		
		//exceptions handled differently; hides hair
		if (exception_hidden)
		{
			m_CharactersHead.SetSimpleHiddenSelectionState(m_CharactersHead.GetHairIndex(),false);
			if (IsMale())
				m_CharactersHead.SetSimpleHiddenSelectionState(m_CharactersHead.GetBeardIndex(),false);
		}
	}
	
	// 8630
	void UpdateTranslatedSelections(SelectionTranslation stt)
	{
		int index;
		array<int> translatedSelectinosArray = stt.GetTranslatedSelections();
		for (int i = 0; i < translatedSelectinosArray.Count(); i++)
		{
			index = translatedSelectinosArray.Get(i);
			//if (index > -1)
				m_CharactersHead.SetSimpleHiddenSelectionState(index,false); //safe this way, only hiding/carving from shown parts
		}
	}
	
	// 8643
	//! helper method for re-checking hairhiding on character load
	void CheckHairClippingOnCharacterLoad()
	{
		ItemBase headgear = ItemBase.Cast(GetInventory().FindAttachment(InventorySlots.HEADGEAR));
		ItemBase mask = ItemBase.Cast(GetInventory().FindAttachment(InventorySlots.MASK));
		
		HideHairSelections(headgear,true);
		HideHairSelections(mask,true);
	}
	
	// 8766
	void AdjustBandana(EntityAI item, string slot_name)
	{
		if (Bandana_ColorBase.Cast(item))
		{
			if (slot_name == "Headgear")
			{
				item.SetSimpleHiddenSelectionState(0,1);
				item.SetSimpleHiddenSelectionState(1,0);
			}
			else if (slot_name == "Mask")
			{
				item.SetSimpleHiddenSelectionState(0,0);
				item.SetSimpleHiddenSelectionState(1,1);
			}
		}
	}
	
	void AdjustShemag(EntityAI item, string slot_name)
	{
		if (Shemag_ColorBase.Cast(item))
		{
			if (slot_name == "Headgear")
			{
				item.SetSimpleHiddenSelectionState(0,1);
				item.SetSimpleHiddenSelectionState(1,0);
			}
			else if (slot_name == "Mask")
			{
				item.SetSimpleHiddenSelectionState(0,0);
				item.SetSimpleHiddenSelectionState(1,1);
			}
		}
	}
	
	// 8784
	// client-side
	void UpdateCorpseStateVisual()
	{
		//Print("---Prettying up corpses... | " + g_Game.GetTime() + " | " + this + " | " + GetType() + "---");
		//Print("m_DecayedTexture = " + m_DecayedTexture);
		int state = Math.AbsInt(m_CorpseState);//negative sign denotes a special meaning(state was forced to a live player), but we are only intetested in the positive value here
		if (state == PlayerConstants.CORPSE_STATE_DECAYED)
		{
			EntityAI bodypart;
			ItemBase item;
			
			string path;
			int idx;
			int slot_id;
			array<string> bodyparts = {"Gloves","Body","Legs","Feet"};
			
			for (int i = 0; i < bodyparts.Count(); i++)
			{
				slot_id = InventorySlots.GetSlotIdFromString(bodyparts.Get(i));
				bodypart = GetInventory().FindPlaceholderForSlot(slot_id);
				item = ItemBase.Cast(GetInventory().FindAttachment(slot_id));
				
				if (bodypart)
				{
					path = "cfgVehicles " + bodypart.GetType();
					idx = bodypart.GetHiddenSelectionIndex("personality");
					if (idx > -1)
					{
						bodypart.SetObjectTexture(idx,m_DecayedTexture);
					}
				}
				if (item)
				{
					path = "cfgVehicles " + item.GetType();
					idx = item.GetHiddenSelectionIndex("personality");
					if (idx > -1)
					{
						item.SetObjectTexture(idx,m_DecayedTexture);
					}
				}
			}
			
			SetFaceTexture(m_DecayedTexture);
		}
	}
	
	// 8829
	void SetDecayEffects(int effect = -1)
	{
		int boneIdx = GetBoneIndexByName("Spine2");
		Particle p;
		
		switch (effect)
		{
			case PlayerConstants.CORPSE_STATE_MEDIUM :
				//play medium sound/flies particle
				if (!m_FliesEff)
					m_FliesEff = new EffSwarmingFlies();
				
				if (m_FliesEff && !SEffectManager.IsEffectExist(m_FliesIndex))
				{
					m_FliesEff.SetDecalOwner(this);
					m_FliesIndex = SEffectManager.PlayOnObject(m_FliesEff, this, "0 0.25 0");
					p = m_FliesEff.GetParticle();
					AddChild(p, boneIdx);
					if (!m_SoundFliesEffect)
					{
						PlaySoundSetLoop(m_SoundFliesEffect, "Flies_SoundSet", 1.0, 1.0);
//						ErrorEx("DbgFlies | CORPSE_STATE_MEDIUM | m_SoundFliesEffect created! " + m_SoundFliesEffect,ErrorExSeverity.INFO);
					}
				}
			break;
			case PlayerConstants.CORPSE_STATE_DECAYED :
				//play serious sound/flies particle
				if (!m_FliesEff)
					m_FliesEff = new EffSwarmingFlies();
				
				if (m_FliesEff && !SEffectManager.IsEffectExist(m_FliesIndex))
				{
					m_FliesEff.SetDecalOwner(this);
					m_FliesIndex = SEffectManager.PlayOnObject(m_FliesEff, this, "0 0.25 0");
					p = m_FliesEff.GetParticle();
					AddChild(p, boneIdx);
					if (!m_SoundFliesEffect)
					{
						PlaySoundSetLoop(m_SoundFliesEffect, "Flies_SoundSet", 1.0, 1.0);
//						ErrorEx("DbgFlies | CORPSE_STATE_DECAYED | m_SoundFliesEffect created! " + m_SoundFliesEffect,ErrorExSeverity.INFO);
					}
				}
			break;
			//remove
			default:
				SEffectManager.DestroyEffect(m_FliesEff);
//				ErrorEx("DbgFlies | StopSoundSet | exit 3 ",ErrorExSeverity.INFO);
				StopSoundSet(m_SoundFliesEffect);
			break;
		}
	}
	
	// 8881
	void PreloadDecayTexture()
	{
		int idx;
		int slot_id;
		
		idx = GetHiddenSelectionIndex("decay_preload");
		if (idx > -1)
		{
			SetObjectTexture(idx,m_DecayedTexture);
			//Print("'decay_preload'" + m_DecayedTexture +" loaded on " + GetModelName());
		}
		else
		{
			//Print("No 'decay_preload' selection found on " + this);
		}
	}
}
