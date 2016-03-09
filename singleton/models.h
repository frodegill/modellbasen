#ifndef _SINGLETON_MODELS_H_
#define _SINGLETON_MODELS_H_

#include <Wt/WStandardItemModel>


extern Wt::WStandardItemModel g_messageboard_model;
bool InitializePersistedGlobalResources();

#endif // _SINGLETON_MODELS_H_
