/**
 * ExpansionGarageData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionGarageDataBase
{
	int m_Version;
	int m_TerritoryID;
	string m_OwnerUID;
}

class ExpansionGarageDataV1: ExpansionGarageDataBase
{
	ref array<ref ExpansionGarageVehicleDataV1> m_Vehicles;
}

class ExpansionGarageData: ExpansionGarageDataBase
{
	static const int VERSION = 3;

	ref array<ref ExpansionGarageVehicleData> m_Vehicles;

	void ExpansionGarageData()
	{
		m_TerritoryID = -1;
		m_Vehicles = new array<ref ExpansionGarageVehicleData>;
	}

	void SetTerritoryID(int id)
	{
		m_TerritoryID = id;
	}

	void SetOwnerUID(string uid)
	{
		m_OwnerUID = uid;
	}

	void AddVehicle(ExpansionGarageVehicleData vehicleData)
	{
		m_Vehicles.Insert(vehicleData);
	}

	void RemoveVehicle(ExpansionGarageVehicleData vehicleData)
	{
		for (int i = m_Vehicles.Count() - 1; i >= 0; i--)
		{
			ExpansionGarageVehicleData storedData = m_Vehicles[i];
			if (storedData.IsGlobalIDValid() && storedData.IsGlobalIDEqual(vehicleData))
				m_Vehicles.RemoveOrdered(i);
		}
	}

	static ExpansionGarageData Load(string fileName)
	{
		CF_Log.Info("[ExpansionGarageData] Load existing garage file:" + fileName);

		ExpansionGarageData garageData;
		ExpansionGarageDataBase garageDataBase;
		if (!ExpansionJsonFileParser<ExpansionGarageDataBase>.Load(fileName, garageDataBase))
			return NULL;

		bool save;

		if (garageDataBase.m_Version < VERSION)
		{
			EXPrint("[ExpansionGarageData] Load - Converting v" + garageDataBase.m_Version + " \"" + fileName + "\" to v" + VERSION);

			if (garageDataBase.m_Version < 2)
			{
				garageData = new ExpansionGarageData();
				garageData.m_OwnerUID = garageDataBase.m_OwnerUID;
				garageData.m_TerritoryID = garageDataBase.m_TerritoryID;
				ExpansionGarageDataV1 garageDataV1;
				if (ExpansionJsonFileParser<ExpansionGarageDataV1>.Load(fileName, garageDataV1))
				{
					foreach (ExpansionGarageVehicleDataV1 vehicleDataV1: garageDataV1.m_Vehicles)
					{
						if (!vehicleDataV1.IsGlobalIDValid())
						{
							Error(vehicleDataV1.m_ClassName + ": Invalid vehicle global ID " + ExpansionStatic.IntToHex(vehicleDataV1.m_GlobalID));
							continue;
						}
						string oldFileName = ExpansionEntityStorageModule.GetFileName(vehicleDataV1.m_EntityStorageID);
						if (FileExist(oldFileName) && CopyFile(oldFileName, vehicleDataV1.GetEntityStorageFileName()))
							DeleteFile(oldFileName);
						auto vehicleDataV2 = new ExpansionGarageVehicleData();
						vehicleDataV2.Copy(vehicleDataV1);
						garageData.m_Vehicles.Insert(vehicleDataV2);
					}
				}
			}
			else
			{
				if (!ExpansionJsonFileParser<ExpansionGarageData>.Load(fileName, garageData))
					return NULL;
			}

			if (garageDataBase.m_Version < 3)
			{
				Object obj;
				ExpansionVehicle vehicle;
				foreach (ExpansionGarageVehicleData vehicleDataV3: garageData.m_Vehicles)
				{
					obj = GetGame().CreateObject(vehicleDataV3.m_ClassName, "0 0 0", true);
					if (obj)
					{
						if (ExpansionVehicle.Get(vehicle, obj))
						{
							float modelZeroPointFromGround = vehicle.GetModelZeroPointDistanceFromGround();
							vehicleDataV3.m_Position = Vector(vehicleDataV3.m_Position[0], vehicleDataV3.m_Position[1] - modelZeroPointFromGround, vehicleDataV3.m_Position[2]);
						}

						GetGame().ObjectDelete(obj);
					}
					
					if (!vehicleDataV3.m_SkinIndex)
						vehicleDataV3.m_SkinIndex = -1;
				}
			}

			garageData.m_Version = VERSION;
			save = true;
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionGarageData>.Load(fileName, garageData))
				return NULL;
		}

		if (fileName != garageData.GetFileName())
		{
			save = true;
			DeleteFile(fileName);
		}

		if (save)
			garageData.Save();

		//! Make sure stored vehicle global IDs are registered. This needs to happen before loading storage!
		foreach (ExpansionGarageVehicleData vehicleData: garageData.m_Vehicles)
		{
			if (!vehicleData.IsGlobalIDValid())
			{
				Error(vehicleData.m_ClassName + ": Invalid vehicle global ID " + ExpansionStatic.IntToHex(vehicleData.m_GlobalID));
				continue;
			}
			auto globalID = new ExpansionGlobalID;
			globalID.Set(vehicleData.m_GlobalID);

			if (vehicleData.m_SkinName)
				vehicleData.m_SkinIndex = ExpansionSkinModule.s_Instance.GetSkinIndex(vehicleData.m_ClassName, vehicleData.m_SkinName);
		}

		return garageData;
	}

	string GetFileName()
	{
		return ExpansionGarageModule.s_GarageFolderPath + m_OwnerUID + ".json";
	}

	void Save()
	{
		ExpansionJsonFileParser<ExpansionGarageData>.Save(GetFileName(), this);
	}
};