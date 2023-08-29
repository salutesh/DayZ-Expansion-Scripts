/**
 * ExpansionGarageSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionGarageMode
{
	Territory = 0,
	Personal = 1
};

enum ExpansionGarageStoreMode
{
	Personal = 0,
	TerritoryShared = 1,
	TerritoryTyrannical = 2
};

enum ExpansionGarageRetrieveMode
{
	Personal = 0,
	TerritoryShared = 1,
	TerritoryTyrannical = 2
};

enum ExpansionGarageGroupStoreMode
{
	StoreOnly = 0,
	RetrieveOnly = 1,
	StoreAndRetrieve = 2
};

class ExpansionGarageSettings: ExpansionSettingBase
{
	static const int VERSION = 6;

	bool Enabled;
	bool AllowStoringDEVehicles;  //! Allow storing of vehicles spawned via dynamic events (events.xml)
	int GarageMode; //!0 - Territory Mode. The player needs to be a member of a territory to store/retrive vehicles. | 1 - Personal Mode. Can store/retrive vehicles without any requirement.
	int GarageStoreMode; //! 0 - Personal mode. Player can store vehicles where he was the last driver | 1 - Shared mode. All territory members can store a vehicles where the last vehicle driver was a territory member. | 2 - Only the territory owner and moderators can store vehicles of territory members.
	int GarageRetrieveMode; //! 0 - Personal mode. Only the player who stored the vehicle can retrieve it. | 1 - Shared mode. All vehicles can be retrieved by all territory members. | 2 - Tyranical mode. Only the territory owner and moderators can retrieve vehicles.
	int MaxStorableVehicles;
	float VehicleSearchRadius; //! Max search radius for vehicles from a garage access point.
	float MaxDistanceFromStoredPosition; //! Max distance a player can have from a position where a vehicle has been stored to retrieve it again.
	bool CanStoreWithCargo; //! Can store vehicles with cargo?
	bool UseVirtualStorageForCargo; //! Whether to use virtual storage for cargo or transfer cargo to/from placeholder
	bool NeedKeyToStore; //! A a car key need to be paired to the vehicle to store it.
	autoptr TStringArray EntityWhitelist;
#ifdef EXPANSIONMODGROUPS
	bool EnableGroupFeatures;  //! Enable/Disable garage group mod features.
	int GroupStoreMode; //! 0 - Group members can only store vehicles of other group members. | 1 - Group members can only retrieve vehicles of other group members. | 2 - Group members can store and retrieve vehicles of other group members.
#endif
#ifdef EXPANSIONMODMARKET
	bool EnableMarketFeatures; //! Enable/Disable garage market mod features.
	float StorePricePercent; //! If StorePricePercent is higer then 0 StaticStorePrice is set to 0 this value will be used for the price calculation. The base price is the market item max. price for the vehicle.
	int StaticStorePrice; //! If static price is higher then 0 and StorePricePercent is set to 0 this value will be used for the storing price for all vehicles.
#endif
	
#ifdef EXPANSIONMODBASEBUILDING
	int MaxStorableTier1;
	int MaxStorableTier2;
	int MaxStorableTier3;
	float MaxRangeTier1;
	float MaxRangeTier2;
	float MaxRangeTier3;
	bool ParkingMeterEnableFlavor;
#endif

	[NonSerialized()]
	bool m_IsLoaded;
	
	void ExpansionGarageSettings()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "ExpansionGarageSettings");
	#endif
		
		EntityWhitelist = new TStringArray;
	}
	
	override bool OnRecieve( ParamsReadContext ctx )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
	#endif

		ctx.Read(Enabled);

		ctx.Read(GarageMode);
		ctx.Read(GarageStoreMode);
		ctx.Read(GarageRetrieveMode);
		ctx.Read(MaxStorableVehicles);
		ctx.Read(VehicleSearchRadius);
		ctx.Read(MaxDistanceFromStoredPosition);
		ctx.Read(CanStoreWithCargo);
		ctx.Read(NeedKeyToStore);
		ctx.Read(EntityWhitelist);
		
	#ifdef EXPANSIONMODGROUPS
		ctx.Read(EnableGroupFeatures);
		ctx.Read(GroupStoreMode);
	#endif
		
	#ifdef EXPANSIONMODMARKET
		ctx.Read(EnableMarketFeatures);
		ctx.Read(StorePricePercent);
		ctx.Read(StaticStorePrice);
	#endif

	#ifdef EXPANSIONMODBASEBUILDING
		ctx.Read(MaxStorableTier1);
		ctx.Read(MaxStorableTier2);
		ctx.Read(MaxStorableTier3);
		ctx.Read(MaxRangeTier1);
		ctx.Read(MaxRangeTier2);
		ctx.Read(MaxRangeTier3);
	#endif
		
		m_IsLoaded = true;

		ExpansionSettings.SI_Garage.Invoke();

		return true;
	}

	override void OnSend( ParamsWriteContext ctx )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnSend").Add(ctx);
	#endif

		ctx.Write(Enabled);

		ctx.Write(GarageMode);
		ctx.Write(GarageStoreMode);
		ctx.Write(GarageRetrieveMode);
		ctx.Write(MaxStorableVehicles);
		ctx.Write(VehicleSearchRadius);
		ctx.Write(MaxDistanceFromStoredPosition);
		ctx.Write(CanStoreWithCargo);
		ctx.Write(NeedKeyToStore);
		ctx.Write(EntityWhitelist);

	#ifdef EXPANSIONMODGROUPS
		ctx.Write(EnableGroupFeatures);
		ctx.Write(GroupStoreMode);
	#endif
		
	#ifdef EXPANSIONMODMARKET
		ctx.Write(EnableMarketFeatures);
		ctx.Write(StorePricePercent);
		ctx.Write(StaticStorePrice);
	#endif
		
	#ifdef EXPANSIONMODBASEBUILDING
		ctx.Write(MaxStorableTier1);
		ctx.Write(MaxStorableTier2);
		ctx.Write(MaxStorableTier3);
		ctx.Write(MaxRangeTier1);
		ctx.Write(MaxRangeTier2);
		ctx.Write(MaxRangeTier3);
	#endif
	}

	override int Send( PlayerIdentity identity )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Send").Add(identity);
	#endif

		if ( !IsMissionHost() )
		{
			return 0;
		}

		auto rpc = ExpansionScriptRPC.Create();
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.Garage, true, identity );

		return 0;
	}

	override bool Copy( ExpansionSettingBase setting )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Copy").Add(setting);
	#endif

		ExpansionGarageSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}
	
	void CopyInternal(ExpansionGarageSettings s)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
	#endif

		GarageMode = s.GarageMode;
		GarageStoreMode = s.GarageStoreMode;
		GarageRetrieveMode = s.GarageRetrieveMode;
		MaxStorableVehicles = s.MaxStorableVehicles;
		VehicleSearchRadius = s.VehicleSearchRadius;
		MaxDistanceFromStoredPosition = s.MaxDistanceFromStoredPosition;
		CanStoreWithCargo = s.CanStoreWithCargo;
		NeedKeyToStore = s.NeedKeyToStore;
		EntityWhitelist.Copy(s.EntityWhitelist);
	
	#ifdef EXPANSIONMODGROUPS
		EnableGroupFeatures = s.EnableGroupFeatures;
		GroupStoreMode = s.GroupStoreMode;
	#endif	
		
	#ifdef EXPANSIONMODMARKET
		EnableMarketFeatures = s.EnableMarketFeatures;
		StorePricePercent = s.StorePricePercent;
		StaticStorePrice = s.StaticStorePrice;
	#endif
		
	#ifdef EXPANSIONMODBASEBUILDING
		MaxStorableTier1 = s.MaxStorableTier1;
		MaxStorableTier2 = s.MaxStorableTier2;
		MaxStorableTier3 = s.MaxStorableTier3;
		MaxRangeTier1 = s.MaxRangeTier1;
		MaxRangeTier2 = s.MaxRangeTier2;
		MaxRangeTier3 = s.MaxRangeTier3;
	#endif
	}

	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	override void Unload()
	{
		m_IsLoaded = false;
	}

	override bool OnLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoad");
#endif

		m_IsLoaded = true;

		bool save;
		bool garageSettingsExist = FileExist(EXPANSION_GARAGE_SETTINGS);
		if (garageSettingsExist)
		{
			JsonFileLoader<ExpansionGarageSettings>.JsonLoadFile(EXPANSION_GARAGE_SETTINGS, this);
			if (m_Version < VERSION)
			{
				EXPrint("[ExpansionGarageSetting] Load - Converting v" + m_Version + " \"" + EXPANSION_GARAGE_SETTINGS + "\" to v" + VERSION);

				ExpansionGarageSettings defaultSettings = new ExpansionGarageSettings;
				defaultSettings.Defaults();
				
			#ifdef EXPANSIONMODMARKET
				if (m_Version < 1)
					StorePricePercent *= 100;
			#endif
				
			#ifdef EXPANSIONMODBASEBUILDING
				if (m_Version < 2)
				{					
					MaxStorableTier1 = defaultSettings.MaxStorableTier1;
					MaxStorableTier2 = defaultSettings.MaxStorableTier2;
					MaxStorableTier3 = defaultSettings.MaxStorableTier3;
					MaxRangeTier1 = defaultSettings.MaxRangeTier1;
					MaxRangeTier2 = defaultSettings.MaxRangeTier2;
					MaxRangeTier3 = defaultSettings.MaxRangeTier3;
				}
			#endif
				
				if (m_Version < 3)
					Enabled = defaultSettings.Enabled;

			#ifdef EXPANSIONMODBASEBUILDING
				if (m_Version < 6)
					ParkingMeterEnableFlavor = defaultSettings.ParkingMeterEnableFlavor;
			#endif

				m_Version = VERSION;
				save = true;
			}
		}
		else
		{
			CF_Log.Info("[ExpansionGarageSettings] No existing setting file:" + EXPANSION_GARAGE_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}

		if (save)
		{
			Save();
		}

		return garageSettingsExist;
	}

	override bool OnSave()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnSave");
	#endif

		JsonFileLoader<ExpansionGarageSettings>.JsonSaveFile(EXPANSION_GARAGE_SETTINGS, this);

		return true;
	}


	override void Update( ExpansionSettingBase setting )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Update").Add(setting);
	#endif

		super.Update( setting );

		ExpansionSettings.SI_Garage.Invoke();
	}

	override void Defaults()
	{
		m_Version = VERSION;
	
		Enabled = true;
		AllowStoringDEVehicles = false;

	#ifdef EXPANSIONMODBASEBUILDING
		GarageMode = ExpansionGarageMode.Territory;
	#else
		GarageMode = ExpansionGarageMode.Personal;
	#endif
		GarageStoreMode = 0;
		GarageRetrieveMode = 0;
		MaxStorableVehicles = 2;
		
		VehicleSearchRadius = 20.0;
		MaxDistanceFromStoredPosition = 150.0;
		CanStoreWithCargo = true;
		NeedKeyToStore = true;

		EntityWhitelist.Insert("ExpansionParkingMeter");
	
	#ifdef EXPANSIONMODGROUPS
		EnableGroupFeatures = true;
		GroupStoreMode = 2;
	#endif	
		
	#ifdef EXPANSIONMODMARKET
		EnableMarketFeatures = true;
		StorePricePercent = 5.0;
		StaticStorePrice = 0;
	#endif
		
	#ifdef EXPANSIONMODBASEBUILDING
		MaxStorableTier1 = 2;
		MaxStorableTier2 = 4;
		MaxStorableTier3 = 6;
		MaxRangeTier1 = 20.0;
		MaxRangeTier2 = 30.0;
		MaxRangeTier3 = 40.0;
		ParkingMeterEnableFlavor = true;
	#endif
	}

	override string SettingName()
	{
		return "Garage Settings";
	}
};