import java.io.*;
import java.net.*;
import java.util.*;

public class UDPSocketServer
{

   private static final double LOSS_RATE = 0.0;//0.05
   private static final int AVERAGE_DELAY = 0;//1


   public static FileOutputStream fos;
   public static DatagramPacket receivePacket;
   public static DatagramSocket serverSocket;

   public static void main(String[] args) throws Exception
   {    
		int PAKET_OK=0;
		int PAKET_FAIL=1;
		int FILE_FAIL=2;		
		boolean startpaketerthalten=false;
		
		int port=0;		
		
		//---HEADER--------------------------------------------
		//|CRC			  |Typ|          Paketnummer          |
		//| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10| 11| 12|
		int ChecksumCRCLaenge=4; //(int)
		int TypLaenge=1; //(byte)
		int PaketNrLaenge=8; //(long)		
		
		int HEADER=ChecksumCRCLaenge+TypLaenge+PaketNrLaenge;		
		//-----------------------------------------------------
		
		if (args.length == 0) 
		{
			port = 3333;
		}
		else if (args.length != 1) 
		{
			System.out.println("Gib den Port als Parameter an.");
			return;
		}
		else port = Integer.parseInt(args[0]);

		Random random = new Random();		
		
		//Warten auf Datei
		serverSocket = new DatagramSocket(port);	
		receivePacket = new DatagramPacket(new byte[1460], 1460);		
		
		CRC16 CRCfile=new CRC16();	
		long PaketAnzahl=0;
		long LastPaketNr=0;
		int dateiCRCneu=0;
		int dateiCRCalt=0;
	
		
		while (true) 
		{			
			serverSocket.receive(receivePacket);			
			byte[] tmp=receivePacket.getData();
			//System.out.println(Integer.toHexString(tmp.length));
			
			
			// Simulate packet loss.
			if (random.nextDouble() < LOSS_RATE) continue;	
				
			// Simulate network delay.
			Thread.sleep((int)random.nextDouble() * AVERAGE_DELAY);
			
			
			//-------------HEADER------------------------------
			//CRC
			byte[] CRCBytAry=new byte[ChecksumCRCLaenge];
			for(int i=0;i<CRCBytAry.length;i++)CRCBytAry[i]=tmp[i];
			int CRC=UDPSocketServer.byteArrayToInt(CRCBytAry);
			//System.out.println(Integer.toHexString((int)CRC));
			
			//Typ
			byte Typ = tmp[CRCBytAry.length];			
			
			//Paketnummer
			byte[] PaketNrBytAry=new byte[PaketNrLaenge];
			for(int i=0;i<PaketNrBytAry.length;i++)PaketNrBytAry[i]=tmp[i+ChecksumCRCLaenge+TypLaenge];
			long PaketNr=UDPSocketServer.ByteArrayTolong(PaketNrBytAry);		

			//System.out.println("Paket erhalten");
			//System.out.println("Status: "+Typ);
			//System.out.println("LastPaketNr: "+LastPaketNr+"  PaketNR: "+PaketNr);			
			//-------------------------------------------------
			
			//---Paket-CRC-Checksum-Kontrolle------------------
			byte[] CRCByte=new byte[ChecksumCRCLaenge];
			//String CRCStr="0000";
			//CRCByte=CRCStr.getBytes();
			
			for(int i=0;i<CRCByte.length;i++)tmp[i]=CRCByte[i];//CRC mit Nullen fuellen			
			
			CRC16 CRCobj=new CRC16();
			CRCobj.update(tmp,0,receivePacket.getLength());
			//System.out.println(Integer.toHexString((int)CRCobj.getValue()));
			if(CRC!=(int)CRCobj.getValue()){System.out.println("Paket-CRC fehlerhaft.");reply(PAKET_FAIL);continue;}			
			//-------------------------------------------------
			
			//------------NUTZDATEN----------------------------
			byte[] buffer = new byte[receivePacket.getLength()-HEADER];
			for(int i=0;i<buffer.length;i++)buffer[i]=tmp[i+HEADER];	
			//System.out.println(buffer.length);
			//-------------------------------------------------
		
			//------Startdaten------------------------------------------------------------
			if(Typ == 0x0) 
			{
				//System.out.println(new String(tmp));					
			System.out.println("StartPacket erhalten: "+receivePacket.getLength()+" Byte");
					
				if(startpaketerthalten==false)
				{
				System.out.println("Dateiuebertragung beginnt.");				
				startpaketerthalten=true;
				
				//Datei-CRC
				byte[] dateiCRC=new byte[4];
				for(int i=0;i<dateiCRC.length;i++)dateiCRC[i]=buffer[i];
				dateiCRCalt=UDPSocketServer.byteArrayToInt(dateiCRC);							
					
				//Dateiname
				byte[] datei=new byte[buffer.length-dateiCRC.length];
				for(int i=0;i<datei.length;i++)datei[i]=buffer[i+dateiCRC.length];	
				//System.out.println(new String(datei));				
				File file=new File(new String(datei));
				String filename= file.getName();
				UDPSocketServer.fos=new FileOutputStream(filename);

				PaketAnzahl=PaketNr;
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
			else if(Typ == 0x1) //Informationsdaten
			{					
				//Paket schon durch erhalten aber durch Delay kam Request zu spaet an
				if(LastPaketNr==PaketNr)
				{
					//System.out.println("Durch zu grosses Delay wurde vom Client das gleiche Paket erneut verschickt.");
					reply(PAKET_OK);
					continue;
				}
				//PaketNummer korrekt?
				if(LastPaketNr+1!=PaketNr)
				{
					System.out.println("Die PaketNummer ist falsch.");
					//System.out.println("LastPaketNr: "+LastPaketNr+"  PaketNR: "+PaketNr);
					reply(PAKET_FAIL);
					continue;
				}
				LastPaketNr++;		
				
				//Datei-CRC-Checksum-Aktualisierung
				CRCfile.update(buffer,0,buffer.length);
				
				//System.out.println("InformationsPacket erhalten:"+receivePacket.getLength()+" Byte");
				
			  	//Datei schreiben aus empfangenen Packet						
			  	UDPSocketServer.fos.write(buffer);
			
			  	reply(PAKET_OK);
			}
			//----------------------------------------------------------------------------

			//------Enddaten--------------------------------------------------------------
			else if(Typ == 0x2) 
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
			  	UDPSocketServer.fos.write(buffer);
				
				//Datei-CRC-Checksum-Aktualisierung
				CRCfile.update(buffer,0,buffer.length);
				
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
				UDPSocketServer.fos.close();	
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
	
	public static void reply(int status) throws Exception
	{		
		InetAddress clientHost = receivePacket.getAddress();
		int clientPort = receivePacket.getPort();		
		String antwort;
		if(status==0)antwort="Packet_OK";
		else if(status==1)antwort="Packet_FAIL";
		else antwort="File_FAIL";
		byte[] buf = antwort.getBytes();
		DatagramPacket reply = new DatagramPacket(buf, buf.length, clientHost, clientPort);
		serverSocket.send(reply);
		//System.out.println("Request verschickt");
	}
	
	public static long ByteArrayTolong(byte[] data) 
	{
		if (data == null || data.length != 8) return 0x0;		

		return (long)
		( 	
			(long)(0xff & (data[0])) << 56  |
			(long)(0xff & (data[1])) << 48  |
			(long)(0xff & (data[2])) << 40  |
			(long)(0xff & (data[3])) << 32  |
			(long)(0xff & (data[4])) << 24  |
			(long)(0xff & (data[5])) << 16  |
			(long)(0xff & (data[6])) << 8   |
			(long)(0xff & (data[7])) << 0
		);
	}
	
	public static int byteArrayToInt(byte [] b) 
	{
        return (b[0] << 24)
            + ((b[1] & 0xFF) << 16)
            + ((b[2] & 0xFF) << 8)
            + (b[3] & 0xFF);
	}

}