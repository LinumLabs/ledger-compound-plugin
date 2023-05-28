#include "compound_plugin.h"

// EDIT THIS: You need to adapt / remove the static functions (set_send_ui, set_receive_ui ...) to
// match what you wish to display.

// Set UI for the "Send" screen.
// EDIT THIS: Adapt / remove this function to your needs.
static void set_asset_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Send", msg->titleLength);

    uint8_t i;
    compoundAssetDefinition_t *currentAsset = NULL;
    for (i = 0; i < NUM_COMPOUND_ASSETS; i++) {
        currentAsset = (compoundAssetDefinition_t *) PIC(&COMPOUND_ASSETS[i]);
        if (memcmp(currentAsset->address, context->asset, ADDRESS_LENGTH) == 0) {
            context->decimals = currentAsset->decimals;
            memcpy(context->ticker, currentAsset->assetName, MAX_VAULT_TICKER_LEN);
        }
    }

    if (context->vault[0] == '\0') {
        PRINTF("Received an invalid vault\n");
        msg->result = ETH_PLUGIN_RESULT_ERROR;
    }

    // Converts the uint256 number located in `eth_amount` to its string representation and
    // copies this to `msg->msg`.
    // amountToString(eth_amount, eth_amount_size, WEI_TO_ETHER, "ETH ", msg->msg, msg->msgLength);
}

// Set UI for "Receive" screen.
// EDIT THIS: Adapt / remove this function to your needs.
static void set_send_asset(ethQueryContractUI_t *msg, const context_t *context) {
    strlcpy(msg->title, "Amount", msg->titleLength);

    uint8_t decimals = context->decimals;
    const char *ticker = context->ticker;

    // If the token look up failed, use the default network ticker along with the default decimals.
    if (!context->token_found) {
        decimals = 18;
        ticker = msg->network_ticker;
    }

    amountToString(context->amount,
                   sizeof(context->amount),
                   decimals,
                   ticker,
                   msg->msg,
                   msg->msgLength);
}

// Set UI for "Beneficiary" screen.
// EDIT THIS: Adapt / remove this function to your needs.
static void set_beneficiary_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Asset", msg->titleLength);

    // Prefix the address with `0x`.
    msg->msg[0] = '0';
    msg->msg[1] = 'x';

    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;

    // Get the string representation of the address stored in `context->beneficiary`. Put it in
    // `msg->msg`.
    getEthAddressStringFromBinary(
        context->beneficiary,
        msg->msg + 2,  // +2 here because we've already prefixed with '0x'.
        msg->pluginSharedRW->sha3,
        chainid);
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    msg->result = ETH_PLUGIN_RESULT_OK;

    // EDIT THIS: Adapt the cases for the screens you'd like to display.
    // switch(context->selectorIndex){
    //     case SUPPLY:
        
    // }
    switch (msg->screenIndex) {
        case 0:
            set_asset_ui(msg, context);
            break;
        case 1:
            set_send_ui(msg, context);
            break;
        // Keep this
        default:
            PRINTF("Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
