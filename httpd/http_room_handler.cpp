#include "http_room_handler.h"

#include "http_error.h"

using namespace std;

bool HTTP_Room_Handler::handle_post(const Request& req, Response& res)
{
    if (req[http::field::content_type] != "text/json") {
        set_error(res,
                  http::status::bad_request,
                  "This method accept only JSON type.");

        return false;
    }

    // TODO:
    // Handle JSON body.

    return true;
}
