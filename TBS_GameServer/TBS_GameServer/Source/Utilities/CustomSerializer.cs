using System;
using System.IO;
using System.Text;

namespace TBS_GameServer.Source.Utilities
{
    public static class CustomSerializer
    {
        //Serialization
        public static void Serialize(int value, byte[] bytes, ref int readPos)
        {
            byte[] currentBytes = BitConverter.GetBytes(value);
            currentBytes.CopyTo(bytes, readPos);
            readPos += sizeof(int);
        }

        public static void Serialize(float value, byte[] bytes, ref int readPos)
        {
            byte[] currentBytes = BitConverter.GetBytes(value);
            currentBytes.CopyTo(bytes, readPos);
            readPos += sizeof(float);
        }

        public static void Serialize(double value, byte[] bytes, ref int readPos)
        {
            byte[] currentBytes = BitConverter.GetBytes(value);
            currentBytes.CopyTo(bytes, readPos);
            readPos += sizeof(double);
        }

        public static void Serialize(string value, byte[] bytes, ref int readPos)
        {
            Serialize(value.Length + 1, bytes, ref readPos);
            byte[] currentBytes = Encoding.ASCII.GetBytes(value);
            currentBytes.CopyTo(bytes, readPos);
            readPos += value.Length + 1;
        }

        //Deserialization
        public static void Deserialize(ref int value, byte[] bytes, int receivedSize, ref int readPos)
        {
            if (readPos < receivedSize)
            {
                value = BitConverter.ToInt32(bytes, readPos);
                readPos += sizeof(int);
            }
        }

        public static void Deserialize(ref float value, byte[] bytes, int receivedSize, ref int readPos)
        {
            if (readPos < receivedSize)
            {
                value = BitConverter.ToSingle(bytes, readPos);
                readPos += sizeof(float);
            }
        }

        public static void Deserialize(ref double value, byte[] bytes, int receivedSize, ref int readPos)
        {
            if (readPos < receivedSize)
            {
                value = BitConverter.ToDouble(bytes, readPos);
                readPos += sizeof(double);
            }
        }

        public static void Deserialize(ref string value, byte[] bytes, int receivedSize, ref int readPos)
        {
            int lenght = BitConverter.ToInt32(bytes, readPos);
            readPos += sizeof(int);

            if (readPos < receivedSize && lenght > 0)
            {
                MemoryStream stream = new MemoryStream(bytes, readPos, lenght - 1);
                StreamReader reader = new StreamReader(stream);

                value = reader.ReadToEnd();
                readPos += lenght;
            }
        }
    }
}
