import java.nio.charset.StandardCharsets;
import java.util.Base64;
import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.io.FileInputStream;


public class CryptoAES  {

    private static Cipher cipher = null;

    private static Cipher getCypher() throws Exception {
    if (cipher == null) {
        cipher=Cipher.getInstance("AES/CBC/PKCS5PADDING");
        cipher.init(2, new SecretKeySpec("B263DCA5FF46AD12C7C5ADB06DF2BC45".getBytes(StandardCharsets.UTF_8.name()), "AES"), new IvParameterSpec("EB4A46BCE1D40B9F"
              .getBytes(StandardCharsets.UTF_8.name())));
    }
    return cipher;
    }

    public String decrypt(String encryptedString) throws Exception {
        try {
            byte[] original = getCypher().doFinal(Base64.getDecoder().decode(encryptedString));
            return new String(original, StandardCharsets.UTF_8);
        } catch (Exception e){
            throw new Exception("Unable to load or read the source file for conversion", e);
        }
    }

    static String convertStreamToString(java.io.InputStream is) {
        if (is == null) {
            return "";
        }
    
        java.util.Scanner s = new java.util.Scanner(is);
        s.useDelimiter("\\A");
    
        String streamString = s.hasNext() ? s.next() : "";
    
        s.close();
    
        return streamString;
    }
    public static void main(String[] args) throws Exception {
        CryptoAES myCrypto = new CryptoAES();

        FileInputStream fis = new FileInputStream("AAbot_b64_encrypted.txt");
        String stringTooLong = convertStreamToString(fis);

        System.out.printf("Decrypted Text: %s", myCrypto.decrypt(stringTooLong));
    }
}


