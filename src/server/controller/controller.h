#ifndef BITCOIN_EWALLET_THESIS_CONTROLLER_H
#define BITCOIN_EWALLET_THESIS_CONTROLLER_H

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

#include "../../wallet/hd_wallet.h"

#include "../dto/dtos.h"
#include "../data.h"

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
        std::string passphrase = new_wallet_passphrase->passphrase->std_str();

        try {
            server::wallet = new hd_wallet();
            server::wallet->set_passphrase(passphrase);
        } catch (std::exception& e) {
            auto dto = fail_dto::createShared();
            dto->status = "failed";
            dto->message = e.what();
            return createDtoResponse(Status::CODE_400, dto);
        }

        auto dto = wallet_init_dto::createShared();
        dto->status = "ok";
        dto->mnemonic = server::wallet->get_mnemonic().c_str();
        dto->passphrase = passphrase.c_str();

        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT("POST", "/loadwallet", load_wallet,
             BODY_DTO(Object<load_wallet_dto>, wallet_init_info)) {
        OATPP_LOGD("load-wallet", "mnemonic='%s', passphrase = '%s'",
                   wallet_init_info->mnemonic->getData(),
                   wallet_init_info->passprhase->getData());
        return createResponse(Status::CODE_200, "OK");
    }

    ENDPOINT("GET", "/getbalance", get_balance) {
        auto dto = balance_dto::createShared();
        dto->status = "ok";
        return createDtoResponse(Status::CODE_200, dto);
    }

};

#include OATPP_CODEGEN_END(ApiController)

#endif //BITCOIN_EWALLET_THESIS_CONTROLLER_H
