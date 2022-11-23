package de.rub.nds.tls.server;

import dtls.example.DtlsServer;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.GeneralSecurityException;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.UnrecoverableKeyException;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.util.Arrays;
import javax.net.ServerSocketFactory;
import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLEngine;
import javax.net.ssl.SSLServerSocket;
import javax.net.ssl.SSLServerSocketFactory;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509ExtendedTrustManager;

public class Main {

    private static ServerSocket ss;
    private static byte[] acceptedFingerprint = hexToBytes("B3EAFA469E167DDC7358CA9B54006932E4A5A654699707F68040F529637ADBC2");

    public Main(ServerSocket ss) throws IOException {
        this.ss = ss;
    }

    public static void main(String args[]) throws NoSuchAlgorithmException, KeyStoreException, FileNotFoundException, IOException, CertificateException, UnrecoverableKeyException, KeyManagementException, GeneralSecurityException {
        if (!(args.length == 1 || args.length == 2 && (!args[0].equalsIgnoreCase("TLS") && !args[0].equalsIgnoreCase("DTLS")))) {
            System.out.println("Usage: java -jar CVE-2020-2655-PoC-Server.jar [<allowedClientCertSha256Hash>]");
            System.out.println("If you do not provide a ClientCertificate hash this server is only accepting connections from client certificates"
                    + "with the SHA256 fingerprint: B3EAFA469E167DDC7358CA9B54006932E4A5A654699707F68040F529637ADBC2");
            return;
        }
        if (args.length > 1) {
            acceptedFingerprint = hexToBytes(args[1]);
            if (acceptedFingerprint.length != 32) {
                System.out.println("The fingerprint has to be exactly 32 bytes long :^)");
            }
            System.out.println("Only accepting the client certificate: " + args[0]);
        }
        if (args[0].equalsIgnoreCase("TLS")) {

            System.out.println("This PoC runs on port tcp:4433");
            int port = 4433;

            ServerSocketFactory ssf
                    = getServerSocketFactory();
            ServerSocket ss = ssf.createServerSocket(port);
            ((SSLServerSocket) ss).setNeedClientAuth(true);

            while (true) {
                try {
                    System.out.println("Waiting for clients to connect");
                    SSLSocket sslSocket = (SSLSocket) ss.accept();
                    System.out.println("A client connected");
                    sslSocket.startHandshake();
                    System.out.println("The TLS handshake was completed successfully, getting to this point should already be impossible without the private key");
                    System.out.println("The connection is valid:" + sslSocket.getSession().isValid());
                    System.out.println("The connection needs client authentication:" + sslSocket.getNeedClientAuth());
                    try {
                        Certificate[] peerCertificates = sslSocket.getSession().getPeerCertificates();
                        System.out.println("I can access the peer certificate - no exception is thrown - there is no way for the application to know that something fishy is going on :)");
                        System.out.println("The peer provided the following certificates:");
                        System.out.println("---------------------------------------------");
                        for (Certificate c : peerCertificates) {
                            System.out.println(c.toString());
                            System.out.println("---------------------------------------------");
                        }
                    } catch (Exception E) {
                        System.out.println("Cannot access peer certificate - it is detectable that the peer is not authenticated :(");
                        E.printStackTrace();
                    }
                    System.out.println("At this point the connection is completly functional - I can send and receive data as usual");
                    sslSocket.getOutputStream().write("Hello CVE-2020-2655".getBytes());
                } catch (Exception E) {
                    System.out.println("An exception occured. This happens for example if the handshake failed because the client did not provide a correct certificate or did not provide a valid siganture for the certificate.");
                    E.printStackTrace();
                }
            }
        } else {
            InetSocketAddress address = new InetSocketAddress(4433);
            DatagramSocket socket = new DatagramSocket(address);
            socket.setReuseAddress(true);
            DtlsServer server = new DtlsServer(createContext("DTLS"));
            server.runServer(socket, address);
        }
    }

    private static ServerSocketFactory getServerSocketFactory() {
        SSLServerSocketFactory ssf = null;
        try {
            SSLContext ctx = createContext("TLS");

            ssf = ctx.getServerSocketFactory();
            return ssf;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public static SSLContext createContext(String version) throws KeyStoreException, NoSuchAlgorithmException, FileNotFoundException, IOException, CertificateException, UnrecoverableKeyException, KeyManagementException {
        SSLContext ctx = SSLContext.getInstance(version);
        KeyManagerFactory kmf = KeyManagerFactory.getInstance("SunX509");
        KeyStore ks = KeyStore.getInstance("JKS");
        char[] passphrase = "password".toCharArray();

        ks.load(new FileInputStream("server.jks"), passphrase);
        kmf.init(ks, passphrase);
        /**
         * Here you usually have to define which certificates you trust - as we
         * create a server socket we need to define which client certificates we
         * trust. Per default we only trust the certificate "client_cert.pem"
         * which is contained in this repository. Note that there is no private
         * key for this certificate present - so nobody _should_ be able to
         * authenticate with this certificate.
         */
        ctx.init(kmf.getKeyManagers(), new TrustManager[]{new X509ExtendedTrustManager() {
            @Override
            public void checkClientTrusted(X509Certificate[] chain, String authType, Socket socket) throws CertificateException {
                //We only accept certificates which have the provided sha256 fingerprint. 
                for (X509Certificate cert : chain) {
                    MessageDigest instance = null;
                    try {
                        instance = MessageDigest.getInstance("SHA-256");
                    } catch (NoSuchAlgorithmException ex) {
                        ex.printStackTrace();
                    }
                    if (!Arrays.equals(instance.digest(cert.getEncoded()), acceptedFingerprint)) {
                        throw new CertificateException("The fingerprint for the provided certificate is wrong");
                    }
                }
            }

            @Override
            public void checkClientTrusted(X509Certificate[] chain, String authType) throws CertificateException {
                //We only accept certificates which have the provided sha256 fingerprint. 
                for (X509Certificate cert : chain) {
                    MessageDigest instance = null;
                    try {
                        instance = MessageDigest.getInstance("SHA-256");
                    } catch (NoSuchAlgorithmException ex) {
                        ex.printStackTrace();
                    }
                    if (!Arrays.equals(instance.digest(cert.getEncoded()), acceptedFingerprint)) {
                        //throw new CertificateException("The fingerprint for the provided certificate is wrong");
                    }
                }
            }

            @Override
            public void checkServerTrusted(X509Certificate[] chain, String authType, Socket socket) throws CertificateException {
            }

            @Override
            public void checkClientTrusted(X509Certificate[] chain, String authType, SSLEngine engine) throws CertificateException {
                //We only accept certificates which have the provided sha256 fingerprint. 
                System.out.println("This function is not called as far as I know");
                for (X509Certificate cert : chain) {
                    MessageDigest instance = null;
                    try {
                        instance = MessageDigest.getInstance("SHA-256");
                    } catch (NoSuchAlgorithmException ex) {
                        ex.printStackTrace();
                    }
                    if (!Arrays.equals(instance.digest(cert.getEncoded()), acceptedFingerprint)) {
                        //throw new CertificateException("The fingerprint for the provided certificate is wrong");
                    }
                }
            }

            @Override
            public void checkServerTrusted(X509Certificate[] chain, String authType, SSLEngine engine) throws CertificateException {
            }

            @Override
            public void checkServerTrusted(X509Certificate[] chain, String authType) throws CertificateException {
            }

            @Override
            public X509Certificate[] getAcceptedIssuers() {
                return new X509Certificate[]{};
            }
        }}, null);
        return ctx;
    }

    public static byte[] hexToBytes(String s) {
        int len = s.length();
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4)
                    + Character.digit(s.charAt(i + 1), 16));
        }
        return data;
    }
}
