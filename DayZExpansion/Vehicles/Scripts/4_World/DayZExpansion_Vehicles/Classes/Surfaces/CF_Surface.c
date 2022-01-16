#ifndef CF_SURFACES
class CF_Surface
{
	private static ref map<string, ref CF_Surface> s_Surfaces = new map<string, ref CF_Surface>();

	private static string s_LastSurface;

	string Name = s_LastSurface;

	float Interior = GetValueFloat("interior");
	float Deflection = GetValueFloat("deflection");
	float Friction = GetValueFloat("friction");
	float Restitution = GetValueFloat("restitution");

	string SoundEnvironment = GetValueString("soundEnviron");
	string SoundHit = GetValueString("soundHit");
	string Character = GetValueString("character");

	string Files = GetValueString("files");

	float FootDamage = GetValueFloat("footDamage");
	float Audibility = GetValueFloat("audibility");

	string Impact = GetValueString("impact");

	bool IsDigable = GetValueInt("isDigable");
	bool IsFertile = GetValueInt("isFertile");
	bool IsLiquid = GetValueInt("liquidType");
	bool IsStairs = GetValueInt("isStairs");

	int LiquidType = GetValueInt("liquidType");

	float ChanceForCatch = GetValueFloat("chanceForCatch");
	float WindModifier = GetValueFloat("windModifier");

	CF_VehicleSurface VehicleSurface = GetValueVehicleSurface("vpSurface");

	/**
	 * @brief Private constructor to prevent outside creation of the class
	 */
	private void CF_Surface()
	{
		s_Surfaces[s_LastSurface] = this;
	}
	
	/**
	 * @brief Private constructor to prevent from marking as ref
	 */
	private void ~CF_Surface()
	{
	}

	/**
	 * @brief Get the surface by the name. From 'CfgSurfaces'
	 * 
	 * @param name The class name of the surface
	 * 
	 * @return Instance of the surface class, creates a new one if it doesn't exist
	 */
	static CF_Surface Get(string name)
	{
		auto surf = s_Surfaces[name];
		if (!surf)
		{
			s_LastSurface = name;
			return new CF_Surface();
		}

		return surf;
	}

	/**
	 * @brief Get the surface at the position in the world
	 * 
	 * @param position The position of the surface within the world
	 * 
	 * @return Global instance of the surface class
	 */
	static CF_Surface At(vector position)
	{
		string name;
		GetGame().SurfaceGetType3D(position[0], position[1], position[2], name);
		return CF_Surface.Get(name);
	}

	/**
	 * @brief Get the surface at the position in the world at it's maximum height
	 * 
	 * @param x The x position of the surface within the world
	 * @param z The z position of the surface within the world
	 * 
	 * @return Global instance of the surface class
	 */
	static CF_Surface At(float x, float z)
	{
		string name;
		GetGame().SurfaceGetType(x, z, name);
		return CF_Surface.Get(name);
	}

	/**
	 * @brief Get the surface at the position in the world at it's maximum height
	 * 
	 * @param object The object above the surface of the object 
	 * 
	 * @return Global instance of the surface class
	 */
	static CF_Surface At(Object object)
	{
		string name;
		int liquidType;
		GetGame().SurfaceUnderObject(object, name, liquidType);
		return CF_Surface.Get(name);
	}

#ifdef CF_DebugUI
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		instance.Add("Name", Name);
		instance.Add("Interior", Interior);
		instance.Add("Deflection", Deflection);
		instance.Add("Friction", Friction);
		instance.Add("Restitution", Restitution);
		instance.Add("Sound Environent", SoundEnvironment);
		instance.Add("Sound Hit", SoundHit);
		instance.Add("Character", Character);
		instance.Add("Files", Files);
		instance.Add("Foot Damage", FootDamage);
		instance.Add("Audibility", Audibility);
		instance.Add("Impact", Impact);
		instance.Add("Is Digable", IsDigable);
		instance.Add("Is Fertile", IsFertile);
		instance.Add("Is Liquid", IsLiquid);
		instance.Add("Is Stairs", IsStairs);
		instance.Add("LiquidType", LiquidType);
		instance.Add("Chance For Catch", ChanceForCatch);
		instance.Add("Wind Modifier", WindModifier);
		instance.Add(VehicleSurface);

		return true;
	}
#endif
	
	override string GetDebugName()
	{
		string str = this.ToString();
		
		str += ",\n\tName=\"" + Name + "\""; 
		str += ",\n\tInterior=\"" + Interior + "\""; 
		str += ",\n\tDeflection=\"" + Deflection + "\""; 
		str += ",\n\tFriction=\"" + Friction + "\""; 
		str += ",\n\tRestitution=\"" + Restitution + "\""; 
		str += ",\n\tSoundEnvironent=\"" + SoundEnvironment + "\""; 
		str += ",\n\tSoundHit=\"" + SoundHit + "\""; 
		str += ",\n\tCharacter=\"" + Character + "\""; 
		str += ",\n\tFiles=\"" + Files + "\""; 
		str += ",\n\tFootDamage=\"" + FootDamage + "\""; ;
		str += ",\n\tAudibility=\"" + Audibility + "\""; 
		str += ",\n\tImpact=\"" + Impact + "\""; 
		str += ",\n\tIsDigable=\"" + IsDigable + "\""; 
		str += ",\n\tIsFertile=\"" + IsFertile + "\""; 
		str += ",\n\tIsLiquid=\"" + IsLiquid + "\""; 
		str += ",\n\tIsStairs=\"" + IsStairs + "\""; 
		str += ",\n\tLiquidType=\"" + LiquidType + "\""; 
		str += ",\n\tChanceForCatch=\"" + ChanceForCatch + "\""; 
		str += ",\n\tWindModifier=\"" + WindModifier + "\""; 
		str += ",\n\tVehicleSurface=\"" + VehicleSurface.GetDebugName() + "\""; 		
		
		return str;
	}

	[CF_EventSubscriber(CF_Surface._GetAllSurfaces, CF_LifecycleEvents.OnMissionCreate)]
	static void _GetAllSurfaces()
	{
		string path = "cfgSurfaces";
		string name;
		int count = GetGame().ConfigGetChildrenCount(path);

		for (int index = 0; index < count; index++)
		{
			GetGame().ConfigGetChildName(path, index, name);

			CF_Surface.Get(name);
		}
	}

	private static int GetValueInt(string param)
	{
		string path = "CfgSurfaces" + " " + s_LastSurface + " " + param;
		if (GetGame().ConfigIsExisting(path)) return GetGame().ConfigGetInt(path);
		return 0;
	}

	private static float GetValueFloat(string param)
	{
		string path = "CfgSurfaces" + " " + s_LastSurface + " " + param;
		if (GetGame().ConfigIsExisting(path)) return GetGame().ConfigGetFloat(path);
		return 0;
	}

	private static string GetValueString(string param)
	{
		string path = "CfgSurfaces" + " " + s_LastSurface + " " + param;
		if (GetGame().ConfigIsExisting(path)) return GetGame().ConfigGetTextOut(path);
		return string.Empty;
	}

	private static CF_VehicleSurface GetValueVehicleSurface(string param)
	{
		return CF_VehicleSurface.Get(GetValueString(param));
	}
};
#endif
