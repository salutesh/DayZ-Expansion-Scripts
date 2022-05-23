/**
 * ExpansionObjectSet.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionObjectSet
{
	private string m_FileName;
	private string m_FilePath;
	//! All objects from the loaded file that got created
	private ref array<Object> m_ObjectEnties;
	//! Special - Fireplaces
	private ref array<EntityAI> m_FirePlaces = new array<EntityAI>;
	
	void ExpansionObjectSet(string filePath, string fileName)
	{
		m_FilePath = filePath;
		m_FileName = fileName;
	}
	
	void ~ExpansionObjectSet()
	{
		Delete();
	}
	
	void Delete()
	{
		foreach (Object obj: m_ObjectEnties)
		{
			GetGame().ObjectDelete(obj);
		}
	}
	
	void DeleteSpecial()
	{
		foreach (EntityAI entity: m_FirePlaces)
		{
			GetGame().ObjectDelete(entity);
		}
	}
	
	void SpawnObjects()
	{
		string filePath = m_FilePath + m_FileName + ".map";
		if (!FileExist(filePath))
			return;
		
		FileHandle file = OpenFile(filePath, FileMode.READ);
		if (!file)
			return;
		
		Object obj;
		string className;
		vector position;
		vector rotation;
		string special;
		
		if (!m_ObjectEnties)
			m_ObjectEnties = new array<Object>;
				
		while (GetObjectFromMissionFile(file, className, position, rotation, special))
		{
			obj = GetGame().CreateObjectEx(className, position, ECE_CREATEPHYSICS);

			if (!obj)
				continue;

			obj.SetFlags(EntityFlags.STATIC, false);

			obj.SetPosition(position);
			obj.SetOrientation(rotation);

			FixObjectCollision(obj);

			if (obj.CanAffectPathgraph())
			{
				obj.SetAffectPathgraph(true, false);
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM ).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, obj);
			}
			
			EntityAI entityAI = EntityAI.Cast( obj );
			if (entityAI)
				entityAI.SetLifetime(1.0);

			if (special == "true")
				ProcessMissionObject(obj);

			m_ObjectEnties.Insert(obj);
		}

		CloseFile(file);
	}
	
	private bool GetObjectFromMissionFile(FileHandle file, out string name, out vector position, out vector rotation, out string special = "false")
	{
		string line;
		int lineSize = FGets(file, line);

		if (lineSize < 1)
			return false;
		
		TStringArray tokens = new TStringArray;
		line.Split( "|", tokens );

		name = tokens.Get(0);
		position = tokens.Get(1).ToVector();
		rotation = tokens.Get(2).ToVector();
		special = tokens.Get(3);
		
		return true;
	}

	private void FixObjectCollision( Object obj )
	{
		vector roll = obj.GetOrientation();
		roll[2] = roll[2] - 1;
		obj.SetOrientation( roll );
		roll[2] = roll[2] + 1;
		obj.SetOrientation( roll );
	}
	
	private void ProcessMissionObject(Object obj)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MAPPING, "ExpansionObjectSet", "ProcessMissionObject");
#endif

		ItemBase item;

		if (obj.IsInherited(ExpansionPointLight))
		{
			ExpansionPointLight light = ExpansionPointLight.Cast(obj);
			if (light)
			{
				light.SetDiffuseColor(1,0,0);
			}
		}
		else if (obj.IsKindOf("Fireplace"))
		{
			Fireplace fireplace = Fireplace.Cast(obj);
			if (fireplace)
			{
				//! Add bark
				item = ItemBase.Cast(fireplace.GetInventory().CreateAttachment("Bark_Oak"));
				item.SetQuantity(8);
				//! Add firewood
				item = ItemBase.Cast(fireplace.GetInventory().CreateAttachment("Firewood"));
				item.SetQuantity(6);  //! Can only increase stack over 1 AFTER it has been attached because stack max depends on slot!
				//! Add sticks
				item = ItemBase.Cast(fireplace.GetInventory().CreateAttachment("WoodenStick"));
				item.SetQuantity(10);  //! Can only increase stack over 5 AFTER it has been attached because stack max depends on slot!

				fireplace.StartFire();
			}
		}
		else if (obj.IsInherited(BarrelHoles_ColorBase))
		{
			BarrelHoles_Red barrel = BarrelHoles_Red.Cast(obj);
			if (barrel) 
			{
				//! Need to open barrel first, otherwise can't add items
				barrel.Open();

				//! Add bark
				item = ItemBase.Cast(barrel.GetInventory().CreateAttachment("Bark_Oak"));
				item.SetQuantity(8);
				//! Add firewood
				item = ItemBase.Cast(barrel.GetInventory().CreateAttachment("Firewood"));
				item.SetQuantity(6);  //! Can only increase stack over 1 AFTER it has been attached because stack max depends on slot!
				//! Add sticks
				item = ItemBase.Cast(barrel.GetInventory().CreateAttachment("WoodenStick"));
				item.SetQuantity(10);  //! Can only increase stack over 5 AFTER it has been attached because stack max depends on slot!

				barrel.StartFire();
			}
		}
		else if (obj.IsKindOf("Roadflare"))
		{
			Roadflare flare = Roadflare.Cast( obj );
			if (flare) 
			{
				flare.GetCompEM().SetEnergy(999999);
				flare.GetCompEM().SwitchOn();
				flare.SwitchLight(false); //! Flickering
			}
		}
	#ifdef EXPANSIONMODMAPASSETS
		else if (obj.IsInherited(BuildingWithFireplace))
		{
			BuildingWithFireplace buildingWithFireplace;
			bldr_land_misc_barel_fire_1 barel_1;
			bldr_land_misc_barel_fire_2 barel_2;
			bldr_land_misc_barel_fire_3 barel_3;
			bldr_land_misc_barel_fire_4 barel_4;

			if (Class.CastTo(barel_1, obj))
				buildingWithFireplace = barel_1;
			else if (Class.CastTo(barel_2, obj))
				buildingWithFireplace = barel_2;
			else if (Class.CastTo(barel_3, obj))
				buildingWithFireplace = barel_3;
			else if (Class.CastTo(barel_4, obj))
				buildingWithFireplace = barel_4;

			if (buildingWithFireplace) 
			{
				int fire_point_index = 1;
				vector fire_place_pos = buildingWithFireplace.GetSelectionPositionMS(FireplaceIndoor.FIREPOINT_FIRE_POSITION + fire_point_index.ToString());
				vector fire_place_pos_world = buildingWithFireplace.ModelToWorld(fire_place_pos);	
				vector smoke_point_pos = buildingWithFireplace.GetSelectionPositionMS(FireplaceIndoor.FIREPOINT_SMOKE_POSITION + fire_point_index.ToString());
				vector smoke_point_pos_world = buildingWithFireplace.ModelToWorld(smoke_point_pos);		
				vector smokePos = smoke_point_pos_world;
				
				Object obj_fireplace = GetGame().CreateObjectEx("FireplaceIndoor", fire_place_pos_world, ECE_PLACE_ON_SURFACE|ECE_NOLIFETIME);
				m_FirePlaces.Insert(EntityAI.Cast(obj_fireplace));
				
				FireplaceIndoor fp_indoor = FireplaceIndoor.Cast(obj_fireplace);
				if (fp_indoor)
				{
					fp_indoor.SetFirePointIndex(fire_point_index);
					fp_indoor.SetSmokePointPosition(smokePos);
					fp_indoor.SetOrientation("0 0 0");

					//! Add bark
					item = ItemBase.Cast(fp_indoor.GetInventory().CreateAttachment("Bark_Oak"));
					item.SetQuantity(8);
					//! Add firewood
					item = ItemBase.Cast(fp_indoor.GetInventory().CreateAttachment("Firewood"));
					item.SetQuantity(6);  //! Can only increase stack over 1 AFTER it has been attached because stack max depends on slot!
					//! Add sticks
					item = ItemBase.Cast(fp_indoor.GetInventory().CreateAttachment("WoodenStick"));
					item.SetQuantity(10);  //! Can only increase stack over 5 AFTER it has been attached because stack max depends on slot!
					
					fp_indoor.StartFire();
				}
			}
		}
		#endif
	}
};
