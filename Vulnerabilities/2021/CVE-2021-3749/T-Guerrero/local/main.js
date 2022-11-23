import { redosAttack } from './poc.js';
import readline from 'readline';
import { exit } from 'process';

function recursiveRead() {
  rl.question('Escolha o tamanho da string irregular desejado (0 para sair): ', n => {
    if(n == 0)
      exit(0);
    redosAttack(n);
    recursiveRead();
  });
}

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

recursiveRead();