/**
 * ExpansionAISettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAIRoamingLocation: ExpansionLocation
{
	bool Enabled;

	override void Init(vector position, float radius, string name = string.Empty, string type = string.Empty, string className = string.Empty, Object obj = null, int index = -1)
	{
		super.Init(position, radius, name, type, className, obj, index);

		//! Since name is not shown in-game and just serves an identifying purpose, prefer classname but include name if applicable
		if (m_ClassName.IndexOf(Name) < 0)
			Name = string.Format("%1 (%2)", m_ClassName, Name);
		else
			Name = m_ClassName;

		Enabled = true;
	}
}

class ExpansionAINoGoAreaConfig
{
	string Name;
	vector Position;
	float Radius;
	float Height;
}

class ExpansionAINoGoAreas: array<ExpansionAINoGoAreaConfig>
{
	bool IsPointInside(vector point)
	{
		foreach (ExpansionAINoGoAreaConfig areaConfig: this)
		{
			if (ExpansionMath.IsPointInCylinder(areaConfig.Position, areaConfig.Radius, areaConfig.Height, point))
				return true;
		}

		return false;
	}
}

class ExpansionAILocationSettings: ExpansionSettingBase
{
	static const string PATH = EXPANSION_MISSION_SETTINGS_FOLDER + "AILocationSettings.json";
	static const int VERSION = 3;

	ref array<ref ExpansionAIRoamingLocation> RoamingLocations = {};
	ref TStringArray ExcludedRoamingBuildings = {};
	ref ExpansionAINoGoAreas NoGoAreas = {};

	[NonSerialized()]
	private bool m_IsLoaded;

	override int Send(PlayerIdentity identity)
	{
		return 0;
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
		m_IsLoaded = true;

		bool save;
		bool updateIndex = true;

		bool settingsExist = FileExist(PATH);

		if (settingsExist)
		{
			EXPrint(this, "Load existing setting file:" + PATH);

			if (!ExpansionJsonFileParser<ExpansionAILocationSettings>.Load(PATH, this))
			{
				//! Use defaults, but DON'T save them
				Defaults();
				updateIndex = false;
			}
			else if (m_Version < VERSION)
			{
				EXPrint(this, "Load - Converting v" + m_Version + " \"" + PATH + "\" to v" + VERSION);

				auto settingsDefault = new ExpansionAILocationSettings;
				settingsDefault.Defaults();

				foreach (string excluded: settingsDefault.ExcludedRoamingBuildings)
				{
					if (ExcludedRoamingBuildings.Find(excluded) == -1)
						ExcludedRoamingBuildings.Insert(excluded);
				}

				m_Version = VERSION;
				save = true;
			}
		}
		else
		{
			EXPrint(this, "No existing setting file:" + PATH + ". Creating defaults!");
			Defaults();
			save = true;
			updateIndex = false;
		}

		if (save)
			Save();

		if (updateIndex)
		{
			foreach (int i, auto loc: RoamingLocations)
			{
				loc.m_Index = i;
			}
		}

		//! Convert excluded buildings to lowercase (don't save!)
		TStringArray excludedRoamingBuildings = {};
		excludedRoamingBuildings.Copy(ExcludedRoamingBuildings);
		ExcludedRoamingBuildings.Clear();
		foreach (string excludedBuilding: excludedRoamingBuildings)
		{
			excludedBuilding.ToLower();
			ExcludedRoamingBuildings.Insert(excludedBuilding);
		}

		return settingsExist;
	}

	override bool OnSave()
	{
		JsonFileLoader<ExpansionAILocationSettings>.JsonSaveFile(PATH, this);
		return true;
	}

	override void Defaults()
	{
		m_Version = VERSION;

		int include;
		include |= ExpansionLocationType.CAMP;
		include |= ExpansionLocationType.HILL;
		include |= ExpansionLocationType.LOCAL;
		include |= ExpansionLocationType.LOCALOFFICE;
		//include |= ExpansionLocationType.MARINE;
		include |= ExpansionLocationType.RAILROADSTATION;
		include |= ExpansionLocationType.RUIN;
		include |= ExpansionLocationType.SETTLEMENT;
		//include |= ExpansionLocationType.VIEWPOINT;

		RoamingLocations = ExpansionLocationT<ExpansionAIRoamingLocation>.GetWorldLocations(include);

		if (RoamingLocations.Count() == 0)
			EXPrint(this, "Map " + g_Game.GetWorldName() + " has no locations!");

		//! Excluded buildings are structures where the pathfinding won't generate a good path.
		//! TODO: Possibly move excludes to cover obj discovery in eAIBase::UpdateTargets?
		//!       Testing necessary. Is that even a problem for taking cover (since it's temporary) or just
		//!       for roaming (target position won't change unless reached)?
		//! @note classnames can be for groups of buildings (e.g. Land_House_1W09 has a yellow variant)
		ExcludedRoamingBuildings = {
			"Land_Boat_",  //! Sakhal, pathfinding won't find a path off the boat
			"Land_CementWorks_Hall2_Grey",  //! Unsuitable path endpoint
			"Land_Factory_Small",  //! Unsuitable path endpoint
			"Land_House_1W09",  //! Path endpoint between opened door and wall leading to AI running in circles trying to reach point
			"Land_House_2W03",  //! Unsuitable path endpoint
			"Land_HouseBlock_1F4",  //! Path endpoint behind unopenable lattice door
			"Land_Boathouse",  //! When AI falls into water it will likely get stuck under pier due to path always leading under it
			"Land_Mine_Building",  //! Stairs
			"Land_Shed_W2",  //! Pathfinding tends to find a way in, but not out.
			"Land_Tenement_Big",  //! AI can get stuck on upper floors when climbing the broken stairs
			"Land_Misc_Toilet_Mobile",  //! Too many door interactions
			"Land_Ship_Medium2"  //! AI can get stuck
		};
	}

	override string SettingName()
	{
		return "AI Location Settings";
	}	
}
