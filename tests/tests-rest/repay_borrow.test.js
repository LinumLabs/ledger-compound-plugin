import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu,nano_models} from './test.fixture';

// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const abi = require(abi_path);


// Reference transaction for this test:
// https://etherscan.io/tx/0x863beb87fc6fa03b6557530142a3e134c3890a8922bab0df5802e4d11a13e4b8

  nano_models.forEach(function(model) {
    test('[Nano ' + model.letter + '] Repay borrow', zemu(model, async (sim, eth) => {
   // The rawTx of the tx up above is accessible through: https://etherscan.io/getRawTx?tx=0x863beb87fc6fa03b6557530142a3e134c3890a8922bab0df5802e4d11a13e4b8
   const tx = eth.signTransaction(
     "44'/60'/0'/0",
     "02f890012c843b9aca008505616adcb48302ee149470e36f6bf80a52b3b46b3af8e106cc0ed743e8e480a40e75270200000000000000000000000000000000000000000000000006d660efa683526ec080a0cc314148699d9196704ab35eb49152a2da323cb93a3f8490dede87f3ca44998fa02597356ef56706d70285adcd334ee9ad570523ec36105dac09b782a8ebf5c7eb",
   );
 
   const right_clicks = model.letter === 'S' ? 12 : 9;
 
   // Wait for the application to actually load and parse the transaction
   await waitForAppScreen(sim);
   // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
   await sim.navigateAndCompareSnapshots('.', model.name + 'repay', [right_clicks, 0]);
 
   await tx;
   }));
});