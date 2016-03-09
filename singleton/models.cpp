#include "models.h"
#include "../storage/dbo/messageboard.h"


Wt::WStandardItemModel g_messageboard_model;
bool InitializePersistedGlobalResources()
{
	return modellbasen::MessageBoard::InitializeGlobalMessageboardList();
}
