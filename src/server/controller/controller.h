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
        dto->status_code = 200;
        dto->message = "Hello World!";
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("POST", "/newwallet", create_wallet,
             BODY_DTO(Object<new_wallet_dto>, new_wallet_passphrase)) {
        OATPP_LOGD("Test", "passphrase='%s'", new_wallet_passphrase->passphrase->getData());
        return createResponse(Status::CODE_200, "OK");
    }
};

#include OATPP_CODEGEN_END(ApiController)

#endif //BITCOIN_EWALLET_THESIS_CONTROLLER_H
