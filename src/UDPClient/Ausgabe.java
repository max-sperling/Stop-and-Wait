public class Ausgabe extends Thread
{
    public static long startZeit;
    public static boolean threadFlag;

    @Override
    public void run()
    {    
        System.out.println("START");
        while(threadFlag)
        {
        try{Thread.sleep(500);}catch(InterruptedException ie){}
        System.out.println("UebertragungsGeschwindigkeit: " 
            + ((UDPClient.packetNum*(1460-13))/((System.currentTimeMillis()-startZeit)))
            + " kiByte/sec");
        //System.out.println("PaketNr: "+UDPSocketClient.PaketNr);
        try{Thread.sleep(500);}catch(InterruptedException ie){}
        }
        System.out.println("ENDE");
    }
}