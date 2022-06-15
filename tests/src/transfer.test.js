import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, txFromEtherscan, transactionUploadDelay} from './test.fixture';


// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const rawTxHex = "02f8b1013e84773594008504a817c8008305152e9470e36f6bf80a52b3b46b3af8e106cc0ed743e8e480b844a9059cbb000000000000000000000000fc91dc54f06a25f16d83fb07c7d2ea78d57d345a000000000000000000000000000000000000000000000000000000044d7fae3cc080a0b07f49bdaab4de25c03c93e0068679dc1af77120c78d179bb801574f9d178001a07f02a89716210e7ceaba9db71d0c01b99b686649088107d979a0e7577c342031";
const testLabel = "transfer";
const testNetwork = "ethereum";
const signed = false;
const contractName = "Compound"
const devices = [
  {
    name: "nanos",
    label: "nano S",
    steps: 6
  }
];
// Reference transaction for this test:
// https://etherscan.io/tx/0xa26b900bd6de31f61e673c4f424f952bf9b0e94ece49b09dd5e8dccb198478af

// const processTransaction = async (eth, sim, steps, label, rawTxHex) => {

//   const serializedTx = txFromEtherscan(rawTxHex);

//   let tx = eth.signTransaction("44'/60'/0'/0/0", serializedTx);

//   await sim.waitUntilScreenIsNot(
//     sim.getMainMenuSnapshot(),
//     transactionUploadDelay
//   );
//   await sim.navigateAndCompareSnapshots(".", label, [steps, 0]);

//   await tx;
// }
// devices.forEach(async (device) => 
//   test(
//     "[" + contractName + "] - " + device.label + " - " + testLabel,
//     zemu(device.name, async (sim, eth) => {
//       await processTransaction(
//         eth,
//         sim,
//         device.steps,
//         testLabel,
//         rawTxHex
//       );
//     },signed, testNetwork)
//   )
// );



// Reference transaction for this test:
// https://etherscan.io/tx/0x0160b3aec12fd08e6be0040616c7c38248efb4413168a3372fc4d2db0e5961bb

// Nanos S test
test('[Nano S] Swap Exact Eth For Tokens with beneficiary', zemu("nanos", async (sim, eth) => {
  // Constants used to create the transaction
  
  // Rather than constructing the tx ourselves, one can can obtain it directly through etherscan (with a little bit of editing)
  
  const tx = eth.signTransaction(
    "44'/60'/0'/0",
    rawTxHex,
  );

  // Wait for the application to actually load and parse the transaction
  await waitForAppScreen(sim);
  // Navigate the display by pressing the right button 7 times, then pressing both buttons to accept the transaction.
  await sim.navigateAndCompareSnapshots('.', 'nanox_compound_transfer', [6, 0]);

  await tx;
}));

