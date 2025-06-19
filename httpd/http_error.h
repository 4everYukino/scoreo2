#ifndef HTTP_ERROR_H
#define HTTP_ERROR_H

#include "http_def.h"

void set_error(Response& res, http::status stuts, const char* msg, ...);

#endif
