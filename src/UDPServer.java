import java.io.*;
import java.net.*;
import java.util.*;

public class UDPServer
{
    public static DatagramSocket serverSocket;
    public static FileOutputStream fileOutput;
    public static DatagramPacket receivePacket;

    public static CRC16 fileCRC;
    public static long packetNum;
    public static long packetNumLast;

    public static int PAKET_OK=0;
    public static int PAKET_FAIL=1;
    public static int FILE_FAIL=2;

    public static void main(String[] args) throws Exception
    {
        if (args.length != 2)
        {
            Output.printStr("Usage: program <hostport> <location>\n");
            return;
        }

        setup(args[0]);

        long packetCount = 0;
        packetNumLast = 0;
        int fileChecksum = 0;
    
        while (true)
        {
            serverSocket.receive(receivePacket);
            byte[] tmp = receivePacket.getData();
            byte[] buf = new byte[receivePacket.getLength()];
            System.arraycopy(tmp, 0, buf, 0, buf.length);

            Packet packet = new Packet();
            if(!packet.setPacket(buf)) {
                Output.printStr("Wrong packet CRC\n");
                reply(PAKET_FAIL);
                continue;
            }
            byte typ = packet.getTyp();
            packetNum = packet.getPacketNum();
            byte[] payload = packet.getPayload();

            //----- First Packet ---------------------------------------------------------
            if(typ == 0x0)
            {
                Output.printStr("Transfer started\n");

                byte[] fileCRCAry = new byte[4];
                System.arraycopy(payload, 0, fileCRCAry, 0, fileCRCAry.length);
                fileChecksum = Convert.byteArrayToInt(fileCRCAry);

                byte[] fileNameAry = new byte[payload.length - fileCRCAry.length];
                System.arraycopy(payload, fileCRCAry.length, fileNameAry, 0, fileNameAry.length);
                File file = new File(args[1] + "/" + new String(fileNameAry));
                file.getParentFile().mkdirs();
                file.createNewFile();
                fileOutput = new FileOutputStream(file, false);

                packetCount = packetNum;
                packetNumLast = 0;

                Output.printStr(
                    "Client: " + receivePacket.getAddress()
                    + "\nPort: " + serverSocket.getLocalPort()
                    + "\nDateiName: " + (new String(fileNameAry))
                    + "\npacketCount: " + packetCount + "\n");

                reply(PAKET_OK);
            }
            //----------------------------------------------------------------------------

            //------ Middle Packet--------------------------------------------------------
            else if(typ == 0x1)
            {
                if(!checkPacket()) continue;
                fileCRC.update(payload, 0, payload.length);                    
                fileOutput.write(payload);

                reply(PAKET_OK);

            }
            //----------------------------------------------------------------------------

            //------Enddaten--------------------------------------------------------------
            else if(typ == 0x2)
            {    
                if(!checkPacket()) continue;
                fileCRC.update(payload, 0, payload.length);
                fileOutput.write(payload);

                if(fileChecksum != (int)fileCRC.getValue())
                {
                    Output.printStr("Wrong file CRC\n");
                    reply(FILE_FAIL);
                }
                else reply(PAKET_OK);

                Output.printStr("Transfer finished\n");

                cleanup();
            }
            //----------------------------------------------------------------------------

            else
            {
                System.out.println("Unknown Packet type");
                reply(PAKET_FAIL);
            }
        }
    }

    private static void setup(String port) throws Exception
    {
        serverSocket = new DatagramSocket(Integer.parseInt(port));
        receivePacket = new DatagramPacket(new byte[1460], 1460);
        fileCRC = new CRC16();
    }

    private static boolean checkPacket() throws Exception
    {
        if(packetNumLast == packetNum)
        {
            Output.printStr("Same packet\n");
            reply(PAKET_OK);
            return false;
        }
        if(packetNumLast+1 != packetNum)
        {
            Output.printStr("Wrong packet\n");
            reply(PAKET_FAIL);
            return false;
        }
        packetNumLast++;

        return true;
    }

    private static void cleanup() throws Exception
    {
        fileOutput.close();
        fileCRC.reset();
    }

    public static void reply(int status) throws Exception
    {        
        InetAddress clientHost = receivePacket.getAddress();
        int clientPort = receivePacket.getPort();
        byte[] response = new byte[1];
        if(status==0)response[0] = (byte) 0x0;
        else if(status==1)response[0] = (byte) 0x1;
        else response[0] = (byte) 0x2;
        DatagramPacket reply = new DatagramPacket(response, response.length, clientHost, clientPort);
        serverSocket.send(reply);
        //System.out.println("Request verschickt");
    }
}