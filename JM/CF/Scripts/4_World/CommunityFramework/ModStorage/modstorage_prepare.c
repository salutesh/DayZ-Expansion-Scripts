//! This is just here to suppress modstorage_prepare.c in CF
#ifndef EXPANSION_MODSTORAGE
modded class BuildingBase
{
    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(1);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx,version))
            return false;

        // If the persistence file is before 1.10, cf data doesn't exist
        if (version < 116) return true;

        int cf_version;
        if (!ctx.Read( cf_version )) return false;

        return true;
    }
}

modded class AnimalBase
{
    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(1);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx,version))
            return false;

        // If the persistence file is before 1.10, cf data doesn't exist
        if (version < 116) return true;

        int cf_version;
        if (!ctx.Read( cf_version )) return false;

        return true;
    }
}

modded class ZombieBase
{
    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(1);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx,version))
            return false;

        // If the persistence file is before 1.10, cf data doesn't exist
        if (version < 116) return true;

        int cf_version;
        if (!ctx.Read( cf_version )) return false;

        return true;
    }
}

modded class ItemBase
{
    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(1);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx,version))
            return false;

        // If the persistence file is before 1.10, cf data doesn't exist
        if (version < 116) return true;

        int cf_version;
        if (!ctx.Read( cf_version )) return false;

        return true;
    }
}

modded class DayZPlayerImplement
{
    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(1);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx,version))
            return false;

        // If the persistence file is before 1.10, cf data doesn't exist
        if (version < 116) return true;

        int cf_version;
        if (!ctx.Read( cf_version )) return false;

        return true;
    }
}

modded class CarScript
{
    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(1);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx,version))
            return false;

        // If the persistence file is before 1.10, cf data doesn't exist
        if (version < 116) return true;

        int cf_version;
        if (!ctx.Read( cf_version )) return false;

        return true;
    }
}

#ifndef DAYZ_1_25
modded class BoatScript
{
    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(1);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx,version))
            return false;

        // If the persistence file is before 1.10, cf data doesn't exist
        if (version < 116) return true;

        int cf_version;
        if (!ctx.Read( cf_version )) return false;

        return true;
    }
}
#endif

modded class HelicopterScript
{
    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(1);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx,version))
            return false;

        // If the persistence file is before 1.10, cf data doesn't exist
        if (version < 116) return true;

        int cf_version;
        if (!ctx.Read( cf_version )) return false;

        return true;
    }
}

modded class AdvancedCommunication
{
    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        ctx.Write(1);
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx,version))
            return false;

        // If the persistence file is before 1.10, cf data doesn't exist
        if (version < 116) return true;

        int cf_version;
        if (!ctx.Read( cf_version )) return false;

        return true;
    }
}
#endif
