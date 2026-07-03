//! @note order matters because we rely on it in ExpansionVehicleHelicopter::Simulate_Common
enum ExpansionHelicopterSimulationMode
{
	RotorDisk,
	Legacy_Extended,
	Legacy
}

enum ExpansionHelicopterSimulationAirFrictionMode
{
	Balanced,
	Legacy
}

enum ExpansionHelicopterAutoCollectiveMode
{
	Switchable,
	AlwaysOn,
	AlwaysOff
}
