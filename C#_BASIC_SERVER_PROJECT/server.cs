using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

class TCPServer
{
    static void Main(string[] args)
    {
        // 서버의 IP 주소와 포트 번호
        string ipAddress = "127.0.0.1"; // 또는 원하는 IP 주소
        int port = 8888; // 원하는 포트 번호

        // TcpListener 객체 생성
        TcpListener server = null;
        try
        {
            // 지정된 IP 주소와 포트 번호를 바인딩하는 TcpListener 생성
            IPAddress localAddr = IPAddress.Parse(ipAddress);
            server = new TcpListener(localAddr, port);

            // 서버 시작
            server.Start();

            // 클라이언트 연결 대기
            Console.WriteLine("서버가 시작되었습니다...");

            // 클라이언트 연결 대기 루프
            while (true)
            {
                // 클라이언트 연결 수락
                TcpClient client = server.AcceptTcpClient();
                Console.WriteLine("클라이언트가 연결되었습니다.");

                // 클라이언트와 통신을 위한 NetworkStream 객체 생성
                NetworkStream stream = client.GetStream();

                // 클라이언트로부터 데이터를 읽기 위한 버퍼
                byte[] buffer = new byte[1024];
                int bytesRead;

                // 클라이언트로부터 데이터를 읽고 출력
                while ((bytesRead = stream.Read(buffer, 0, buffer.Length)) != 0)
                {
                    string data = Encoding.UTF8.GetString(buffer, 0, bytesRead);
                    Console.WriteLine($"클라이언트로부터 받은 메시지: {data}");

                    // 클라이언트에게 응답 메시지 전송 (에코)
                    byte[] response = Encoding.UTF8.GetBytes($"서버에서 받은 메시지: {data}");
                    stream.Write(response, 0, response.Length);
                }

                // 클라이언트와의 연결 종료
                client.Close();
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"에러: {ex.Message}");
        }
        finally
        {
            // 서버 종료
            server.Stop();
        }
    }
}
