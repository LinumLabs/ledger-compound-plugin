import "core-js/stable";
import "regenerator-runtime/runtime";
import { txFromEtherscan, zemu, transactionUploadDelay} from './test.fixture';


// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const rawTxHex = "0x02f890011c843b9aca00850a34c5492683033c349470e36f6bf80a52b3b46b3af8e106cc0ed743e8e480a4a0712d68000000000000000000000000000000000000000000000005b34717b0eb495ec8c001a0b4eaad3f6f094ee88a7413fd08164416c50e92cf52c7072301f35985df1ebf80a02d59d6d4b3e335433f1bd21b5de29a21de5904e83dc293db34863f19fd7325e2";
const testLabel = "mint";
const signed = false;
const testNetwork = "ethereum";
const contractName = "Compound"
const devices = [
  {
    name: "nanos",
    label: "nano S",
    steps: 6
  }
];
// Reference transaction for this test:
// https://etherscan.io/tx/0x02d32caf20582bf1c84c941ef6c80266036034929b6947ef7f781053b411d6da

const processTransaction = async (eth, sim, steps, label, rawTxHex) => {
    const serializedTx = txFromEtherscan(rawTxHex);
    
    let tx = eth.signTransaction("44'/60'/0'/0/0", serializedTx);
  
    await sim.waitUntilScreenIsNot(
      sim.getMainMenuSnapshot(),
      transactionUploadDelay
    );
    await sim.navigateAndCompareSnapshots(".", label, [steps, 0]);
  
    await tx;
}
devices.forEach(async (device) =>  
  test(
    "[" + contractName + "] - " + device.label + " - " + testLabel,
    zemu(device.name, async (sim, eth) => {
      await processTransaction(
        eth,
        sim,
        device.steps,
        testLabel,
        rawTxHex
      );
    },signed, testNetwork)
  )
);
