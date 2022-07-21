import { processTest, populateTransaction } from "./test.fixture";

// EDIT THIS: Replace with your contract address
const contractAddr = "0x5d3a536e4d6dbd6114cc1ead35777bab948e3643";

const rawTxHex = "0x02f89201820222843b9aca00850868c124c683038a54945d3a536e4d6dbd6114cc1ead35777bab948e364380a4a0712d68000000000000000000000000000000000000000000000000a3e32d330314f99bc080a0d9ad07a04091d78eee2db8cc0f44ab4949cd63fd81e8d43e2bde17ce02e492a1a04c0bd0ecfbea9f7209942cc4fb6ecb2b321c572be87ad4ff28ee37179b3d4763";
// Reference transaction for this test:
// https://etherscan.io/tx/0x02d32caf20582bf1c84c941ef6c80266036034929b6947ef7f781053b411d6da
const testLabel = "mint" // <= Name of the test
const testNetwork = "ethereum";
const testDirSuffix = "mint"; // <= directory to compare device snapshots to
const signedPlugin = true;
const contractName = "Compound DAI";
const chainID = 1;

const devices = [
  {
    name: "nanos",
    label: "nano S",
    steps: 8
  }
];
devices.forEach((device) => processTest(device, contractName, testLabel, testLabel, rawTxHex, signedPlugin, "", testNetwork));


