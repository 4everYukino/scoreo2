#ifndef EXPORT_H
#define EXPORT_H

#if __GNUC__ >= 4
    #define SCOREO2_EXPORT __attribute__((visibility("default")))
#else
    #define SCOREO2_EXPORT
#endif

#endif
