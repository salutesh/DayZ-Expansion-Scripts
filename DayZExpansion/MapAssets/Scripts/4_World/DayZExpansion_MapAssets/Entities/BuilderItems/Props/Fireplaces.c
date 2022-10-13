class bldr_prop_Fireplace extends House
{
	Particle m_ParticleEfx;
	protected EffectSound m_SoundFireLoop;
	protected FireplaceLight m_Light;
	
	void bldr_prop_Fireplace()
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SpawnParticles, 2000, false);
		}
	}
	
	void SpawnParticles()
	{
		m_ParticleEfx = Particle.PlayOnObject(ParticleList.CAMP_NORMAL_FIRE, this, Vector( 0, 0.05, 0 ));
		m_ParticleEfx = Particle.PlayOnObject(ParticleList.CAMP_NORMAL_SMOKE, this, Vector( 0, 0.05, 0 ));
		PlaySoundSetLoop( m_SoundFireLoop, "LightFire_SoundSet", 1.0, 1.0 );		
		SetLightEntity( FireplaceLight.Cast( ScriptedLightBase.CreateLight(FireplaceLight, GetPosition(), 20) ) );
		GetLightEntity().AttachOnMemoryPoint(this, "light");
	}
	
	void SetLightEntity( FireplaceLight light )
	{
		m_Light = light;
	}	
	FireplaceLight GetLightEntity()
	{
		return m_Light;
	}
	
}

class bldr_prop_FireplaceIndoor extends House
{
	Particle m_ParticleEfx;
	protected EffectSound m_SoundFireLoop;
	protected FireplaceLight m_Light;
	
	void bldr_prop_FireplaceIndoor()
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(SpawnParticles, 2000, false);
		}
	}
	
	void SpawnParticles()
	{
		m_ParticleEfx = Particle.PlayOnObject(ParticleList.HOUSE_NORMAL_FIRE, this, Vector( 0, 0.05, 0 ));
		//m_ParticleEfx = Particle.PlayOnObject(ParticleList.HOUSE_NORMAL_SMOKE, this, Vector( 0, 0.05, 0 ));
		PlaySoundSetLoop( m_SoundFireLoop, "LightFire_SoundSet", 1.0, 1.0 );		
		SetLightEntity( FireplaceLight.Cast( ScriptedLightBase.CreateLight(FireplaceLight, GetPosition(), 20) ) );
		GetLightEntity().AttachOnMemoryPoint(this, "light");
	}
	
	void SetLightEntity( FireplaceLight light )
	{
		m_Light = light;
	}	
	FireplaceLight GetLightEntity()
	{
		return m_Light;
	}
	
}

class bldr_fire_barrel extends BarrelHoles_Red
{
	protected bool m_bldr_Opened;

	override protected void SpendFireConsumable( float amount ) {};

	void bldr_fire_barrel()
	{
		SetAllowDamage(false);
	}

	override bool CanExtinguishFire()
	{
		return false;
	}

	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}

	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}

	override void SoundBarrelOpenPlay()
	{
		if (!m_bldr_Opened)
		{
			m_bldr_Opened = true;
			super.SoundBarrelOpenPlay();
		}
	}

	override void SoundBarrelClosePlay()
	{
		if (m_bldr_Opened)
		{
			m_bldr_Opened = false;
			super.SoundBarrelClosePlay();
		}
	}
}

class bldr_fireplace extends Fireplace
{
	override protected void SpendFireConsumable( float amount ) {};

	void bldr_fireplace()
	{
		SetAllowDamage(false);
	}

	override bool CanExtinguishFire()
	{
		return false;
	}

	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}

	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}
}

class bldr_Chemlight_ColorBase extends Chemlight_ColorBase
{
	void bldr_Chemlight_ColorBase()
	{
		SetAllowDamage(false);
		CreateLight();
	}
	
	override void OnEnergyConsumed() {};
	
	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}
	
	override bool CanPutAsAttachment( EntityAI parent )
	{
		return false;
	}
	
	override bool CanReceiveItemIntoCargo( EntityAI item )
	{
		return false;
	}
	
	override bool CanReceiveAttachment( EntityAI attachment, int slotId )
	{
		return false;
	}
	
	override bool CanLoadAttachment( EntityAI attachment )
	{		
		return false;
	}
	
	override bool CanBePlaced( Man player, vector position ) 
	{ 
		return false; 
	}
	
	override bool IsInventoryVisible()
	{
		return false;
	}
	
	override bool CanDetachAttachment(EntityAI parent)
	{
		return false;
	}
	
	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}

	override bool CanLoadItemIntoCargo( EntityAI item )
	{
		return false;
	}
	
	override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
	{
		return false;
	}
	
	override bool CanReleaseCargo(EntityAI cargo)
	{
		return false;
	}
	
	override bool CanRemoveFromCargo(EntityAI parent)
	{
		return false;
	}
	
	override bool CanReceiveItemIntoHands(EntityAI item_to_hands)
	{
		return false;
	}
	
	override bool CanSaveItemInHands(EntityAI item_in_hands)
	{
		return false;
	}
	
	override bool CanReleaseFromHands(EntityAI handheld)
	{
		return false;
	}

	override bool CanRemoveFromHands(EntityAI parent)
	{
		return false;
	}
	
	override bool CanDisplayAttachmentSlot(int slot_id)
	{
		return false;
	}
	
	override bool CanDisplayAttachmentCategory( string category_name )
	{
		return false;
	}
	
	override bool CanDisplayCargo()
	{
		return false;
	}
	
	override bool CanAssignToQuickbar()
	{
		return false;
	}
	
	override bool IsTakeable()
	{
		return false;
	}
	
	override void CreateLight()
	{
		SetObjectMaterial(0, GetMaterialForDamageState(true)); // must be server side!
		
		if (!GetGame().IsServer()  ||  !GetGame().IsMultiplayer()) // client side
		{
			m_Light = ChemlightLight.Cast(ScriptedLightBase.CreateLight( ChemlightLight, "0 0 0"));
			m_Light.AttachOnMemoryPoint(this, "light");
		
			string type = GetType();
			switch (type)
			{
				case "bldr_Chemlight_White": 
					m_Light.SetColorToWhite();
					break;
				case "bldr_Chemlight_Red": 
					m_Light.SetColorToRed();
					break;
				case "bldr_Chemlight_Green": 
					m_Light.SetColorToGreen();
					break;
				case "bldr_Chemlight_Blue": 
					m_Light.SetColorToBlue();
					break;
				case "bldr_Chemlight_Yellow": 
					m_Light.SetColorToYellow();
					break;
				default: {m_Light.SetColorToWhite();};
			}
		}
	}
}

class bldr_Chemlight_White extends bldr_Chemlight_ColorBase {};
class bldr_Chemlight_Red extends bldr_Chemlight_ColorBase {};
class bldr_Chemlight_Green extends bldr_Chemlight_ColorBase {};
class bldr_Chemlight_Blue extends bldr_Chemlight_ColorBase {};
class bldr_Chemlight_Yellow extends bldr_Chemlight_ColorBase {};

class bldr_PortableGasLamp_Prop extends PortableGasLamp
{
	void bldr_PortableGasLamp_Prop()
	{
		SetAllowDamage(false);

		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // client side
		{
			m_Light = PortableGasLampLight.Cast( ScriptedLightBase.CreateLight( PortableGasLampLight, "0 0 0") );
			m_Light.AttachOnMemoryPoint(this, "light");
		}
		
		//refresh visual
		SetObjectMaterial( 0, "dz\\gear\\cooking\\data\\GasLightOn.rvmat" );
		
		//sound (client only)
		SoundBurningStart();
	}

	override void OnEnergyConsumed() {};
	
	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}
	
	override bool CanPutAsAttachment( EntityAI parent )
	{
		return false;
	}
	
	override bool CanReceiveItemIntoCargo( EntityAI item )
	{
		return false;
	}
	
	override bool CanReceiveAttachment( EntityAI attachment, int slotId )
	{
		return false;
	}
	
	override bool CanLoadAttachment( EntityAI attachment )
	{		
		return false;
	}
	
	override bool CanBePlaced( Man player, vector position ) 
	{ 
		return false; 
	}
	
	override bool IsInventoryVisible()
	{
		return false;
	}
	
	override bool CanDetachAttachment(EntityAI parent)
	{
		return false;
	}
	
	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}

	override bool CanLoadItemIntoCargo( EntityAI item )
	{
		return false;
	}
	
	override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
	{
		return false;
	}
	
	override bool CanReleaseCargo(EntityAI cargo)
	{
		return false;
	}
	
	override bool CanRemoveFromCargo(EntityAI parent)
	{
		return false;
	}
	
	override bool CanReceiveItemIntoHands(EntityAI item_to_hands)
	{
		return false;
	}
	
	override bool CanSaveItemInHands(EntityAI item_in_hands)
	{
		return false;
	}
	
	override bool CanReleaseFromHands(EntityAI handheld)
	{
		return false;
	}

	override bool CanRemoveFromHands(EntityAI parent)
	{
		return false;
	}
	
	override bool CanDisplayAttachmentSlot(int slot_id)
	{
		return false;
	}
	
	override bool CanDisplayAttachmentCategory( string category_name )
	{
		return false;
	}
	
	override bool CanDisplayCargo()
	{
		return false;
	}
	
	override bool CanAssignToQuickbar()
	{
		return false;
	}
	
	override bool IsTakeable()
	{
		return false;
	}
}

class CandleLight extends PointLightBase
{
	void CandleLight()
	{
		SetVisibleDuringDaylight(false);
		SetRadiusTo(2.5);
		SetBrightnessTo(2.0);
		SetCastShadow(true);
		FadeIn(0.5);
		SetFadeOutTime(0.1);
		SetAmbientColor(1.0, 0.5, 0.3);
		SetDiffuseColor(1.0, 0.5, 0.3);
		SetFlickerAmplitude(0.3);
		SetFlickerSpeed(0.9);
		SetDancingShadowsMovementSpeed(0.010);
		SetDancingShadowsAmplitude(0.008);
	}
}

class bldr_Candle extends Candle
{
	CandleLight m_Light;
	
	void bldr_Candle()
	{
		SetAllowDamage(false);
		
		SetObjectMaterial( 0, "dz\gear\tools\data\candle_flame.rvmat" );
	}
	
	override void DeferredInit()
	{
		super.DeferredInit();

		CreateLight();
	}

	override void OnEnergyConsumed() {};
	
	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}
	
	override bool CanPutAsAttachment( EntityAI parent )
	{
		return false;
	}
	
	override bool CanReceiveItemIntoCargo( EntityAI item )
	{
		return false;
	}
	
	override bool CanReceiveAttachment( EntityAI attachment, int slotId )
	{
		return false;
	}
	
	override bool CanLoadAttachment( EntityAI attachment )
	{		
		return false;
	}
	
	override bool CanBePlaced( Man player, vector position ) 
	{ 
		return false; 
	}
	
	override bool IsInventoryVisible()
	{
		return false;
	}
	
	override bool CanDetachAttachment(EntityAI parent)
	{
		return false;
	}
	
	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}

	override bool CanLoadItemIntoCargo( EntityAI item )
	{
		return false;
	}
	
	override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
	{
		return false;
	}
	
	override bool CanReleaseCargo(EntityAI cargo)
	{
		return false;
	}
	
	override bool CanRemoveFromCargo(EntityAI parent)
	{
		return false;
	}
	
	override bool CanReceiveItemIntoHands(EntityAI item_to_hands)
	{
		return false;
	}
	
	override bool CanSaveItemInHands(EntityAI item_in_hands)
	{
		return false;
	}
	
	override bool CanReleaseFromHands(EntityAI handheld)
	{
		return false;
	}

	override bool CanRemoveFromHands(EntityAI parent)
	{
		return false;
	}
	
	override bool CanDisplayAttachmentSlot(int slot_id)
	{
		return false;
	}
	
	override bool CanDisplayAttachmentCategory( string category_name )
	{
		return false;
	}
	
	override bool CanDisplayCargo()
	{
		return false;
	}
	
	override bool CanAssignToQuickbar()
	{
		return false;
	}
	
	override bool IsTakeable()
	{
		return false;
	}
	
	void CreateLight()
	{		
		if (!GetGame().IsServer()  ||  !GetGame().IsMultiplayer()) // client side
		{
			m_Light = CandleLight.Cast(ScriptedLightBase.CreateLight( CandleLight, GetPosition() + "0 0.15 0"));
		}
	}
}

class SpotlightLightProp extends SpotLightBase
{
	void SpotlightLightProp()
	{
		SetVisibleDuringDaylight(false);
		SetRadiusTo(60);
		SetSpotLightAngle(140);
		SetCastShadow(true);
		SetBrightnessTo(20.0);
		SetFadeOutTime(0.3);
		FadeIn(0.2);
		SetAmbientColor(0.8, 0.9, 1.0);
		SetDiffuseColor(0.8, 0.9, 1.0);
	}
}

class bldr_Spotlight extends Spotlight
{
	SpotlightLightProp m_LightProp;
	
	void bldr_Spotlight()
	{
		SetAllowDamage(false);
		Unfold();
		
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // client side
		{
			m_LightProp = SpotlightLightProp.Cast( ScriptedLightBase.CreateLight( SpotlightLightProp, "0 0 0") );
			m_LightProp.AttachOnObject(this, m_LightLocalPosition, m_LightLocalOrientation);
		}
		
		UpdateAllSelections();
	}
	
	override void UpdateAllSelections()
	{
		HideAllSelections();
		
		ShowSelection( SEL_PLACING );
		ShowSelection( SEL_REFLECTOR_U );
		ShowSelection( SEL_GLASS_U );
		ShowSelection( SEL_REFLECTOR_COMP_U );
		ShowSelection( SEL_CORD_PLUGGED_U );
		SetObjectMaterial(ID_GLASS_UNFOLDED, LIGHT_ON_GLASS);
		SetObjectMaterial(ID_REFLECTOR_UNFOLDED, LIGHT_ON_REFLECTOR);
	}
	
	override void OnEnergyConsumed() {};
	
	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}

	override bool CanLoadItemIntoCargo( EntityAI item )
	{
		return false;
	}
	
	override bool CanReceiveItemIntoHands(EntityAI item_to_hands)
	{
		return false;
	}
	
	override bool CanReleaseFromHands(EntityAI handheld)
	{
		return false;
	}

	override bool CanRemoveFromHands(EntityAI parent)
	{
		return false;
	}
	
	override bool IsTakeable()
	{
		return false;
	}
	
	void CreateLight()
	{		
		if (!GetGame().IsServer()  ||  !GetGame().IsMultiplayer()) // client side
		{
			m_LightProp = SpotlightLightProp.Cast(ScriptedLightBase.CreateLight( SpotlightLightProp, "0 0 0"));
			m_LightProp.AttachOnObject(this, m_LightLocalPosition, m_LightLocalOrientation);
		}
	}
}

class bldr_land_misc_barel_fire_1 extends BuildingWithFireplace {};
class bldr_land_misc_barel_fire_2 extends BuildingWithFireplace {};
class bldr_land_misc_barel_fire_3 extends BuildingWithFireplace {};
class bldr_land_misc_barel_fire_4 extends BuildingWithFireplace {};