#ifndef BITCOIN_EWALLET_THESIS_CONTROLLER_H
#define BITCOIN_EWALLET_THESIS_CONTROLLER_H

#include "../dto/dtos.h"

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

using namespace oatpp;

#include OATPP_CODEGEN_BEGIN(ApiController)

class controller : public web::server::api::ApiController {
public:
    controller(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, object_mapper))
            : oatpp::web::server::api::ApiController(object_mapper) {}

public:

    ENDPOINT("GET", "/", root) {
        auto dto = message_dto::createShared();
        dto->statusCode = 200;
        dto->message = "Hello World!";
        return createDtoResponse(Status::CODE_200, dto);
    }

    // Insert Your endpoints here !!!
};

#include OATPP_CODEGEN_END(ApiController)

#endif //BITCOIN_EWALLET_THESIS_CONTROLLER_H
