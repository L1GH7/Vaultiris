import { argv, exit } from 'node:process';
import * as fs from 'fs';

const MEASUREMENTS = 100;

if(argv.length < 2) {
  console.log("[ERROR] You must pass the file path!");
  exit(1);
}

const filepath = argv[2];
fs.readFile(filepath, 'utf8', (err, data) => {
  if(err) throw err;
  
  const dataArray = data.split('\n');
  dataArray.pop(); //Empty line
  let timeUsage = dataArray.reduce((acc, measure) => Number(measure) + acc, 0);
  timeUsage /= MEASUREMENTS;
  
	// variance estimator: s = sqrt(1 / (n - 1) * sum_1^n(X[i] - mean)^2)
  let varianceEstimator = dataArray.reduce((acc, measure) => acc + Math.pow(Number(measure) - timeUsage, 2), 0);
  varianceEstimator = Math.sqrt(varianceEstimator);
  varianceEstimator /= Math.sqrt(MEASUREMENTS);

  const left = timeUsage - 1.96*varianceEstimator;
  const right = timeUsage + 1.96*varianceEstimator;
  console.log(`${timeUsage} - [${left} | ${right} ]`);
});