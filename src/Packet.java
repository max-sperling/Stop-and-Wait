public class Packet
{
    // | --- HEADER ---------------------------------------|
    // |CRC            |Typ|            Packetnumber       |
    // | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10| 11| 12|
    private static byte crcLen = 4;
    private static byte typLen = 1;
    private static byte packetNumLen = 8;
    public static int headerSize = crcLen + typLen + packetNumLen;
    //------------------------------------------------------

    public static int packetSizeMax = 1024;
    public static int payloadSizeMax = packetSizeMax - headerSize;

    private byte[] checksum;
    private byte typ;
    private byte[] packetNum;
    private byte[] payload;

    public void setHeader(byte typ, long packetNum)
    {        
        this.typ = typ;
        this.packetNum = longToByteArray(packetNum);
    }

    public void setPayload(byte[] payload)
    {
        this.payload = payload;
    }

    private int calChecksum(byte[] packet)
    {
        CRC16 crc = new CRC16();
        crc.update(packet, 0, packet.length);
        return (int)crc.getValue();
    }

    public byte[] genPacket()
    {
        byte[] packet = new byte[headerSize + payload.length];

        packet[crcLen] = typ;
        for(int i=0; i<packetNumLen; i++) packet[i+crcLen+typLen] = packetNum[i];
        for(int i=0; i<payload.length; i++) packet[i+headerSize] = payload[i];

        int crc = calChecksum(packet);
        checksum = intToByteArray(crc);
        for(int i=0; i<crcLen; i++) packet[i] = checksum[i];

        return packet;
    }

    public static byte[] longToByteArray(long data) 
    {
        return new byte[]
        { 
            (byte)(((data >> 56) & 0xff)),
            (byte)(((data >> 48) & 0xff)),
            (byte)(((data >> 40) & 0xff)),
            (byte)(((data >> 32) & 0xff)),
            (byte)(((data >> 24) & 0xff)),
            (byte)(((data >> 16) & 0xff)),
            (byte)(((data >> 8) & 0xff)),
            (byte)(((data >> 0) & 0xff))
        };
    }

    public static byte[] intToByteArray(int value) 
    {    
        return new byte[]
        {
                (byte)(value >>> 24),
                (byte)(value >>> 16),
                (byte)(value >>> 8),
                (byte)value
        };

    }
}