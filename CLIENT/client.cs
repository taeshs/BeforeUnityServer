
using System;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

public class Client
{   
    private CancellationTokenSource cts = new CancellationTokenSource();

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct Packet
    {
        public int type;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
        public string data;
    }

    public static byte[] StructureToByteArray<T>(T structure) where T : struct
    {
        int size = Marshal.SizeOf(structure);
        byte[] byteArray = new byte[size];
        IntPtr ptr = Marshal.AllocHGlobal(size);

        try
        {
            Marshal.StructureToPtr(structure, ptr, true);
            Marshal.Copy(ptr, byteArray, 0, size);
        }
        finally
        {
            Marshal.FreeHGlobal(ptr);
        }

        return byteArray;
    }

    public static T ByteArrayToStructure<T>(byte[] byteArray) where T : struct
    {
        T structure;
        int size = Marshal.SizeOf(typeof(T));
        IntPtr ptr = Marshal.AllocHGlobal(size);

        try
        {
            Marshal.Copy(byteArray, 0, ptr, size);
            structure = Marshal.PtrToStructure<T>(ptr);
        }
        finally
        {
            Marshal.FreeHGlobal(ptr);
        }

        return structure;
    }

    static async Task RecvData()
    {
        while(true)
        {
            if ()
            {
                break;
            }
        }
        return;
    }

    static async void Main(string[] args)
    {

        Packet pack = new Packet();
        Console.WriteLine(Marshal.SizeOf(pack));

        Socket client_socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        client_socket.Connect(new IPEndPoint(IPAddress.Parse("127.0.0.1"), 8888));

        Task task = RecvData();
        

        while(true)
        {
            Console.WriteLine("to server : ");
            string str = Console.ReadLine();

            pack.type = 1001;
            pack.data = str;

            byte[] arr = StructureToByteArray<Packet>(pack);

            client_socket.Send(arr);
        }

        await task;//?

        client_socket.Shutdown(SocketShutdown.Both);
        client_socket.Close();
    }
}










