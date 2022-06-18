import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu,nano_models} from './test.fixture';

// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const abi = require(abi_path);


// Reference transaction for this test:
// https://etherscan.io/tx/0x2caf4f71c3766b94f1d662df3da04b45e9b6d1ddca1601950471375f3d1cdeab

  nano_models.forEach(function(model) {
    test('[Nano ' + model.letter + '] Redeem function call', zemu(model, async (sim, eth) => {
   // The rawTx of the tx up above is accessible through: https://etherscan.io/getRawTx?tx=0x2caf4f71c3766b94f1d662df3da04b45e9b6d1ddca1601950471375f3d1cdeab
   const tx = eth.signTransaction(
     "44'/60'/0'/0",
     "f88b8211ce850df8475800830927c09470e36f6bf80a52b3b46b3af8e106cc0ed743e8e480a4db006a75000000000000000000000000000000000000000000000000000002af8edf610725a084e5076df6cc7bfb749d08da27cd031ab959ac4c58a0b3a314f5106b6ae33da9a030c16cd471d720a7420dde7ef109def72ebfa113a3c41e38b5eaddf9c3a63fcf",
   );
 
   const right_clicks = model.letter === 'S' ? 12 : 6;
 
   // Wait for the application to actually load and parse the transaction
   await waitForAppScreen(sim);
   // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
   await sim.navigateAndCompareSnapshots('.', model.name + 'redeem', [right_clicks, 0]);
 
   await tx;
   }));
});