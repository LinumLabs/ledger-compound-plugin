import { processTest, populateTransaction } from "../src/test.fixture";

// EDIT THIS: Replace with your contract address
const contractAddr = "0xc00e94cb662c3520282e6f5717214004a7f26888";

// Reference transaction for this test:
// https://etherscan.io/tx/0xb327a0c3864e130d0cfd16b5e828882b381464b2329cdb08d097050c9f7fc6e5
// The rawTx of the tx up above is accessible through: https://etherscan.io/getRawTx?tx=0xb327a0c3864e130d0cfd16b5e828882b381464b2329cdb08d097050c9f7fc6e5

const rawTxHex = "0x02f89101068501bf08eb008501bf08eb008301b59494c00e94cb662c3520282e6f5717214004a7f2688880a45c19a95c000000000000000000000000b47e74e6fc3dd121b23cb0155d41dbf7de832ac5c001a0d15f04920ebfea7d68137669a2b5aaee5ac592b761f85048bdd5743e6b8a7f53a0154b06b93d2a5fcedc1ecc4d7214a54940eee418237bc07333aecd6a09b07033";
const testLabel = "delegate" // <= Name of the test
const testNetwork = "ethereum";
const testDirSuffix = "delegate"; // <= directory to compare device snapshots to
const signedPlugin = true;
const contractName = "COMP";
const chainID = 1;

const serializedTx = populateTransaction(contractAddr, rawTxHex, chainID);
const devices = [
  {
    name: "nanos",
    label: "nano S",
    steps: 7
  }
];

devices.forEach((device) => processTest(device, contractName, testLabel, testLabel, rawTxHex, signedPlugin, "", testNetwork));



