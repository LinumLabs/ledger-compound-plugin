import { processTest, populateTransaction } from "./test.fixture";

// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";
// EDIT THIS: Replace `boilerplate` with your plugin name
const abi_path = '../compound/abis/' + contractAddr + '.json';
const rawTxHex = "0xf88a819985066d169400830733d49470e36f6bf80a52b3b46b3af8e106cc0ed743e8e480a4852a12e300000000000000000000000000000000000000000000001b1ae4d6e2ef50000026a0d277740760308a1378221d77fe193c9c4e99e0cfb080b8296fde120437ba120ba03dfe161ad5f6eb65ff024d6f3f4b874a4b4ff37614d3dec307126fe45981f9f1";

// Reference transaction for this test:
// https://etherscan.io/tx/0x08d3407543117eb4531cc1721689a5792b30fb3d2b228968ef449f958ba9eaca
const testLabel = "redeem underlying" // <= Name of the test
const testNetwork = "ethereum";
const testDirSuffix = "redeem_underlying"; // <= directory to compare device snapshots to
const signedPlugin = false;
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

devices.forEach((device) =>{
  processTest(device, contractName, testLabel, testDirSuffix, "", signedPlugin, serializedTx, testNetwork);
}
);



