import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu,nano_models} from './test.fixture';

// EDIT THIS: Replace with your contract address
const contractAddr = "0xc0da01a04c3f3e0be433606045bb7017a7323e38";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const abi = require(abi_path);


// Reference transaction for this test:
// https://etherscan.io/tx/0xa9680976fbebfb6b5777d6b5545769eb3f112da75b8fc9c3c7546517cbffb755

  nano_models.forEach(function(model) {
    test('[Nano ' + model.letter + '] Cast vote', zemu(model, async (sim, eth) => {
   // The rawTx of the tx up above is accessible through: https://etherscan.io/getRawTx?tx=0xa9680976fbebfb6b5777d6b5545769eb3f112da75b8fc9c3c7546517cbffb755
   const tx = eth.signTransaction(
     "44'/60'/0'/0",
     "f8ab81a08513ca6512008301195494c0da01a04c3f3e0be433606045bb7017a7323e3880b84415373e3d000000000000000000000000000000000000000000000000000000000000002a000000000000000000000000000000000000000000000000000000000000000126a04aec2c90c0648c84bd966601df71b94d30c2a1d33591e622b8c89c2b6560eecba073e13e16b8879ed8fc5fc979c5d4b908b31aba1d25c4bedac04d6fd859688806",
   );
 
   const right_clicks = model.letter === 'S' ? 12 : 6;
 
   // Wait for the application to actually load and parse the transaction
   await waitForAppScreen(sim);
   // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
   await sim.navigateAndCompareSnapshots('.', model.name + 'manual_vote', [right_clicks, 0]);
 
   await tx;
   }));
});