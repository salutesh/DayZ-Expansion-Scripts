#ifndef CF_SURFACES
class CF_VehicleSurface
{
	private static ref map<string, ref CF_VehicleSurface> s_VehicleSurfaces = new map<string, ref CF_VehicleSurface>();

	private static string s_LastSurface;

	string Name = s_LastSurface;

	float NoiseSteer = GetValueFloat("noiseSteer");
	float NoiseFrequency = GetValueFloat("noiseFrequency");
	float Roughness = GetValueFloat("roughness");
	float FrictionOffroad = GetValueFloat("frictionOffroad");
	float FrictionSlick = GetValueFloat("frictionSlick");
	float RollResistance = GetValueFloat("rollResistance");
	float RollDrag = GetValueFloat("rollDrag");

	void CF_VehicleSurface()
	{
		s_VehicleSurfaces[s_LastSurface] = this;
	}
	
	void ~CF_VehicleSurface()
	{
	}

	/**
	 * @brief Get the vehicle surface by the name. From 'CfgVehicleSurfaces'
	 * 
	 * @param name The class name of the surface
	 * 
	 * @return Instance of the vehicle surface class, creates a new one if it doesn't exist
	 */
	static CF_VehicleSurface Get(string name)
	{
		auto surf = s_VehicleSurfaces[name];
		if (!surf)
		{
			s_LastSurface = name;
			return new CF_VehicleSurface();
		}

		return surf;
	}

	/**
	 * @brief Get the vehicle surface at the position in the world
	 * 
	 * @param position The position of the surface within the world
	 * 
	 * @return Global instance of the vehicle surface class
	 */
	static CF_VehicleSurface At(vector position)
	{
		return CF_Surface.At(position).VehicleSurface;
	}

	/**
	 * @brief Get the vehicle surface at the position in the world at it's maximum height
	 * 
	 * @param x The x position of the surface within the world
	 * @param z The z position of the surface within the world
	 * 
	 * @return Global instance of the vehicle surface class
	 */
	static CF_VehicleSurface At(float x, float z)
	{
		return CF_Surface.At(x, z).VehicleSurface;
	}

	/**
	 * @brief Get the surface at the position in the world at it's maximum height
	 * 
	 * @param object The object above the surface of the object 
	 * 
	 * @return Global instance of the surface class
	 */
	static CF_VehicleSurface At(Object object)
	{
		string name;
		int liquidType;
		GetGame().SurfaceUnderObject(object, name, liquidType);
		return CF_Surface.Get(name).VehicleSurface;
	}

#ifdef CF_DebugUI
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		instance.Add("Name", Name);
		instance.Add("Noise Steer", NoiseSteer);
		instance.Add("Noise Frequency", NoiseFrequency);
		instance.Add("Roughness", Roughness);
		instance.Add("Friction Offroad", FrictionOffroad);
		instance.Add("Friction Slick", FrictionSlick);
		instance.Add("Roll Resistance", RollResistance);
		instance.Add("Roll Drag", RollDrag);

		return true;
	}
#endif
	
	override string GetDebugName()
	{
		string str = this.ToString();

		str += ",\n\tName=\"" + Name + "\""; 
		str += ",\n\tNoiseSteer=\"" + NoiseSteer + "\""; 
		str += ",\n\tNoiseFrequency=\"" + NoiseFrequency + "\""; 
		str += ",\n\tRoughness=\"" + Roughness + "\""; 
		str += ",\n\tFrictionOffroad=\"" + FrictionOffroad + "\""; 
		str += ",\n\tFrictionSlick=\"" + FrictionSlick + "\""; 
		str += ",\n\tRollResistance=\"" + RollResistance + "\""; 
		str += ",\n\tRollDrag=\"" + RollDrag + "\""; 

		return str;
	}

	[CF_EventSubscriber(CF_VehicleSurface._GetAllSurfaces, CF_LifecycleEvents.OnMissionCreate)]
	static void _GetAllSurfaces()
	{
		string path = "CfgVehicleSurfaces";
		string name;
		int count = GetGame().ConfigGetChildrenCount(path);

		for (int index = 0; index < count; index++)
		{
			GetGame().ConfigGetChildName(path, index, name);

			CF_VehicleSurface.Get(name);
		}
	}

	private static int GetValueInt(string param)
	{
		string path = "CfgVehicleSurfaces" + " " + s_LastSurface + " " + param;
		if (GetGame().ConfigIsExisting(path)) return GetGame().ConfigGetInt(path);
		return 0;
	}

	private static float GetValueFloat(string param)
	{
		string path = "CfgVehicleSurfaces" + " " + s_LastSurface + " " + param;
		if (GetGame().ConfigIsExisting(path)) return GetGame().ConfigGetFloat(path);
		return 0;
	}

	private static string GetValueString(string param)
	{
		string path = "CfgVehicleSurfaces" + " " + s_LastSurface + " " + param;
		if (GetGame().ConfigIsExisting(path)) return GetGame().ConfigGetTextOut(path);
		return string.Empty;
	}
};
#endif
