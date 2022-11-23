import { IsWhiteSpaceOrLineTerminator } from "./util.js";

/*
oldTrim implements old axios implementation.
newTrimJs implements the current axios implementation.
Src: https://github.com/axios/axios/commit/5b457116e31db0e88fede6c428e969e87f290929
*/
function oldTrim(str) {
  return str.replace(/^\s*/, '').replace(/\s*$/, '');
}
function newTrimJs(str) {
  return str.trim ? str.trim() : str.replace(/^\s+|\s+$/g, '');
}

/*
Implements the built-in javascript trim
function in order to understand what it does.

Src: https://github.com/nodejs/node/blob/e46c680bf2b211bbd52cf959ca17ee98c7f657f5/deps/v8/src/builtins/string-trim.tq
*/
function stringTrimLoop(str, startIndex, endIndex, increment) {
  let index = startIndex;

  while (true) {
    if (index == endIndex) {
      return index;
    }

    const char = str[index];
    if (!IsWhiteSpaceOrLineTerminator(char)) {
      return index;
    }

    index += increment;
  }
}

function stringTrimBody(str, leftTrim) {
  const stringLength = str.length;
  let startIndex = 0;
  let endIndex = stringLength - 1;
  if(leftTrim) {
    startIndex = stringTrimLoop(str, startIndex, stringLength, 1);
    if(startIndex == stringLength)
      return "";
  }
  else {
    endIndex = stringTrimLoop(str, endIndex, -1, -1);
    if(endIndex == -1)
      return "";
  }

  return str.substr(startIndex, endIndex + 1);
}

function newTrimBuilt(str) {
  str = stringTrimBody(str, true);
  str = stringTrimBody(str, false);

  return str;
}

/*
Receives the size of the irregular
string to be generated.
*/
function buildIrregularString(n) {
  if(n < 2) {
    console.log("[ERROR] The irregular string must have at least 2 chars!");
    return "";
  }

  let irregular = "1";
  irregular += ' '.repeat(n - 2);
  irregular += '1';
  return irregular;
}

function runTrim(string, trim) {
  let startTime, endTime;

  startTime = Date.now();
  trim(string);
  endTime = Date.now();
  
  return endTime - startTime;
}

export function redosAttack(n) {
  const irregularString = buildIrregularString(n);
  let result = `n = ${n}: \n`;
  result += `\t Old Trim: ${runTrim(irregularString, oldTrim)}ms\n`;
  result += `\t New Trim using js function: ${runTrim(irregularString, newTrimJs)} ms\n`;
  result += `\t New Trim using built function: ${runTrim(irregularString, newTrimBuilt)} ms\n`;
  console.log(result);
}

export function redosAttackMinimalist(n, trimStrategy) {
  let trimFunction;
  switch(trimStrategy) {
    case "old":
      trimFunction = oldTrim;
      break;
    case "js":
      trimFunction = newTrimJs;
      break;
    case "built":
      trimFunction = newTrimBuilt;
      break;
    default:
      console.log("[ERROR] Wrong trim strategy! Run 'node test.js' to see all strategies available.");
      return;
  }

  const irregularString = buildIrregularString(n);
  const result = runTrim(irregularString, trimFunction);
  console.log(result);
}