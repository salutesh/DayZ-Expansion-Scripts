/**
 * ExpansionGarageVehicleData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionGarageVehicleDataBase
{
	autoptr array<string> m_OwnerUIDs;
	string m_ClassName;
	vector m_Position;
	vector m_Orientation;

	autoptr array<int> m_GlobalID;

	bool IsGlobalIDValid()
	{
		if (m_GlobalID.Count() != 4)
			return false;

		//! @note {0, 0, 0, 0} is reserved
		return m_GlobalID[0] != 0 || m_GlobalID[1] != 0 || m_GlobalID[2] != 0 || m_GlobalID[3] != 0;
	}

	bool IsGlobalIDEqual(TIntArray id)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_GlobalID[i] != id[i])
				return false;
		}

		return true;
	}

	bool IsGlobalIDEqual(ExpansionGarageVehicleData vehicleData)
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_GlobalID[i] != vehicleData.m_GlobalID[i])
				return false;
		}

		return true;
	}

	string GetEntityStorageFileName()
	{
		return ExpansionEntityStorageModule.GetFileName(ExpansionStatic.IntToHex(m_GlobalID));
	}
}

class ExpansionGarageVehicleDataV1: ExpansionGarageVehicleDataBase
{
	int m_EntityStorageID;
}

class ExpansionGarageVehicleData: ExpansionGarageVehicleDataBase
{
	string m_SkinName;

	[NonSerialized()];
	int m_Index;

	[NonSerialized()];
	int m_SkinIndex;

	[NonSerialized()];
	int m_TerritoryID;  //! World vehicles only
	[NonSerialized()];
	int m_NetworkIDLow;  //! World vehicles only
	[NonSerialized()];
	int m_NetworkIDHigh;  //! World vehicles only
	[NonSerialized()];
	Object m_VehicleObject;  //! World vehicles only
	[NonSerialized()];
	int m_StorePrice;
	
	void ExpansionGarageVehicleData()
	{
		m_TerritoryID = -1;
		m_OwnerUIDs = new array<string>;
		m_GlobalID = {0, 0, 0, 0};
	}

	void SetDataFromVehicleObject(CarScript vehicle)
	{
		if (vehicle.HasKey())
		{
			array<string> keyOwners = ExpansionGarageModule.s_Instance.GetVehicleOwnersFromKeys(vehicle);
			foreach (string uid: keyOwners)
			{
				if (m_OwnerUIDs.Find(uid) == -1)
					m_OwnerUIDs.Insert(uid);
			}
		}

		if (vehicle.ExpansionGetLastDriverUID() && vehicle.ExpansionGetLastDriverUID() != string.Empty)
		{
			if (m_OwnerUIDs.Find(vehicle.ExpansionGetLastDriverUID()) == -1)
				m_OwnerUIDs.Insert(vehicle.ExpansionGetLastDriverUID());
		}
					
		m_ClassName = vehicle.GetType();
		m_Position = vehicle.GetPosition();
		float modelZeroPointFromGround = vehicle.GetModelZeroPointDistanceFromGround();
		m_Position = Vector(m_Position[0], m_Position[1] - modelZeroPointFromGround, m_Position[2]);
		m_Orientation = vehicle.GetOrientation();
		m_SkinName = vehicle.ExpansionGetCurrentSkinName();
		m_SkinIndex = vehicle.ExpansionGetCurrentSkinIndex();
		vehicle.GetNetworkID(m_NetworkIDLow, m_NetworkIDHigh);
		m_VehicleObject = vehicle;

		if (!vehicle.m_Expansion_GlobalID.m_IsSet)
			vehicle.m_Expansion_GlobalID.Acquire();

		for (int i = 0; i < 4; i++)
			m_GlobalID[i] = vehicle.m_Expansion_GlobalID.m_ID[i];

	#ifdef EXPANSIONMODMARKET
		if (GetExpansionSettings().GetGarage().EnableMarketFeatures)
		{
			int price;
			if (GetExpansionSettings().GetGarage().StorePricePercent > 0)	
			{		
				price = ExpansionGarageModule.s_Instance.GetMarketPrice(vehicle.GetType());
			}
			else
			{
				price = GetExpansionSettings().GetGarage().StaticStorePrice;
			}
			
			m_StorePrice = price;
		}
	#endif
	}	
	
	void Copy(ExpansionGarageVehicleDataBase other)
	{
		m_OwnerUIDs.Copy(other.m_OwnerUIDs);
		m_ClassName = other.m_ClassName;
		m_Position = other.m_Position;
		m_Orientation = other.m_Orientation;

		m_GlobalID.Copy(other.m_GlobalID);
	}

	void SetTerritoryID(int id)
	{
		m_TerritoryID = id;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(m_GlobalID);
		//ctx.Write(m_TerritoryID);
		
		//ctx.Write(m_OwnerUIDs.Count());
		//foreach (string uid: m_OwnerUIDs)
		//{
			//ctx.Write(uid);
		//}
		
		ctx.Write(m_ClassName);
		ctx.Write(m_Position);
		//ctx.Write(m_Orientation);
		ctx.Write(m_SkinIndex);
		ctx.Write(m_NetworkIDLow);
		ctx.Write(m_NetworkIDHigh);
		ctx.Write(m_StorePrice);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(m_GlobalID))
			return false;

		//if (!ctx.Read(m_TerritoryID))
			//return false;
		
		//int uidCount;
		//if (!ctx.Read(uidCount))
			//return false;
		
		//for (int i = 0; i < uidCount; i++)
		//{
			//string uid;
			//if (!ctx.Read(uid))
				//return false;
			
			//m_OwnerUIDs.Insert(uid);
		//}

		if (!ctx.Read(m_ClassName))
			return false;

		if (!ctx.Read(m_Position))
			return false;

		//if (!ctx.Read(m_Orientation))
			//return false;

		if (!ctx.Read(m_SkinIndex))
			return false;

		if (!ctx.Read(m_NetworkIDLow))
			return false;

		if (!ctx.Read(m_NetworkIDHigh))
			return false;
		
		if (!GetGame().IsDedicatedServer())
			m_VehicleObject = GetGame().GetObjectByNetworkId(m_NetworkIDLow, m_NetworkIDHigh);

		if (!ctx.Read(m_StorePrice))
			return false;

		return true;
	}
	
	bool IsVehicleOwner(string playerUID)
	{
		int findIndex = -1;
		findIndex = m_OwnerUIDs.Find(playerUID);
		return (findIndex > -1);
	}
};