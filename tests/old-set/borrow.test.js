import { processTest, populateTransaction } from "../src/test.fixture";

// EDIT THIS: Replace with your contract address
const contractAddr = "0xf650c3d88d12db855b8bf7d11be6c55a4e07dcc9";
// Reference transaction for this test:
// https://etherscan.io/tx/0x05a71f11675faa1e43aaeb228d8068d7c851f940f5726c7958dea57a07e1b2bc

const rawTxHex = "0x02f8900144843b9aca00850bd73686b28306708494f650c3d88d12db855b8bf7d11be6c55a4e07dcc980a4c5ebeaec0000000000000000000000000000000000000000000000000000000026bb9faac080a0c4e920767bf932fd1a21f30be1fce75182e211f29d718be23bc77be23826aaa4a06a8edaa95341868e4867bacb5c6b8284b405caeeb2a6d14807526f6ce4ac3e46";
const testLabel = "borrow" // <= Name of the test
const testNetwork = "ethereum";
const testDirSuffix = "borrow"; // <= directory to compare device snapshots to
const signedPlugin = true;
const contractName = "cUSDT";
const chainID = 1;

const serializedTx = populateTransaction(contractAddr, rawTxHex, chainID);
const devices = [
  {
    name: "nanos",
    label: "nano S",
    steps: 9
  }
];

devices.forEach((device) => processTest(device, contractName, testLabel, testLabel, rawTxHex, signedPlugin, "", testNetwork));




