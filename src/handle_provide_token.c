#include "compound_plugin.h"

#define NUM_COMPOUND_BINDINGS 9

const compoundAssetDefinition_t UNDERLYING_ASSET_DECIMALS[NUM_COMPOUND_BINDINGS] = {
    {"cDAI", 18},
    {"cETH", 18},
    {"cUSDC", 6},
    {"cZRX", 18},
    {"cUSDT", 6},
    {"cBTC", 8},
    {"cBAT", 18},
    {"cREP", 18},
    {"cSAI", 18},
};

void handle_provide_token(void *parameters) {
    ethPluginProvideInfo_t *msg = (ethPluginProvideInfo_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;
    context->decimals = 18;
    if (msg->item1) {
        // Store its ticker.
        for (size_t i = 0; i < NUM_COMPOUND_BINDINGS; i++) {
            compoundAssetDefinition_t *binding =
                (compoundAssetDefinition_t *) PIC(&UNDERLYING_ASSET_DECIMALS[i]);
            if (strncmp(binding->ticker,
                        context->ticker,
                        strnlen(binding->ticker, MAX_TICKER_LEN)) == 0) {
                context->decimals = binding->decimals;
                msg->result = ETH_PLUGIN_RESULT_OK;
            }
        }
        context->token_found = true;
    }
    if (!msg->item1 || !context->token_found) {
        // The Ethereum App did not manage to find the info for the requested token.
        context->token_found = false;

        // Default to ETH's decimals (for wei).
        context->decimals = 18;
        msg->result = ETH_PLUGIN_RESULT_OK;
        // If data wasn't found, use "???" as the ticker.
        msg->additionalScreens = 1;

        strlcpy(context->ticker,
                "Unknown token. Please contact Ledger support.",
                sizeof(context->ticker));
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}
