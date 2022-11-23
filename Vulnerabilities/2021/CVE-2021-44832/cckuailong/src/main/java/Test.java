import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class Test {

    static {
        System.setProperty("log4j2.configurationFile","http://127.0.0.1:9202/log4j2.xml");
        System.setProperty("com.sun.jndi.ldap.object.trustURLCodebase","true");
    }
    private static final Logger logger = LogManager.getLogger(Test.class);

    public static void main(String[] args) {
    }

}
