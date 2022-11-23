import java.lang.Runtime;
import java.lang.Process;

public class TouchFile {
  static {
      try {
          Runtime rt = Runtime.getRuntime();
          String[] commands = {"touch", "/tmp/flag_test"};
          Process pc = rt.exec(commands);
          pc.waitFor();
      } catch (Exception e) {
          // do nothing
      }
  }
}
