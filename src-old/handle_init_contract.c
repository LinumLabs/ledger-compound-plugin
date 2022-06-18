#include "compound_plugin.h"

// Called once to init.
void handle_init_contract(void *parameters) {
    // Cast the msg to the type of structure we expect (here, ethPluginInitContract_t).
    ethPluginInitContract_t *msg = (ethPluginInitContract_t *) parameters;

    // Make sure we are running a compatible version.
    if (msg->interfaceVersion != ETH_PLUGIN_INTERFACE_VERSION_LATEST) {
        // If not the case, return the `UNAVAILABLE` status.
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    // Double check that the `context_t` struct is not bigger than the maximum size (defined by
    // `msg->pluginContextLength`).
    if (msg->pluginContextLength < sizeof(context_t)) {
        PRINTF("Plugin parameters structure is bigger than allowed size\n");
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    context_t *context = (context_t *) msg->pluginContext;

    // Initialize the context (to 0).
    memset(context, 0, sizeof(*context));

    // Look for the index of the selectorIndex passed in by `msg`.
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
        case COMPOUND_MANUAL_VOTE:
            context->next_param = PROPOSAL_ID;
            break;
        case COMPOUND_VOTE_DELEGATE:
            context->next_param = DELEGATEE;
            break;
        default:
            PRINTF("Missing selectorIndex: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    // Return valid status.
    msg->result = ETH_PLUGIN_RESULT_OK;
}