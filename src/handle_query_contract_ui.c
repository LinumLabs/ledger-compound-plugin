#include "compound_plugin.h"

// Set UI for "Address" screen.
void set_address_ui(ethQueryContractUI_t *msg, context_t *context) {
    msg->msg[0] = '0';
    msg->msg[1] = 'x';
    uint64_t chainid = 0;
    getEthAddressStringFromBinary(context->dest,
                                  (uint8_t *) msg->msg + 2,
                                  msg->pluginSharedRW->sha3,
                                  chainid);
}

// Set UI for First param screen
void set_param_ui_amount(ethQueryContractUI_t *msg, context_t *context, char *title) {
    strlcpy(msg->title, title, msg->titleLength);
    amountToString(context->amount,
                   sizeof(context->amount),
                   context->decimals,
                   context->ticker,
                   msg->msg,
                   100);
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, 100);

    msg->result = ETH_PLUGIN_RESULT_OK;

    switch (context->selectorIndex) {
        case COMPOUND_MINT:
            switch (msg->screenIndex) {
                case 0:
                    set_param_ui_amount(msg, context, "Lend");
                    break;
                default:
                    PRINTF("Selector index: %d not supported\n", context->selectorIndex);
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
            }
            break;
        case COMPOUND_REDEEM:
            switch (msg->screenIndex) {
                case 0:
                    set_param_ui_amount(msg, context, "Redeem");
                    break;
                default:
                    PRINTF("Selector index: %d not supported\n", context->selectorIndex);
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
            }
            break;
        case COMPOUND_REDEEM_UNDERLYING:
            switch (msg->screenIndex) {
                case 0:
                    set_param_ui_amount(msg, context, "Redeem underlying");
                    break;
                default:
                    PRINTF("Selector index: %d not supported\n", context->selectorIndex);
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
            }
            break;
        case COMPOUND_BORROW:
            switch (msg->screenIndex) {
                case 0:
                    set_param_ui_amount(msg, context, "Borrow");
                    break;
                default:
                    PRINTF("Selector index: %d not supported\n", context->selectorIndex);
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
            }
            break;
        case COMPOUND_REPAY_BORROW:
            switch (msg->screenIndex) {
                case 0:
                    set_param_ui_amount(msg, context, "Repay borrow");
                    break;
                default:
                    PRINTF("Selector index: %d not supported\n", context->selectorIndex);
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
            }
            break;
        case COMPOUND_REPAY_BORROW_ON_BEHALF:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Borrower.", msg->titleLength);
                    set_address_ui(msg, context);
                    break;
                case 1:
                    strlcpy(msg->title, "Repaying amount.", msg->titleLength);
                    break;
                default:
                    PRINTF("Selector index: %d not supported\n", context->selectorIndex);
                    break;
            }
            break;
        case COMPOUND_TRANSFER:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Recipient.", msg->titleLength);
                    set_address_ui(msg, context);
                    break;
                case 1:
                    strlcpy(msg->title, "Amount.", msg->titleLength);
                    break;
                default:
                    PRINTF("Selector index: %d not supported\n", context->selectorIndex);
                    break;
            }
            break;
        case COMPOUND_LIQUIDATE_BORROW:
            switch (msg->screenIndex) {
                case 0:
                    strlcpy(msg->title, "Borrower.", msg->titleLength);
                    set_address_ui(msg, context);
                    break;
                case 1:
                    strlcpy(msg->title, "Amount.", msg->titleLength);
                    break;
                case 2:
                    strlcpy(msg->title, "Collateral.", msg->titleLength);
                    set_address_ui(msg, context);
                    break;
                default:
                    PRINTF("Selector index: %d not supported\n", context->selectorIndex);
                    break;
            }
            break;
        case CETH_MINT:
            switch (msg->screenIndex) {
                case 0:
                    set_param_ui_amount(msg, context, "Mint cETH");
                    break;
                default:
                    PRINTF("Selector index: %d not supported\n", context->selectorIndex);
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
            }
            break;
        default:
            PRINTF("Selector index: %d not supported\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}
