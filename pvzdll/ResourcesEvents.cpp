#include "pch.h"

bool onCreateAtlas(AnimationType::AnimationType type, PVZ::BaseClass def)
{
	switch (type)
	{
	case AnimationType::Peashooter:


	case AnimationType::Wallbut:
	case AnimationType::PotatoMine:
	case AnimationType::SnowPea:
	case AnimationType::Chomper:
	case AnimationType::Repeater:
	case AnimationType::Puffshroom:
	case AnimationType::Sunshroom:
	case AnimationType::Fumeshroom:

	case AnimationType::Hypnoshroom:
	case AnimationType::Scaredyshroom:
	case AnimationType::Iceshroom:

	case AnimationType::LilyPad:
	case AnimationType::Squash: case AnimationType::Squash2:
	case AnimationType::Threepeater:
	case AnimationType::TangleKelp:

	case AnimationType::Spickweed:
	case AnimationType::Torchwood:
	case AnimationType::Tallnut:
	case AnimationType::Seashroom:
	case AnimationType::Plantern:

	case AnimationType::Blover:
	case AnimationType::SplitPea:
	case AnimationType::Starfruit:

	case AnimationType::Magnetshroom:
	case AnimationType::Cabbagepult:

	case AnimationType::Kernelpult:

	case AnimationType::Garlic:
	case AnimationType::UmbrellaLeaf:
	case AnimationType::Marigold:
	case AnimationType::Melonpult:
	case AnimationType::GatlingPea:
	case AnimationType::TwinSunflower:
	case AnimationType::Gloomshroom:
	case AnimationType::Cattail:
	case AnimationType::WinterMelon:
	case AnimationType::GoldMagnet:
	case AnimationType::Spikerock:
		return false;
	default:
		return type < 0x8F;
	}
}

void onReanimCacheDrawFrame(PVZ::Animation model)
{
	model.AssignRenderGroupToPrefix(-1, "easter");
	model.AssignRenderGroupToPrefix(-1, "awake");
	model.AssignRenderGroupToPrefix(-1, "special");
	return;
}

void onAppInitAfter(PVZ::PVZApp app)
{
	ImportNewResources(app);
	app.FreePlantingCheat = true;
}

void InitResourceEvents()
{
	PVZEvent::CreateAtlasEvent((int)onCreateAtlas);
	PVZEvent::ReanimCacheDrawFrameEvent((int)onReanimCacheDrawFrame);
	PVZEvent::AppInitAfterEvent((int)onAppInitAfter);
}