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

    private byte[] header;
    private byte[] payload;

    public Packet()
    {
        header = new byte[headerSize];
    }

    public void setHeader(byte typ, byte[] packetNum)
    {
        header[crcLen] = typ;

        for(int i=0; i<packetNumLen; i++) {
            header[crcLen+typLen+i] = packetNum[i];
        }
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
        System.arraycopy(header, 0, packet, 0, headerSize);
        System.arraycopy(payload, 0, packet, headerSize, payload.length);
        byte[] checksum = Convert.intToByteArray(calChecksum(packet));
        System.arraycopy(checksum, 0, packet, 0, crcLen);
        return packet;
    }
}