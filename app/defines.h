#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <memory>
#include <string>
#include <stdint.h>


namespace modellbasen
{

#define SERVER_LOCALE "nb_NO"

#define PI (3.1415926535897932385) 
#define DEG2RAD(d)	((d) * PI/180.0)


#define UNUSED(x) /* make compiler happy */

#define MIN(x,y) ((x)<(y)?(x):(y))

#define EQUAL (0)

#define DEFAULT_PADDING (8)

#define MAX_UPLOAD_SIZE (-1)

#define INVALID_ID (static_cast<Poco::UInt32>(0))

} // namespace modellbasen

#endif // _DEFINES_H_
