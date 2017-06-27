public class Statistic extends Thread
{
    public long startTime;

    @Override
    public void run()
    {
        try
        {
            startTime = System.currentTimeMillis();

            while(true)
            {
                Thread.sleep(250);
                Output.printStr("Transfer speed: "
                    + ((UDPClient.packetNum*(Packet.payloadSizeMax))/((System.currentTimeMillis()-startTime)))
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