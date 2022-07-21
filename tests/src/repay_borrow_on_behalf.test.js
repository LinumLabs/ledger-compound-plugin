import { processTest, populateTransaction } from "./test.fixture";

// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const rawTxHex = "0xf88b8211ce850df8475800830927c09470e36f6bf80a52b3b46b3af8e106cc0ed743e8e480a4db006a75000000000000000000000000000000000000000000000000000002af8edf610725a084e5076df6cc7bfb749d08da27cd031ab959ac4c58a0b3a314f5106b6ae33da9a030c16cd471d720a7420dde7ef109def72ebfa113a3c41e38b5eaddf9c3a63fcf";

// Reference transaction for this test:
// https://etherscan.io/tx/0x0160b3aec12fd08e6be0040616c7c38248efb4413168a3372fc4d2db0e5961bb
const testLabel = "repay borrow on behalf" // <= Name of the test
const testNetwork = "ethereum";
const testDirSuffix = "repay_borrow_on_behalf"; // <= directory to compare device snapshots to
const signedPlugin = true;
const contractName = "Compound";
const chainID = 1;

const serializedTx = populateTransaction(contractAddr, rawTxHex, chainID);
const devices = [
  {
    name: "nanos",
    label: "nano S",
    steps: 9
  }
];

devices.forEach((device) => processTest(device, contractName, testLabel, testDirSuffix, rawTxHex, signedPlugin, "", testNetwork));




