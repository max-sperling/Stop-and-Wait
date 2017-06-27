import java.io.*;
import java.net.*;
import java.util.*;

public class UDPServer
{
    public static DatagramSocket serverSocket;
    public static FileOutputStream fileOutput;
    public static DatagramPacket receivePacket;

    public static void main(String[] args) throws Exception
    {
        int PAKET_OK=0;
        int PAKET_FAIL=1;
        int FILE_FAIL=2;
        boolean startpaketerthalten=false;
        
		if (args.length != 1) 
        {
            Output.printStr("Usage: program <hostport>\n");
            return;
        }

        setup(args[0]);

        Random random = new Random();
        
        CRC16 CRCfile=new CRC16();
        long PaketAnzahl=0;
        long LastPaketNr=0;
        int dateiCRCneu=0;
        int dateiCRCalt=0;
    
        
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
            long packetNum = packet.getPacketNum();
            byte[] payload = packet.getPayload();

            //------Startdaten------------------------------------------------------------
            if(typ == 0x0)
            {
                if(startpaketerthalten==false)
                {
                Output.printStr("Transfer started\n");
                startpaketerthalten=true;
                
                //Datei-CRC
                byte[] dateiCRC=new byte[4];
                for(int i=0;i<dateiCRC.length;i++)dateiCRC[i]=payload[i];
                dateiCRCalt=Convert.byteArrayToInt(dateiCRC);
                    
                //Dateiname
                byte[] datei=new byte[payload.length-dateiCRC.length];
                for(int i=0;i<datei.length;i++)datei[i]=payload[i+dateiCRC.length];
                //System.out.println(new String(datei));
                File file=new File(new String(datei));
                String filename= file.getName();
                fileOutput=new FileOutputStream(filename);

                PaketAnzahl=packetNum;
                LastPaketNr=0;

                //Informationsausgabe------------
                //Client
                System.out.println("Client: "+receivePacket.getAddress());
                //Port
                if(args.length == 0)System.out.println("Port: 3333");
                else System.out.println("Port: "+args[0]);
                //Dateiname
                System.out.println("DateiName: "+(new String(filename)));
                //Paketanzahl
                System.out.println("PaketAnzahl: "+PaketAnzahl);
                //-------------------------------
                }
                
                reply(PAKET_OK);
            }
            //----------------------------------------------------------------------------

            //------Informationsdaten-----------------------------------------------------
            else if(typ == 0x1)
            {                    
                //Paket schon durch erhalten aber durch Delay kam Request zu spaet an
                if(LastPaketNr==packetNum)
                {
                    //System.out.println("Durch zu grosses Delay wurde vom Client das gleiche Paket erneut verschickt.");
                    reply(PAKET_OK);
                    continue;
                }
                //PaketNummer korrekt?
                if(LastPaketNr+1!=packetNum)
                {
                    System.out.println("Die PaketNummer ist falsch.");
                    //System.out.println("LastPaketNr: "+LastPaketNr+"  PaketNR: "+PaketNr);
                    reply(PAKET_FAIL);
                    continue;
                }
                LastPaketNr++;        
                
                //Datei-CRC-Checksum-Aktualisierung
                CRCfile.update(payload,0,payload.length);
                
                //System.out.println("InformationsPacket erhalten:"+receivePacket.getLength()+" Byte");
                
                  //Datei schreiben aus empfangenen Packet                        
                  fileOutput.write(payload);
            
                  reply(PAKET_OK);
            }
            //----------------------------------------------------------------------------

            //------Enddaten--------------------------------------------------------------
            else if(typ == 0x2)
            {    
                //Paket schon durch erhalten aber durch Delay kam Request zu spaet an
                if(LastPaketNr==PaketAnzahl)
                {
                    //System.out.println("Durch zu grosses Delay wurde vom Client das gleiche Paket erneut verschickt.");
                    reply(PAKET_OK);
                    continue;
                }
                //PaketNummer korrekt?
                if(LastPaketNr+1!=PaketAnzahl)
                {
                    System.out.println("Die PaketNummer des letzten Pakets ist falsch.");
                    reply(PAKET_FAIL);
                    continue;
                }
                
                //System.out.println("EndPacket erhalten:"+receivePacket.getLength()+" Byte");
                
                  //Datei schreiben aus empfangenen Packet
                  fileOutput.write(payload);
                
                //Datei-CRC-Checksum-Aktualisierung
                CRCfile.update(payload,0,payload.length);
                
                //---Datei-CRC-Checksum-Kontrolle------------------
                dateiCRCneu=(int)CRCfile.getValue();
                //System.out.println("uebertragene Datei-CRC-Checksum: "+Integer.toHexString(dateiCRCalt));
                //System.out.println("berechnete Datei-CRC-Checksum: "+Integer.toHexString(dateiCRCneu));
                if(dateiCRCalt!=dateiCRCneu)
                {
                    System.out.println("Die Datei-CRC-Checksum ist fehlerhaft");
                    reply(FILE_FAIL);
                }
                //-------------------------------------------------
                else reply(PAKET_OK);

                System.out.println("Dateiuebertragung abgeschlossen.");
                fileOutput.close();    
                startpaketerthalten=false;
                CRCfile.reset();
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