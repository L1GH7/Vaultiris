import { argv } from 'node:process';
import { redosAttackMinimalist } from './poc.js';

function printHelper() {
  console.log(
    `
    Usage:
    $ node test.js <stringLength> <trimStrategy> <numberOfExperiments>

    * stringLength must be at least 2.
    * trimStrategy must be one of the following:
      - old (Old Axios implementation)
      - js (New Axios (Using nodejs function) implementation)
      - built (Simulated (Using built function) new axios implementation)
    `
  )
}

function runTests() {
  if(argv.length < 5) {
    printHelper();
  }

  const stringLength = argv[2];
  const trimStrategy = argv[3];
  const n = argv[4];
  for(let i = 0; i < n; i++) {
    redosAttackMinimalist(stringLength, trimStrategy);
  }
}

runTests();