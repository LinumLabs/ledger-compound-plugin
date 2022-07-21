import { processTest, populateTransaction } from "../src/test.fixture";

// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";
// Reference transaction for this test:
// https://etherscan.io/tx/0x05a71f11675faa1e43aaeb228d8068d7c851f940f5726c7958dea57a07e1b2bc

const rawTxHex = "0x02f89001248440207e0785072d303d41830586249470e36f6bf80a52b3b46b3af8e106cc0ed743e8e480a4c5ebeaec00000000000000000000000000000000000000000000000161e232e52c760000c080a05caafea5cd1bde52232fb2703111af375943420bc2471feeb3c7c773427577d6a028dc272346cdde78da0916fb92464cfe65ddf0910c409c067fb7fd0a5983c6b3";
const testLabel = "borrow" // <= Name of the test
const testNetwork = "ethereum";
const testDirSuffix = "borrow"; // <= directory to compare device snapshots to
const signedPlugin = true;
const contractName = "cCOMP";
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




