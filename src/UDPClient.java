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

    private static Statistic statistic;

    public static void main(String[] args) throws Exception
    {
        if (args.length != 3) 
        {
            Output.printStr("Usage: program <filepath/file> <hostaddress> <hostport>\n");
            return;
        }
    
        setup(args[0], args[1], args[2]);

        Output.printStr(
        	"Server: " + args[1]
        	+ "\nPort: " + args[2]
        	+ "\nFile length: " + (file.length()/1000) + " KiByte"
        	+ "\nPacket count: "+ packetCount + "\n");
        
        sendFile(args[0]);
        
        cleanup();
    }

    public static void setup(String file, String addr, String port)
    {
        try {
            clientSocket = new DatagramSocket();
            clientSocket.setSoTimeout(250);
            UDPClient.file = new File(file);
            UDPClient.addr = InetAddress.getByName(addr);
            UDPClient.port = Integer.parseInt(port);
            packetNum = 0;
            packetCount = (long)(UDPClient.file.length() / Packet.payloadSizeMax + 1);
        } catch(Exception e){
            Output.printStr("Error during setup\n");
        }
    }

    private static void sendFile(String file)
    {
        try {
            //----- First Packet -------------------------------------------------------------------
            packet = new Packet();
            packet.setHeader((byte)0x0, Convert.longToByteArray(packetCount));
            byte[] fileCRC = checksum();
            byte[] fileName = file.getBytes();
            bytePayload = new byte[fileCRC.length + fileName.length];
            for(int i=0; i<fileCRC.length; i++) bytePayload[i] = fileCRC[i];
            for(int i=0; i<fileName.length; i++) bytePayload[fileCRC.length+i] = fileName[i];
            packet.setPayload(bytePayload);
            bytePacket = packet.genPacket();
            send(bytePacket);
            //--------------------------------------------------------------------------------------
        } catch(Exception e){
            Output.printStr("Error during sending start file\n");
        }
        
        statistic = new Statistic();
        statistic.start();
        int restLen = 0;
        
        try {
            //----- Middle Packet ------------------------------------------------------------------
            bytePayload = new byte[Packet.payloadSizeMax];
            fileInput = new FileInputStream(file);
            while ((restLen = (fileInput.read(bytePayload, 0, bytePayload.length))) != -1) 
            {
                packetNum++;
                if(restLen < (bytePayload.length)) break;

                packet = new Packet();
                packet.setHeader((byte)0x1, Convert.longToByteArray(packetNum));
                packet.setPayload(bytePayload);
                bytePacket = packet.genPacket();
                send(bytePacket);
            }
            fileInput.close();
            //--------------------------------------------------------------------------------------
        } catch(Exception e){
            Output.printStr("Error during sending middle file\n");
        }

        try {
            //-----Last Packet ---------------------------------------------------------------------
            packet = new Packet();
            packet.setHeader((byte)0x2, Convert.longToByteArray(packetNum));
            byte[] rest = new byte[restLen];
            for(int i=0; i<rest.length; i++) rest[i] = bytePayload[i];
            packet.setPayload(rest);
            bytePacket = packet.genPacket();
            send(bytePacket);
            //--------------------------------------------------------------------------------------
        } catch(Exception e){
            Output.printStr("Error during sending last file\n");
        }
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

        return Convert.intToByteArray((int)crc.getValue());
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
            	Output.printStr("Error while sending\n");
            	continue;
            }

            try{clientSocket.receive(receivePacket);} catch(Exception e){
                Output.printStr("Error while receiving\n");
                continue;
            }

            if(response[0] == (byte) 0x0)
            {
                //Output.printStr("The transfered packet is correct\n");
                break;
            }
            else if(response[0] == (byte) 0x1)
            {
                //Output.printStr("The transfered packet has an Error\n");
                continue;
            }
            else if(response[0] == (byte) 0x2)
            {
                Output.printStr("The transfered file has an Error\n");
                break;
            }
            else
            {
                Output.printStr("Unknown response from Server\n");
                continue;
            }
        }
    }
}