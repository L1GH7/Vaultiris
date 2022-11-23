//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//

package org.apache.xmlrpc.server;

import org.apache.xmlrpc.XmlRpcException;
import org.apache.xmlrpc.XmlRpcHandler;
import org.apache.xmlrpc.XmlRpcRequest;
import org.apache.xmlrpc.common.TypeConverterFactory;
import org.apache.xmlrpc.common.TypeConverterFactoryImpl;
import org.apache.xmlrpc.metadata.ReflectiveXmlRpcMetaDataHandler;
import org.apache.xmlrpc.metadata.Util;
import org.apache.xmlrpc.metadata.XmlRpcListableHandlerMapping;
import org.apache.xmlrpc.metadata.XmlRpcMetaDataHandler;
import org.apache.xmlrpc.server.RequestProcessorFactoryFactory.RequestProcessorFactory;
import org.apache.xmlrpc.server.RequestProcessorFactoryFactory.RequestSpecificProcessorFactoryFactory;

import java.lang.reflect.Method;
import java.util.*;
import java.util.Map.Entry;

public abstract class AbstractReflectiveHandlerMapping implements XmlRpcListableHandlerMapping {
    private TypeConverterFactory typeConverterFactory = new TypeConverterFactoryImpl();
    protected Map handlerMap = new HashMap();
    private AbstractReflectiveHandlerMapping.AuthenticationHandler authenticationHandler;
    private RequestProcessorFactoryFactory requestProcessorFactoryFactory = new RequestSpecificProcessorFactoryFactory();
    private boolean voidMethodEnabled;

    public AbstractReflectiveHandlerMapping() {
    }

    public void setTypeConverterFactory(TypeConverterFactory pFactory) {
        this.typeConverterFactory = pFactory;
    }

    public TypeConverterFactory getTypeConverterFactory() {
        return this.typeConverterFactory;
    }

    public void setRequestProcessorFactoryFactory(RequestProcessorFactoryFactory pFactory) {
        this.requestProcessorFactoryFactory = pFactory;
    }

    public RequestProcessorFactoryFactory getRequestProcessorFactoryFactory() {
        return this.requestProcessorFactoryFactory;
    }

    public AbstractReflectiveHandlerMapping.AuthenticationHandler getAuthenticationHandler() {
        return this.authenticationHandler;
    }

    public void setAuthenticationHandler(AbstractReflectiveHandlerMapping.AuthenticationHandler pAuthenticationHandler) {
        this.authenticationHandler = pAuthenticationHandler;
    }

//    protected boolean isHandlerMethod(Method pMethod) {
//        if (!Modifier.isPublic(pMethod.getModifiers())) {
//            return false;
//        } else if (Modifier.isStatic(pMethod.getModifiers())) {
//            return false;
//        } else if (!this.isVoidMethodEnabled() && pMethod.getReturnType() == Void.TYPE) {
//            return false;
//        } else {
//            return pMethod.getDeclaringClass() != (class$java$lang$Object == null ? (class$java$lang$Object = class$("java.lang.Object")) : class$java$lang$Object);
//        }
//    }

//    protected void registerPublicMethods(String pKey, Class pType) throws XmlRpcException {
//        Map map = new HashMap();
//        Method[] methods = pType.getMethods();
//
//        String name;
//        Method[] mArray;
//        for(int i = 0; i < methods.length; ++i) {
//            Method method = methods[i];
//            if (this.isHandlerMethod(method)) {
//                name = pKey + "." + method.getName();
//                Method[] oldMArray = (Method[])((Method[])map.get(name));
//                if (oldMArray == null) {
//                    mArray = new Method[]{method};
//                } else {
//                    mArray = new Method[oldMArray.length + 1];
//                    System.arraycopy(oldMArray, 0, mArray, 0, oldMArray.length);
//                    mArray[oldMArray.length] = method;
//                }
//
//                map.put(name, mArray);
//            }
//        }
//
//        Iterator iter = map.entrySet().iterator();
//
//        while(iter.hasNext()) {
//            Entry entry = (Entry)iter.next();
//            name = (String)entry.getKey();
//            mArray = (Method[])((Method[])entry.getValue());
//            this.handlerMap.put(name, this.newXmlRpcHandler(pType, mArray));
//        }
//
//    }

    protected XmlRpcHandler newXmlRpcHandler(Class pClass, Method[] pMethods) throws XmlRpcException {
        String[][] sig = this.getSignature(pMethods);
        String help = this.getMethodHelp(pClass, pMethods);
        RequestProcessorFactory factory = this.requestProcessorFactoryFactory.getRequestProcessorFactory(pClass);
        return (XmlRpcHandler)(sig != null && help != null ? new ReflectiveXmlRpcMetaDataHandler(this, this.typeConverterFactory, pClass, factory, pMethods, sig, help) : new ReflectiveXmlRpcHandler(this, this.typeConverterFactory, pClass, factory, pMethods));
    }

    protected String[][] getSignature(Method[] pMethods) {
        return Util.getSignature(pMethods);
    }

    protected String getMethodHelp(Class pClass, Method[] pMethods) {
        return Util.getMethodHelp(pClass, pMethods);
    }

    public XmlRpcHandler getHandler(String pHandlerName) throws XmlRpcNoSuchHandlerException, XmlRpcException {
        XmlRpcHandler result = (XmlRpcHandler)this.handlerMap.get(pHandlerName);
        if (result == null) {
                throw new XmlRpcNoSuchHandlerException("r00t4dm");
        } else {
            return result;
        }
    }

    public String[] getListMethods() throws XmlRpcException {
        List list = new ArrayList();
        Iterator iter = this.handlerMap.entrySet().iterator();

        while(iter.hasNext()) {
            Entry entry = (Entry)iter.next();
            if (entry.getValue() instanceof XmlRpcMetaDataHandler) {
                list.add(entry.getKey());
            }
        }

        return (String[])((String[])list.toArray(new String[list.size()]));
    }

//    public String getMethodHelp(String pHandlerName) throws XmlRpcException {
//        XmlRpcHandler h = this.getHandler(pHandlerName);
//        if (h instanceof XmlRpcMetaDataHandler) {
//            return ((XmlRpcMetaDataHandler)h).getMethodHelp();
//        } else {
//            throw new XmlRpcNoSuchHandlerException("No help available for method: " + pHandlerName);
//        }
//    }
//
//    public String[][] getMethodSignature(String pHandlerName) throws XmlRpcException {
//        XmlRpcHandler h = this.getHandler(pHandlerName);
//        if (h instanceof XmlRpcMetaDataHandler) {
//            return ((XmlRpcMetaDataHandler)h).getSignatures();
//        } else {
//            throw new XmlRpcNoSuchHandlerException("No metadata available for method: " + pHandlerName);
//        }
//    }

    public boolean isVoidMethodEnabled() {
        return this.voidMethodEnabled;
    }

    public void setVoidMethodEnabled(boolean pVoidMethodEnabled) {
        this.voidMethodEnabled = pVoidMethodEnabled;
    }

    public interface AuthenticationHandler {
        boolean isAuthorized(XmlRpcRequest var1) throws XmlRpcException;
    }
}
