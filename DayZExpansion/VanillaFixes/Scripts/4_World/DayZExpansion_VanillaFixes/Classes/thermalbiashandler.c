#ifndef DAYZ_1_29
//! 1.30+
modded class ThermalBiasHandler
{
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (version < 144)
			return true;  //! 1.29 or lower

		return super.OnStoreLoad(ctx, version);
	}
}
#endif
