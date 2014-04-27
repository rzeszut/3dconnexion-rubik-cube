#ifndef UTIL_HPP_WYB4UF6C
#define UTIL_HPP_WYB4UF6C

#include <cstring>

#ifndef __FILENAME__
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define deleteNotNull(obj) if (obj) delete (obj)

#define deleteNotNullArr(obj) if (obj) delete[] (obj)

#endif /* end of include guard: UTIL_HPP_WYB4UF6C */

