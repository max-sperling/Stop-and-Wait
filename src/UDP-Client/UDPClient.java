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

    private static Ausgabe output;


    public static void setup(String file, String addr, String port)
    {
        output = new Ausgabe();

        try {
            clientSocket = new DatagramSocket();
            UDPClient.file = new File(file);
            UDPClient.addr = InetAddress.getByName(addr);
            UDPClient.port = Integer.parseInt(port);
        } catch(Exception e){}

        packetNum = 0;
        packetCount = (long)(UDPClient.file.length() / (Packet.packetSizeMax - Packet.headerSize) + 1);
    }

    private static byte[] getFileChecksum() throws Exception
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

    public static void main(String[] args) throws Exception
    {
        if (args.length != 3) 
        {
            System.out.println("Usage: program <filepath/file> <hostaddress> <hostport>");
            return;
        }
    
        setup(args[0], args[1], args[2]);

        //--- Informationsausgabe ------------
        System.out.println("Server: " + args[1]);
        System.out.println("Port: " + args[2]);
        System.out.println("File length: " + (file.length()/1000) + " KiByte");
        System.out.println("Packet count: "+ packetCount);
        //------------------------------------
        
        //-----First Packet ----------------------------------------------------------------------------
        packet = new Packet();
        packet.setHeader((byte)0x0, packetCount);
        byte[] fileCRC = getFileChecksum();
        byte[] fileName = args[0].getBytes();
        bytePayload = new byte[fileCRC.length + fileName.length];
        for(int i=0; i<fileCRC.length; i++) bytePayload[i] = fileCRC[i];
        for(int i=0; i<fileName.length; i++) bytePayload[fileCRC.length+i] = fileName[i];
        packet.setPayload(bytePayload);
        bytePacket = packet.genPacket();

        senden(bytePacket);
        //----------------------------------------------------------------------------------------------
        
        Ausgabe.threadFlag = true;
        Ausgabe.startZeit = System.currentTimeMillis();
        output.start();
        
        //----------Informationsdaten-------------------------------------------------------------------
        bytePayload = new byte[Packet.packetSizeMax - Packet.headerSize];
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

            senden(bytePacket);
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

        senden(bytePacket);
        //----------------------------------------------------------------------------------------------
        
        Ausgabe.threadFlag = false;
        clientSocket.close();
    }

    public static void cleanup()
    {

    }

    public static void senden(byte[] packet)
    {
        DatagramPacket sendPacket = new DatagramPacket(packet, packet.length, addr, port);
        try{clientSocket.setSoTimeout(500);}catch(Exception e){}//Timeout==1000ms==1sec

        byte[] receiveData = new byte[12];
        DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
        
        
        while(true)
        {
            try{clientSocket.send(sendPacket);}catch(Exception e){}    

            try{clientSocket.receive(receivePacket);}catch(Exception e){
                System.out.println("Der Server antwortet nicht. Das Packet wird erneut verschickt.");
                continue;
            }
            String antwort=new String(receivePacket.getData()).trim();
            if(antwort.equals("Packet_OK"))
            {
                //System.out.println("Das Packet ist korrekt angekommen.");
                break;
            }
            else if(antwort.equals("Packet_FAIL"))
            {
                //System.out.println("Das Packet ist nicht korrekt angekommen. Das Packet wird erneut verschickt.");
                continue;
            }
            else if(antwort.equals("File_FAIL"))
            {
                System.out.println("Die Datei ist nicht korrekt uebertragen worden. Bitte verschicken Sie sie erneut.");
                break;
            }
            else 
            {
                System.out.println("Der Server antwortet nicht korrekt.");
                continue;
            }
        
        }
    }
}