#include "compound_plugin.h"

// Called once to init.
void handle_init_contract(void *parameters) {
    ethPluginInitContract_t *msg = (ethPluginInitContract_t *) parameters;

    if (msg->interfaceVersion != ETH_PLUGIN_INTERFACE_VERSION_LATEST) {
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    if (msg->pluginContextLength < sizeof(context_t)) {
        PRINTF("Plugin parameters structure is bigger than allowed size\n");
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    context_t *context = (context_t *) msg->pluginContext;
    memset(context, 0, sizeof(*context));

    uint8_t i;
    for (i = 0; i < NUM_SELECTORS; i++) {
        if (memcmp((uint8_t *) PIC(COMPOUND_SELECTORS[i]), msg->selector, SELECTOR_SIZE) == 0) {
            context->selectorIndex = i;
            break;
        }
    }

    // If `i == NUM_SELECTORS` it means we haven't found the selector. Return an error.
    if (i == NUM_SELECTORS) {
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
    }

    // Set `next_param` to be the first field we expect to parse.

    switch (context->selectorIndex) {
        case COMPOUND_MINT:
            context->next_param = MINT_AMOUNT;
            break;
        case COMPOUND_REDEEM:
            context->next_param = REDEEM_TOKENS;
            break;
        case COMPOUND_REDEEM_UNDERLYING:
            context->next_param = REDEEM_AMOUNT;
            break;
        case COMPOUND_BORROW:
            context->next_param = BORROW_AMOUNT;
            break;
        case COMPOUND_REPAY_BORROW:
            context->next_param = REPAY_AMOUNT;
            break;
        case COMPOUND_REPAY_BORROW_ON_BEHALF:
            context->next_param = BORROWER;
            break;
        case COMPOUND_TRANSFER:
            context->next_param = RECIPIENT;
            break;
        case COMPOUND_LIQUIDATE_BORROW:
            context->next_param = BORROWER;
            break;
        case CETH_MINT:
            context->next_param = CETH_AMOUNT;
        default:
            PRINTF("Missing selectorIndex: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    // Return valid status.
    msg->result = ETH_PLUGIN_RESULT_OK;
}