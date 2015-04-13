#ifndef __IO_WINVER_H_INCLUDED__
#define __IO_WINVER_H_INCLUDED__

#ifndef WINVER                  // Minimum platform is Windows XP
#define WINVER 0x0502
#endif

#ifndef _WIN32_WINNT            // Minimum platform is Windows XP
#define _WIN32_WINNT 0x0502
#endif

#ifndef _WIN32_WINDOWS          // Minimum platform is Windows XP
#define _WIN32_WINDOWS 0x0502
#endif

#ifndef WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX                // Use the standard's templated min/max
#define NOMINMAX
#endif

#ifndef _USE_MATH_DEFINES       // We do want PI defined
#define _USE_MATH_DEFINES
#endif

#endif // __IO_WINVER_H_INCLUDED__
