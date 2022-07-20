#include "compound_plugin.h"

// Sets the first screen to display.
void handle_query_contract_id(void *parameters) {
    ethQueryContractID_t *msg = (ethQueryContractID_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;
    // msg->name will be the upper sentence displayed on the screen.
    // msg->version will be the lower sentence displayed on the screen.

    // For the first screen, display the plugin name.
    strlcpy(msg->name, "Compound", msg->nameLength);

    switch (context->selectorIndex) {
        case COMPOUND_MINT:
            strlcpy(msg->version, "Mint", msg->versionLength);
            break;
        case COMPOUND_REDEEM:
            strlcpy(msg->version, "Redeem", msg->versionLength);
            break;
        case COMPOUND_REDEEM_UNDERLYING:
            strlcpy(msg->version, "Redeem underlying", msg->versionLength);
            break;
        case COMPOUND_BORROW:
            strlcpy(msg->version, "Borrow", msg->versionLength);
            break;
        case COMPOUND_REPAY_BORROW:
            strlcpy(msg->version, "Repay borrow", msg->versionLength);
            break;
        case COMPOUND_REPAY_BORROW_ON_BEHALF:
            strlcpy(msg->version, "Repay borrow on behalf", msg->versionLength);
            break;
        case COMPOUND_TRANSFER:
            strlcpy(msg->version, "Transfer", msg->versionLength);
            break;
        case COMPOUND_LIQUIDATE_BORROW:
            strlcpy(msg->version, "Liquidate borrow", msg->versionLength);
            break;
        case COMPOUND_MANUAL_VOTE:
            strlcpy(msg->version, "Manual vote", msg->versionLength);
            break;
        case COMPOUND_VOTE_DELEGATE:
            strlcpy(msg->version, "Vote delegate", msg->versionLength);
            break;

        // Keep this
        default:
            PRINTF("Selector index: %d not supported\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}