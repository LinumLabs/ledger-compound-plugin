import { processTest, populateTransaction } from "./test.fixture";

// EDIT THIS: Replace with your contract address
const contractAddr = "0x70e36f6bf80a52b3b46b3af8e106cc0ed743e8e4";

const rawTxHex = "0x02f890011c843b9aca00850a34c5492683033c349470e36f6bf80a52b3b46b3af8e106cc0ed743e8e480a4a0712d68000000000000000000000000000000000000000000000005b34717b0eb495ec8c001a0b4eaad3f6f094ee88a7413fd08164416c50e92cf52c7072301f35985df1ebf80a02d59d6d4b3e335433f1bd21b5de29a21de5904e83dc293db34863f19fd7325e2";
// Reference transaction for this test:
// https://etherscan.io/tx/0x02d32caf20582bf1c84c941ef6c80266036034929b6947ef7f781053b411d6da
const testLabel = "mint" // <= Name of the test
const testNetwork = "ethereum";
const testDirSuffix = "mint"; // <= directory to compare device snapshots to
const signedPlugin = true;
const contractName = "Compound";
const chainID = 1;

const devices = [
  {
    name: "nanos",
    label: "nano S",
    steps: 8
  }
];
devices.forEach((device) => processTest(device, contractName, testLabel, testLabel, rawTxHex, signedPlugin, "", testNetwork));


