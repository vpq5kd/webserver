import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;
import com.sun.net.httpserver.HttpsServer;

import java.io.*;
import java.net.*;
import java.sql.SQLOutput;

public class main {
    public static void main(String [] args) throws IOException {
        HttpServer server = HttpServer.create(new InetSocketAddress("smsdesktop",80),0);
        server.createContext("/",new MyHandler());
        server.setExecutor(null);
        server.start();
        InetSocketAddress serverAddress = (InetSocketAddress) server.getAddress();
        InetAddress address = serverAddress.getAddress();
        String addressString = address.getHostAddress();
        System.out.println("Server started at local address: " + addressString + " and port " + serverAddress.getPort());
        try {
            String remoteIp = new getRemoteIp().getIp();
            System.out.println("Remote clients use http://" + remoteIp);
        }catch (Exception e){
            System.out.println("couldn't retrieve the remote IP address either because the server is down or because checkip.amazonaws.com is down.");
        }
    }
    static class getRemoteIp{
        public String getIp(){
            String ip;

            try {
                URL whatismyip = new URL("http://checkip.amazonaws.com");
                BufferedReader br = new BufferedReader(new InputStreamReader(whatismyip.openStream()));
                ip = br.readLine();
            } catch (MalformedURLException e) {
                throw new RuntimeException(e);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
            return ip;

        }
    }
    static class MyHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException
        {
            System.out.println("Incoming connection from: " + exchange.getRemoteAddress());
            System.out.println("Request Method: " + exchange.getRequestMethod());
            StringBuilder htmlResponse = new StringBuilder();
            try{
                File index = new File("C:\\Users\\server\\webserver\\index.html");
                BufferedReader br = new BufferedReader(new FileReader(index));
                String line;
                while ((line = br.readLine())!=null){
                    htmlResponse.append(line);
                }
                br.close();
            }catch(IOException e){
                e.printStackTrace();
            }
            exchange.sendResponseHeaders(200, htmlResponse.length());
            OutputStream os = exchange.getResponseBody();
            os.write(htmlResponse.toString().getBytes());
            os.close();
        }
    }
}
