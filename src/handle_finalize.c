#include "compound_plugin.h"

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    msg->tokenLookup1 = msg->pluginSharedRO->txContent->destination;
    // msg->tokenLookup1 = context->collateral;
    msg->numScreens = 3;
    msg->uiType = ETH_UI_TYPE_GENERIC;
    msg->result = ETH_PLUGIN_RESULT_OK;
    // Setting number of screens based on function
    // switch (context->selectorIndex) {
    //     case COMPOUND_MINT:
    //         msg->numScreens = 2;
    //         break;
    //     case COMPOUND_REDEEM:
    //         msg->numScreens = 2;
    //         break;
    //     case COMPOUND_REDEEM_UNDERLYING:
    //         msg->numScreens = 2;
    //         break;
    //     case COMPOUND_BORROW:
    //         msg->numScreens = 2;
    //         break;
    //     case COMPOUND_REPAY_BORROW:
    //         msg->numScreens = 2;
    //         break;
    //     case COMPOUND_REPAY_BORROW_ON_BEHALF:
    //         msg->numScreens = 1;
    //         break;
    //     case COMPOUND_TRANSFER:
    //         msg->numScreens = 1;
    //         break;
    //     case COMPOUND_LIQUIDATE_BORROW:
    //         msg->numScreens = 2;
    //         break;
    //     case CETH_MINT:
    //         msg->numScreens = 2;
    //         break;
    //     // Keep this
    //     default:
    //         msg->numScreens = 2;
    // }
    if (memcmp(msg->address, context->dest, ADDRESS_LENGTH) != 2) {
        msg->numScreens += 1;
    }
}
