public class Statistic extends Thread
{
    public static long startZeit;
    public Output output;

    public Statistic(Output output)
    {
        this.output = output;
    }

    @Override
    public void run()
    {
        try
        {
            startZeit = System.currentTimeMillis();

            while(true)
            {
                Thread.sleep(250);
                    output.printStr("Transfer speed: "
                        + ((UDPClient.packetNum*(Packet.payloadSizeMax))/((System.currentTimeMillis()-startZeit)))
                        + " KiB/s");
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