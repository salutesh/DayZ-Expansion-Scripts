/**
 * ExpansionPhysicsGeometry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionPhysicsGeometry : PhysicsGeomDef
{
	void ExpansionPhysicsGeometry(string name, dGeom geom, string materialName, int layerMask)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_3(ExpansionTracing.VEHICLES, this, "ExpansionPhysicsGeometry").Add(name).Add(materialName).Add(layerMask);
#endif

		Name = name;
		Geometry = geom;
		MaterialName = materialName;
		LayerMask = layerMask;

		Frame[0] = "1 0 0";
		Frame[1] = "0 1 0";
		Frame[2] = "0 0 1";
		Frame[3] = "0 0 0";
	}

	static ExpansionPhysicsGeometry Create()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, "ExpansionPhysicsGeometry", "Create");
#endif

		return new ExpansionPhysicsGeometry("", NULL, "material/default", 0xFFFFFFFF);
	}

	static ExpansionPhysicsGeometry CreateBox(vector size)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, "ExpansionPhysicsGeometry", "CreateBox").Add(size);
#endif

		ExpansionPhysicsGeometry geom = new ExpansionPhysicsGeometry("", NULL, "material/default", 0xFFFFFFFF);
		geom.Geometry = dGeomCreateBox(size);

		return geom;
	}

	ExpansionPhysicsGeometry SetGeom(dGeom geom)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "SetGeom");
#endif

		Geometry = geom;

		return this;
	}

	ExpansionPhysicsGeometry SetMaterial(string materialName)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "SetMaterial").Add(materialName);
#endif

		MaterialName = materialName;

		return this;
	}

	ExpansionPhysicsGeometry SetMask(int layerMask)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "SetMask").Add(layerMask);
#endif

		LayerMask = layerMask;

		return this;
	}

	ExpansionPhysicsGeometry SetTransform(Transform transform)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "SetTransform").Add(transform);
#endif

		Frame[0] = transform.data[0];
		Frame[1] = transform.data[1];
		Frame[2] = transform.data[2];
		Frame[3] = transform.data[3];

		return this;
	}

	ExpansionPhysicsGeometry SetPosition(vector position)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "SetPosition").Add(position);
#endif

		Frame[3] = position;

		return this;
	}
};
