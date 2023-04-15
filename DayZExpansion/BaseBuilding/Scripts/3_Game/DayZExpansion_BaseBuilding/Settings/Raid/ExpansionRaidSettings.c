/**
 * ExpansionRaidSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionRaidSchedule
{
	static autoptr TStringArray WEEKDAYS = {"SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY"};
	string Weekday;
	int StartHour;
	int StartMinute;
	int DurationMinutes;

	[NonSerialized()]
	int m_WeekdayIndex;

	void ExpansionRaidSchedule(string weekday = string.Empty, int startHour = 0, int startMinute = 0, int duration = 0)
	{
		Weekday = weekday;
		StartHour = startHour;
		StartMinute = startMinute;
		DurationMinutes = duration;

		if (weekday)
			Validate();
	}

	void Validate()
	{
		string weekday = Weekday;
		weekday.ToUpper();
		m_WeekdayIndex = WEEKDAYS.Find(weekday);
		if (m_WeekdayIndex == -1)
			Error(ToString() + " Invalid weekday " + Weekday);

		if (StartHour > 23)
			Error(ToString() + " Invalid start hour " + StartHour);

		if (StartMinute > 59)
			Error(ToString() + " Invalid start minute " + StartMinute);

		if (StartHour * 60 + StartMinute + DurationMinutes > 1440)
			Error(ToString() + " Invalid duration " + DurationMinutes + " for start time " + StartHour + ":" + StartMinute + " on " + Weekday);
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(m_WeekdayIndex);
		ctx.Write(StartHour);
		ctx.Write(StartMinute);
		ctx.Write(DurationMinutes);
	}

	bool OnReceive(ParamsReadContext ctx)
	{
		ctx.Read(m_WeekdayIndex);
		ctx.Read(StartHour);
		ctx.Read(StartMinute);
		ctx.Read(DurationMinutes);

		EXTrace.Print(EXTrace.BASEBUILDING, this, string.Format("Received weekday = %1 start = %2:%3 duration = %4", ExpansionRaidSchedule.WEEKDAYS[m_WeekdayIndex], StartHour, StartMinute, DurationMinutes));

		return true;
	}
}

/**@class		ExpansionRaidSettings
 * @brief		Spawn settings class
 **/
class ExpansionRaidSettings: ExpansionSettingBase
{
	static const int VERSION = 5;

	ExpansionBaseBuildingRaidMode BaseBuildingRaidMode;			//! 0 = Default, everything can take dmg | 1 = doors and gates | 2 = doors, gates and windows | 3 = any wall/fence
	float ExplosionTime;								//! Ammount of time it takes for explosive to explode.
	autoptr TStringArray ExplosiveDamageWhitelist;		//! List of damage sources allowed to damage bases when whitelist is enabled. 
	bool EnableExplosiveWhitelist;   					//! If enabled, only damage sources listed in ExplosiveDamageWhitelist will be able to damage walls. 
	float ExplosionDamageMultiplier;					//! Damage multiplier from explosion.
	float ProjectileDamageMultiplier;					//! Damage multiplier from projectiles.

	bool CanRaidSafes;									//! If enabled, make safes raidable
	bool SafeRaidUseSchedule;

	float SafeExplosionDamageMultiplier;				//! Damage multiplier from explosion on safes.
	float SafeProjectileDamageMultiplier;				//! Damage multiplier from explosion on safes.

	autoptr TStringArray SafeRaidTools;					//! List of tools allowed for raiding safes
	int SafeRaidToolTimeSeconds;						//! Time needed to raid safe with tool
	int SafeRaidToolCycles;								//! Number of cycles needed to raid safe
	float SafeRaidToolDamagePercent;					//! Total damage dealt to tool over time (100 = tool will be in ruined state after all cycles finished)

	bool CanRaidBarbedWire;								//! If enabled, make barbed wire raidable

	autoptr TStringArray BarbedWireRaidTools;			//! List of tools allowed for raiding barbed wire
	int BarbedWireRaidToolTimeSeconds;					//! Time needed to raid barbed wire with tool
	int BarbedWireRaidToolCycles;						//! Number of cycles needed to raid barbed wire
	float BarbedWireRaidToolDamagePercent;				//! Total damage dealt to tool over time (100 = tool will be in ruined state after all cycles finished)

	RaidLocksOnWallsEnum CanRaidLocksOnWalls;			//! If set to 1 make locks (both vanilla and Expansion) raidable on walls | 2 = only doors | 3 = only gates
	bool CanRaidLocksOnFences;							//! If enabled, make locks (both vanilla and Expansion) raidable on fences
	bool CanRaidLocksOnTents;							//! If enabled, make locks (both vanilla and Expansion) raidable on tents

	autoptr TStringArray LockRaidTools;					//! List of tools allowed for raiding locks
	int LockOnWallRaidToolTimeSeconds;					//! Time needed to raid lock on wall with tool. Disabled <= 0
	int LockOnFenceRaidToolTimeSeconds;					//! Time needed to raid lock on fence with tool. Disabled <= 0
	int LockOnTentRaidToolTimeSeconds;					//! Time needed to raid lock on tent with tool. Disabled <= 0
	int LockRaidToolCycles;								//! Number of cycles needed to raid lock
	float LockRaidToolDamagePercent;					//! Total damage dealt to tool over time (100 = tool will be in ruined state after all cycles finished)

	bool CanRaidLocksOnContainers;							//! If enabled, makes code locked containers raidable
	bool LockOnContainerRaidUseSchedule;

	autoptr TStringArray LockOnContainerRaidTools;			//! List of tools allowed for raiding locks on containers
	int LockOnContainerRaidToolTimeSeconds;					//! Time needed to raid lock on container with tool
	int LockOnContainerRaidToolCycles;						//! Number of cycles needed to raid lock on container
	float LockOnContainerRaidToolDamagePercent;				//! Total damage dealt to tool over time (100 = tool will be in ruined state after all cycles finished)

	autoptr array<ref ExpansionRaidSchedule> Schedule;

	[NonSerialized()]
	int m_TimeDiff;  //! Time difference server time to client (local time), client only. TODO: Make this part of some core settings as it can be used elsewhere as well
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionRaidSettings()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "ExpansionRaidSettings");
#endif

		ExplosiveDamageWhitelist = new TStringArray;
		SafeRaidTools = new TStringArray;
		BarbedWireRaidTools = new TStringArray;
		LockRaidTools = new TStringArray;
		LockOnContainerRaidTools = new TStringArray;
		Schedule = new array<ref ExpansionRaidSchedule>;
	}
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
#endif

		ctx.Read(BaseBuildingRaidMode);

		ctx.Read(ExplosionTime);

		ctx.Read(EnableExplosiveWhitelist);
		ctx.Read(ExplosionDamageMultiplier);
		ctx.Read(ProjectileDamageMultiplier);

		ctx.Read(CanRaidSafes);
		ctx.Read(SafeRaidUseSchedule);

		ctx.Read(SafeExplosionDamageMultiplier);
		ctx.Read(SafeProjectileDamageMultiplier);

		ctx.Read(SafeRaidTools);
		ctx.Read(SafeRaidToolTimeSeconds);
		ctx.Read(SafeRaidToolCycles);
		ctx.Read(SafeRaidToolDamagePercent);

		ctx.Read(CanRaidBarbedWire);

		ctx.Read(BarbedWireRaidTools);
		ctx.Read(BarbedWireRaidToolTimeSeconds);
		ctx.Read(BarbedWireRaidToolCycles);
		ctx.Read(BarbedWireRaidToolDamagePercent);

		ctx.Read(CanRaidLocksOnWalls);
		ctx.Read(CanRaidLocksOnFences);
		ctx.Read(CanRaidLocksOnTents);

		ctx.Read(LockRaidTools);
		ctx.Read(LockOnWallRaidToolTimeSeconds);
		ctx.Read(LockOnFenceRaidToolTimeSeconds);
		ctx.Read(LockOnTentRaidToolTimeSeconds);
		ctx.Read(LockRaidToolCycles);
		ctx.Read(LockRaidToolDamagePercent);

		ctx.Read(CanRaidLocksOnContainers);
		ctx.Read(LockOnContainerRaidUseSchedule);

		ctx.Read(LockOnContainerRaidTools);
		ctx.Read(LockOnContainerRaidToolTimeSeconds);
		ctx.Read(LockOnContainerRaidToolCycles);
		ctx.Read(LockOnContainerRaidToolDamagePercent);

		int count;
		ctx.Read(count);
		if (count)
		{
			int timestamp;
			ctx.Read(timestamp);

			auto now = CF_Date.Now();
			m_TimeDiff = timestamp - now.GetTimestamp();

			Schedule.Clear();
			for (int i = 0; i < count; i++)
			{
				auto schedule = new ExpansionRaidSchedule;
				schedule.OnReceive(ctx);
				Schedule.Insert(schedule);
			}
		}
		
		m_IsLoaded = true;

		ExpansionSettings.SI_Raid.Invoke();
		
		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ctx.Write(BaseBuildingRaidMode);

		ctx.Write(ExplosionTime);

		//! Don't send ExplosiveDamageWhitelist (used server-side only)

		ctx.Write(EnableExplosiveWhitelist);
		ctx.Write(ExplosionDamageMultiplier);
		ctx.Write(ProjectileDamageMultiplier);

		ctx.Write(CanRaidSafes);
		ctx.Write(SafeRaidUseSchedule);

		ctx.Write(SafeExplosionDamageMultiplier);
		ctx.Write(SafeProjectileDamageMultiplier);

		ctx.Write(SafeRaidTools);
		ctx.Write(SafeRaidToolTimeSeconds);
		ctx.Write(SafeRaidToolCycles);
		ctx.Write(SafeRaidToolDamagePercent);

		ctx.Write(CanRaidBarbedWire);

		ctx.Write(BarbedWireRaidTools);
		ctx.Write(BarbedWireRaidToolTimeSeconds);
		ctx.Write(BarbedWireRaidToolCycles);
		ctx.Write(BarbedWireRaidToolDamagePercent);

		ctx.Write(CanRaidLocksOnWalls);
		ctx.Write(CanRaidLocksOnFences);
		ctx.Write(CanRaidLocksOnTents);

		ctx.Write(LockRaidTools);
		ctx.Write(LockOnWallRaidToolTimeSeconds);
		ctx.Write(LockOnFenceRaidToolTimeSeconds);
		ctx.Write(LockOnTentRaidToolTimeSeconds);
		ctx.Write(LockRaidToolCycles);
		ctx.Write(LockRaidToolDamagePercent);

		ctx.Write(CanRaidLocksOnContainers);
		ctx.Write(LockOnContainerRaidUseSchedule);

		ctx.Write(LockOnContainerRaidTools);
		ctx.Write(LockOnContainerRaidToolTimeSeconds);
		ctx.Write(LockOnContainerRaidToolCycles);
		ctx.Write(LockOnContainerRaidToolDamagePercent);

		int count = Schedule.Count();
		ctx.Write(count);
		if (count)
		{
			auto now = CF_Date.Now();
			ctx.Write(now.GetTimestamp());

			foreach (auto schedule: Schedule)
			{
				schedule.OnSend(ctx);
			}
		}
	}

	// ------------------------------------------------------------
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
		rpc.Send( null, ExpansionSettingsRPC.Raid, true, identity );
		
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionRaidSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}
	
	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionRaidSettings s )
	{
		BaseBuildingRaidMode = s.BaseBuildingRaidMode;

		ExplosiveDamageWhitelist.Copy( s.ExplosiveDamageWhitelist );

		ExplosionTime = s.ExplosionTime;

		EnableExplosiveWhitelist = s.EnableExplosiveWhitelist;
		ExplosionDamageMultiplier = s.ExplosionDamageMultiplier;
		ProjectileDamageMultiplier = s.ProjectileDamageMultiplier;	
		
		CanRaidSafes = s.CanRaidSafes;
		SafeExplosionDamageMultiplier = s.SafeExplosionDamageMultiplier;
		SafeProjectileDamageMultiplier = s.SafeProjectileDamageMultiplier;

		SafeRaidTools.Copy( s.SafeRaidTools );
		SafeRaidToolTimeSeconds = s.SafeRaidToolTimeSeconds;
		SafeRaidToolCycles = s.SafeRaidToolCycles;
		SafeRaidToolDamagePercent = s.SafeRaidToolDamagePercent;
		
		CanRaidBarbedWire = s.CanRaidBarbedWire;

		BarbedWireRaidTools.Copy( s.BarbedWireRaidTools );
		BarbedWireRaidToolTimeSeconds = s.BarbedWireRaidToolTimeSeconds;
		BarbedWireRaidToolCycles = s.BarbedWireRaidToolCycles;
		BarbedWireRaidToolDamagePercent = s.BarbedWireRaidToolDamagePercent;
		
		CanRaidLocksOnWalls = s.CanRaidLocksOnWalls;
		CanRaidLocksOnFences = s.CanRaidLocksOnFences;
		CanRaidLocksOnTents = s.CanRaidLocksOnTents;

		LockRaidTools.Copy( s.LockRaidTools );
		LockOnWallRaidToolTimeSeconds = s.LockOnWallRaidToolTimeSeconds;
		LockOnFenceRaidToolTimeSeconds = s.LockOnFenceRaidToolTimeSeconds;
		LockOnTentRaidToolTimeSeconds = s.LockOnTentRaidToolTimeSeconds;
		LockRaidToolCycles = s.LockRaidToolCycles;
		LockRaidToolDamagePercent = s.LockRaidToolDamagePercent;

		CanRaidLocksOnContainers = s.CanRaidLocksOnContainers;

		LockOnContainerRaidTools.Copy( s.LockOnContainerRaidTools );
		LockOnContainerRaidToolTimeSeconds = s.LockOnContainerRaidToolTimeSeconds;
		LockOnContainerRaidToolCycles = s.LockOnContainerRaidToolCycles;
		LockOnContainerRaidToolDamagePercent = s.LockOnContainerRaidToolDamagePercent;

		Schedule.Clear();
		foreach (auto schedule: s.Schedule)
		{
			Schedule.Insert(schedule);
		}
	}
	
	// ------------------------------------------------------------
	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		m_IsLoaded = false;
	}

	// ------------------------------------------------------------
	override bool OnLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoad");
#endif

		m_IsLoaded = true;
		
		bool save;
		
		bool raidSettingsExist = FileExist(EXPANSION_RAID_SETTINGS);

		if (raidSettingsExist)
		{
			EXPrint("[ExpansionRaidSettings] Load existing setting file:" + EXPANSION_RAID_SETTINGS);
			
			ExpansionRaidSettings settingsDefault = new ExpansionRaidSettings;
			settingsDefault.Defaults();

			JsonFileLoader<ExpansionRaidSettings>.JsonLoadFile(EXPANSION_RAID_SETTINGS, this);

			if (m_Version < VERSION)
			{
				EXPrint("[ExpansionRaidSettings] Load - Converting v" + m_Version + " \"" + EXPANSION_RAID_SETTINGS + "\" to v" + VERSION);

				if (m_Version < 3)
				{
					CanRaidLocksOnContainers = settingsDefault.CanRaidLocksOnContainers;

					LockOnContainerRaidTools.Copy( settingsDefault.LockOnContainerRaidTools );
					LockOnContainerRaidToolTimeSeconds = settingsDefault.LockOnContainerRaidToolTimeSeconds;
					LockOnContainerRaidToolCycles = settingsDefault.LockOnContainerRaidToolCycles;
					LockOnContainerRaidToolDamagePercent = settingsDefault.LockOnContainerRaidToolDamagePercent;
				}

				if (m_Version < 4)
				{
					foreach (auto defaultSchedule: settingsDefault.Schedule)
					{
						Schedule.Insert(defaultSchedule);
					}
				}

				if (m_Version < 5)
				{
					SafeRaidUseSchedule = settingsDefault.SafeRaidUseSchedule;
					LockOnContainerRaidUseSchedule = settingsDefault.LockOnContainerRaidUseSchedule;
				}

				m_Version = VERSION;
				save = true;
			}
		}
		else
		{
			EXPrint("[ExpansionRaidSettings] No existing setting file:" + EXPANSION_RAID_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}
		
		if (save)
			Save();
		
		foreach (auto schedule: Schedule)
		{
			schedule.Validate();
		}

		return raidSettingsExist;
	}
	
	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionRaidSettings] Saving settings");
		
		if ( IsMissionHost() )
		{
			JsonFileLoader<ExpansionRaidSettings>.JsonSaveFile( EXPANSION_RAID_SETTINGS, this );
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );

		ExpansionSettings.SI_Raid.Invoke();
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;
		
		ExplosionTime = 30;

		ExplosiveDamageWhitelist.Insert("Expansion_C4_Explosion");
		ExplosiveDamageWhitelist.Insert("Expansion_RPG_Explosion");
		ExplosiveDamageWhitelist.Insert("Expansion_LAW_Explosion");
		ExplosiveDamageWhitelist.Insert("M79");
		
		ExplosiveDamageWhitelist.Insert("RGD5Grenade");
		ExplosiveDamageWhitelist.Insert("M67Grenade");
		ExplosiveDamageWhitelist.Insert("FlashGrenade");
		ExplosiveDamageWhitelist.Insert("Land_FuelStation_Feed");
		ExplosiveDamageWhitelist.Insert("Land_FuelStation_Feed_Enoch");

		EnableExplosiveWhitelist = false;
		ExplosionDamageMultiplier = 50;
		ProjectileDamageMultiplier = 1;
		
		CanRaidSafes = true;
		SafeRaidUseSchedule = true;
		CanRaidLocksOnContainers = true;
		LockOnContainerRaidUseSchedule = true;
		SafeExplosionDamageMultiplier = 17;
		SafeProjectileDamageMultiplier = 1;

		LockOnContainerRaidTools.Insert( "ExpansionPropaneTorch" );
		LockOnContainerRaidToolTimeSeconds = 10 * 60;
		LockOnContainerRaidToolCycles = 5;
		LockOnContainerRaidToolDamagePercent = 100;

		SafeRaidTools.Insert( "ExpansionPropaneTorch" );
		SafeRaidToolTimeSeconds = 10 * 60;
		SafeRaidToolCycles = 5;
		SafeRaidToolDamagePercent = 100;
		
		CanRaidBarbedWire = true;

		BarbedWireRaidTools.Insert( "ExpansionBoltCutters" );
		BarbedWireRaidToolTimeSeconds = 5 * 60;
		BarbedWireRaidToolCycles = 5;
		BarbedWireRaidToolDamagePercent = 100;
		
		CanRaidLocksOnWalls = RaidLocksOnWallsEnum.Disabled;
		CanRaidLocksOnFences = false;
		CanRaidLocksOnTents = false;

		LockOnWallRaidToolTimeSeconds = 15 * 60;
		LockOnFenceRaidToolTimeSeconds = 15 * 60;
		LockOnTentRaidToolTimeSeconds = 10 * 60;
		LockRaidToolCycles = 5;
		LockRaidToolDamagePercent = 100;

		BaseBuildingRaidMode = ExpansionBaseBuildingRaidMode.All;

		Schedule.Insert(new ExpansionRaidSchedule("Sunday", 0, 0, 1440));
		Schedule.Insert(new ExpansionRaidSchedule("Monday", 0, 0, 1440));
		Schedule.Insert(new ExpansionRaidSchedule("Tuesday", 0, 0, 1440));
		Schedule.Insert(new ExpansionRaidSchedule("Wednesday", 0, 0, 1440));
		Schedule.Insert(new ExpansionRaidSchedule("Thursday", 0, 0, 1440));
		Schedule.Insert(new ExpansionRaidSchedule("Friday", 0, 0, 1440));
		Schedule.Insert(new ExpansionRaidSchedule("Saturday", 0, 0, 1440));
	}
	
	// ------------------------------------------------------------
	override string SettingName()
	{
		return "Raid Settings";
	}

	bool IsRaidableNow()
	{
		if (!Schedule.Count())
			return true;

		auto now = CF_Date.Now();
		int timestamp = now.GetTimestamp();

		//! Convert local time to server time
		now.EpochToDate(timestamp + m_TimeDiff);

		int weekdayIndex = now.GetDayOfWeek();

		foreach (auto schedule: Schedule)
		{
			if (weekdayIndex != schedule.m_WeekdayIndex)
				continue;

			int current = now.GetHours() * 60 + now.GetMinutes();
			int start = schedule.StartHour * 60 + schedule.StartMinute;
			int end = start + schedule.DurationMinutes;

			if (current >= start && current <= end)
				return true;
		}

		return false;
	}
};