import java.io.*;
import java.net.*;
import java.util.*;
import java.lang.Math;


public class UDPClient
{
    private static DatagramSocket clientSocket;
    private static File file;
    private static InetAddress addr;
    private static int port;

    private static FileInputStream fileInput;
    public static long packetNum;
    private static long packetCount;

    private static Packet packet;
    private static byte[] bytePacket;
    private static byte[] bytePayload;

    private static Output output;
    private static Statistic statistic;


    public static void main(String[] args) throws Exception
    {
        if (args.length != 3) 
        {
            output.printStr("Usage: program <filepath/file> <hostaddress> <hostport>\n");
            return;
        }
    
        setup(args[0], args[1], args[2]);

        output.printStr(
        	"Server: " + args[1]
        	+ "\nPort: " + args[2]
        	+ "\nFile length: " + (file.length()/1000) + " KiByte"
        	+ "\nPacket count: "+ packetCount +"\n");
        
        //-----First Packet ----------------------------------------------------------------------------
        packet = new Packet();
        packet.setHeader((byte)0x0, packetCount);
        byte[] fileCRC = checksum();
        byte[] fileName = args[0].getBytes();
        bytePayload = new byte[fileCRC.length + fileName.length];
        for(int i=0; i<fileCRC.length; i++) bytePayload[i] = fileCRC[i];
        for(int i=0; i<fileName.length; i++) bytePayload[fileCRC.length+i] = fileName[i];
        packet.setPayload(bytePayload);
        bytePacket = packet.genPacket();
        send(bytePacket);
        //----------------------------------------------------------------------------------------------
        
        statistic = new Statistic(output);
        statistic.start();
        
        //----------Informationsdaten-------------------------------------------------------------------
        bytePayload = new byte[Packet.payloadSizeMax];
        fileInput = new FileInputStream(file);
        int restLen;
        while ((restLen = (fileInput.read(bytePayload, 0, bytePayload.length))) != -1) 
        {
            packetNum++;
            if(restLen < (bytePayload.length)) break;

            packet = new Packet();
            packet.setHeader((byte)0x1, packetNum);
            packet.setPayload(bytePayload);
            bytePacket = packet.genPacket();
            send(bytePacket);
        }
        fileInput.close();
        //----------------------------------------------------------------------------------------------

        //----------Enddaten----------------------------------------------------------------------------
        packet = new Packet();
        packet.setHeader((byte)0x2, packetNum);
        byte[] rest = new byte[restLen];
        for(int i=0; i<rest.length; i++) rest[i] = bytePayload[i];
        packet.setPayload(rest);
        bytePacket = packet.genPacket();
        send(bytePacket);
        //----------------------------------------------------------------------------------------------
        
        cleanup();
    }

    public static void setup(String file, String addr, String port) throws Exception
    {
        clientSocket = new DatagramSocket();
        clientSocket.setSoTimeout(250);
        UDPClient.file = new File(file);
        UDPClient.addr = InetAddress.getByName(addr);
        UDPClient.port = Integer.parseInt(port);

        packetNum = 0;
        packetCount = (long)(UDPClient.file.length() / Packet.payloadSizeMax + 1);
    }

    private static byte[] checksum() throws Exception
    {
        CRC16 crc = new CRC16();
        byte[] buffer = new byte[4];

        fileInput = new FileInputStream(file);
        int restlen;
        while((restlen = fileInput.read(buffer)) != -1)
        {                    
            crc.update(buffer, 0, restlen);
        }
        fileInput.close();

        return Packet.intToByteArray((int)crc.getValue());
    }

    public static void cleanup()
    {
    	statistic.interrupt();
        clientSocket.close();
    }

    public static void send(byte[] packet) throws Exception
    {
        DatagramPacket sendPacket = new DatagramPacket(packet, packet.length, addr, port);

        byte[] response = new byte[1];
        DatagramPacket receivePacket = new DatagramPacket(response, response.length);
        
        while(true)
        {
            try{clientSocket.send(sendPacket);} catch(Exception e){
            	output.printStr("Error while sending\n");
            	continue;
            }

            try{clientSocket.receive(receivePacket);} catch(Exception e){
                output.printStr("Error while receiving\n");
                continue;
            }

            if(response[0] == (byte) 0x0)
            {
                //output.printStr("The transfered packet is correct\n");
                break;
            }
            else if(response[0] == (byte) 0x1)
            {
                //output.printStr("The transfered packet has an Error\n");
                continue;
            }
            else if(response[0] == (byte) 0x2)
            {
                output.printStr("The transfered file has an Error\n");
                break;
            }
            else
            {
                output.printStr("Unknown response from Server\n");
                continue;
            }
        }
    }
}