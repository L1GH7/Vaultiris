// import com.sun.org.apache.bcel.internal.*;  // Constants.MAX_SHORT == 65535
import com.sun.org.apache.bcel.internal.classfile.*;
import com.sun.org.apache.bcel.internal.generic.*;

public class Bcel
{
    public static void main(String[] args)
    {
        ClassGen classGen = new ClassGen("HelloWorld", "java/lang/Object", "HelloWorld.java", 1, null);

        classGen.addEmptyConstructor(1);

        ConstantPoolGen constantPoolGen = classGen.getConstantPool();

        // Works or fails only with JDK and/or javac at build time:
        // System.out.println(constantPoolGen.CONSTANT_POOL_SIZE);
        
        int k = 0;
        for (; k < 65535 + 10; k++)
        {
             try
             {
                 constantPoolGen.addInteger(k);
             }
             catch (RuntimeException e)
             {
                 System.out.println("Good! :)");
                 System.out.println("Number of added elements: " + String.valueOf(k));
                 System.out.println("constantPoolGen.getSize(): " + constantPoolGen.getSize());
                 System.out.println("Constants.MAX_SHORT: 65535");
                 throw e;
             }
        }
        System.out.println("Bad! :(");
        System.out.println("Number of added elements: " + String.valueOf(k));
        System.out.println("constantPoolGen.getSize(): " + constantPoolGen.getSize());
        System.out.println("Constants.MAX_SHORT: 65535");
    }
}

