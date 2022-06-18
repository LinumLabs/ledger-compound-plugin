import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu,nano_models} from './test.fixture';

// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const abi = require(abi_path);


// Reference transaction for this test:
// https://etherscan.io/tx/0xa26b900bd6de31f61e673c4f424f952bf9b0e94ece49b09dd5e8dccb198478af

  nano_models.forEach(function(model) {
    test('[Nano ' + model.letter + '] Transfer', zemu(model, async (sim, eth) => {
   // The rawTx of the tx up above is accessible through: https://etherscan.io/getRawTx?tx=0xa26b900bd6de31f61e673c4f424f952bf9b0e94ece49b09dd5e8dccb198478af
   const tx = eth.signTransaction(
     "44'/60'/0'/0",
     "02f8b1013e84773594008504a817c8008305152e9470e36f6bf80a52b3b46b3af8e106cc0ed743e8e480b844a9059cbb000000000000000000000000fc91dc54f06a25f16d83fb07c7d2ea78d57d345a000000000000000000000000000000000000000000000000000000044d7fae3cc080a0b07f49bdaab4de25c03c93e0068679dc1af77120c78d179bb801574f9d178001a07f02a89716210e7ceaba9db71d0c01b99b686649088107d979a0e7577c342031",
   );
 
  //  const right_clicks = model.letter === 'X' ? 12 : 6;
 
   // Wait for the application to actually load and parse the transaction
   await waitForAppScreen(sim);
   // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
   await sim.navigateAndCompareSnapshots('.', model.name + 'transfer', [6, 0]);
 
   await tx;
   }));
});