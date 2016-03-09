#ifndef _SINGLETON_LOCK_H_
#define _SINGLETON_LOCK_H_

#include <boost/thread/mutex.hpp>


extern boost::mutex g_global_mutex;

#endif // _SINGLETON_LOCK_H_
