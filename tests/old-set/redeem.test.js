import { processTest, populateTransaction } from "../src/test.fixture";

// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";
const rawTxHex = "0xf88b8211ce850df8475800830927c09470e36f6bf80a52b3b46b3af8e106cc0ed743e8e480a4db006a75000000000000000000000000000000000000000000000000000002af8edf610725a084e5076df6cc7bfb749d08da27cd031ab959ac4c58a0b3a314f5106b6ae33da9a030c16cd471d720a7420dde7ef109def72ebfa113a3c41e38b5eaddf9c3a63fcf";
// Reference transaction for this test:
// https://etherscan.io/tx/0x2caf4f71c3766b94f1d662df3da04b45e9b6d1ddca1601950471375f3d1cdeab
const testLabel = "redeem" // <= Name of the test
const testNetwork = "ethereum";
const testDirSuffix = "redeem"; // <= directory to compare device snapshots to
const signedPlugin = true;
const contractName = "Compound";
const chainID = 1;

const serializedTx = populateTransaction(contractAddr, rawTxHex, chainID);
const devices = [
  {
    name: "nanos",
    label: "nano S",
    steps: 6
  }
];

devices.forEach((device) => processTest(device, contractName, testLabel, testLabel, rawTxHex, signedPlugin, "", testNetwork));




