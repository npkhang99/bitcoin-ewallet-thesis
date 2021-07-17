#ifndef BITCOIN_EWALLET_THESIS_DTOS_H
#define BITCOIN_EWALLET_THESIS_DTOS_H

#include <oatpp/core/data/mapping/type/Object.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class message_dto : public oatpp::DTO {
    DTO_INIT(message_dto, DTO /* Extends */)

    DTO_FIELD(Int32, status_code);   // Status code field
    DTO_FIELD(String, message);     // Message field
};

class fail_dto : public oatpp::DTO {
    DTO_INIT(fail_dto, DTO /* Extends */)

    DTO_FIELD(String, status);   // Status code field
    DTO_FIELD(String, message);     // Message field
};

class new_wallet_dto : public oatpp::DTO {
    DTO_INIT(new_wallet_dto, DTO);

    DTO_FIELD(String, passphrase);
};

class load_wallet_dto : public oatpp::DTO {
    DTO_INIT(load_wallet_dto, DTO);

    DTO_FIELD(String, mnemonic);
    DTO_FIELD(String, passprhase);
};

class wallet_init_dto : public oatpp::DTO {
    DTO_INIT(wallet_init_dto, DTO);

    DTO_FIELD(String, status);

    DTO_FIELD(String, mnemonic);
    DTO_FIELD(String, passphrase);
};

class balance_dto : public oatpp::DTO {
    DTO_INIT(balance_dto, DTO);

    DTO_FIELD(String, status);

    DTO_FIELD(String, available);
    DTO_FIELD(String, pending);
    DTO_FIELD(String, price);
};

#include OATPP_CODEGEN_END(DTO)

#endif //BITCOIN_EWALLET_THESIS_DTOS_H
