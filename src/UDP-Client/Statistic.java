public class Statistic extends Thread
{
    public static long startZeit;

    @Override
    public void run()
    {
        try
        {
            while(true)
            {
                Thread.sleep(250);
                    System.out.println("UebertragungsGeschwindigkeit: " 
                        + ((UDPClient.packetNum*(1460-13))/((System.currentTimeMillis()-startZeit)))
                        + " kiByte/sec");
                //System.out.println("PaketNr: "+UDPSocketClient.PaketNr);
                Thread.sleep(750);
            }
        }
        catch(InterruptedException ie)
        {
            return;
        }
    }
}