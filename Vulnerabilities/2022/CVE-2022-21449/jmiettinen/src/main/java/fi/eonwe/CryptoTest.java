package fi.eonwe;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.Signature;
import java.util.Properties;

public class CryptoTest {

    private static final byte[] EMPTY_SIGNATURE = new byte[64];

    private static void reportVulnerability(boolean vulnerable) {
        Properties props = System.getProperties();
        String vendor = props.getProperty("java.vendor");
        String majorVersion = props.getProperty("java.version");
        String version = props.getProperty("java.runtime.version", "");
        System.out.println(vulnerable ? "VULNERABLE" : "not vulnerable");
        System.out.printf("%s (%s) %s %n", version, majorVersion, vendor);
    }

    public static void main(String[] args) throws Exception {
        KeyPair keyPair = KeyPairGenerator.getInstance("EC").generateKeyPair();
        Signature sig = Signature.getInstance("SHA256WithECDSAInP1363Format");
        sig.initVerify(keyPair.getPublic());
        sig.update("Hello, World".getBytes());

        boolean valid = sig.verify(EMPTY_SIGNATURE);
        reportVulnerability(valid);
    }
}
