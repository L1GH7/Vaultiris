package pub.fuzz.utils;

import java.io.IOException;
import java.net.Socket;

public class SocketFactory {
    public static Socket newSocket(String ip,int port) throws IOException {
        return new Socket(ip,port);
    }
}
