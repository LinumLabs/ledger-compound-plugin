#include "compound_plugin.h"

// One param functions handler
void handle_one_param_function(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case MINT_AMOUNT:
        case REDEEM_TOKENS:
        case REDEEM_AMOUNT:
        case BORROW_AMOUNT:
        case REPAY_AMOUNT:
        case CETH_AMOUNT:
            copy_parameter(context->amount, msg->parameter, sizeof(context->amount));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

// Repay borrow on behalf handler
void repay_borrow_on_behalf(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case BORROWER:  // mintAmount
            copy_address(context->dest, msg->parameter, sizeof(context->dest));
            context->next_param = REPAY_AMOUNT;
            break;
        case REPAY_AMOUNT:
            copy_parameter(context->amount, msg->parameter, sizeof(context->amount));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

// Transfer function handler
void transfer_tokens(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case RECIPIENT:  // mintAmount
            copy_address(context->dest, msg->parameter, sizeof(context->dest));
            context->next_param = AMOUNT;
            break;
        case AMOUNT:
            copy_parameter(context->amount, msg->parameter, sizeof(context->amount));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

// Liquidate borrow handler
void liquidate_borrow(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case BORROWER:  // borrower
            copy_address(context->dest, msg->parameter, sizeof(context->dest));
            context->next_param = AMOUNT;
            break;
        case AMOUNT:
            copy_parameter(context->amount, msg->parameter, sizeof(context->amount));
            context->next_param = COLLATERAL;
            break;
        case COLLATERAL:
            copy_address(context->dest, msg->parameter, sizeof(context->dest));
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

void handle_provide_parameter(void *parameters) {
    ethPluginProvideParameter_t *msg = (ethPluginProvideParameter_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;
    PRINTF("plugin provide parameter: offset %d\nBytes: %.*H\n",
           msg->parameterOffset,
           32,
           msg->parameter);

    msg->result = ETH_PLUGIN_RESULT_OK;
    if (context->selectorIndex != CETH_MINT) {
        switch (msg->parameterOffset) {
            case 4:
                memmove(context->amount, msg->parameter, 32);
                msg->result = ETH_PLUGIN_RESULT_OK;
                break;
            default:
                PRINTF("Unhandled parameter offset\n");
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
        }
    } else {
        PRINTF("CETH contract expects no parameters\n");
        msg->result = ETH_PLUGIN_RESULT_ERROR;
    }
    switch (context->selectorIndex) {
        case COMPOUND_MINT:
        case COMPOUND_REDEEM:
        case COMPOUND_REDEEM_UNDERLYING:
        case COMPOUND_BORROW:
        case COMPOUND_REPAY_BORROW:
        case CETH_MINT:
            handle_one_param_function(msg, context);
            break;
        case COMPOUND_REPAY_BORROW_ON_BEHALF:
            repay_borrow_on_behalf(msg, context);
            break;
        case COMPOUND_TRANSFER:
            transfer_tokens(msg, context);
            break;
        case COMPOUND_LIQUIDATE_BORROW:
            liquidate_borrow(msg, context);
            break;
        default:
            PRINTF("Missing selectorIndex: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
