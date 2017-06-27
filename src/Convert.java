public class Convert
{
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

    public static long byteArrayToLong(byte[] data)
    {
        if (data == null || data.length != 8) return 0x0;

        return (long)
        (
            (long)(0xff & (data[0])) << 56 |
            (long)(0xff & (data[1])) << 48 |
            (long)(0xff & (data[2])) << 40 |
            (long)(0xff & (data[3])) << 32 |
            (long)(0xff & (data[4])) << 24 |
            (long)(0xff & (data[5])) << 16 |
            (long)(0xff & (data[6])) << 8  |
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