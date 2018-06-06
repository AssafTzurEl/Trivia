using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TestClient
{
    class ClientSocket : IDisposable
    {
        public ClientSocket(string serverIp, int serverPort)
        {
            ServerEndPoint = new IPEndPoint(IPAddress.Parse(serverIp), serverPort);
            _socket = new Socket(AddressFamily.InterNetwork,
            SocketType.Stream, ProtocolType.Tcp);
        }

        public void Send(string msg)
        {
            Debug.Assert(!string.IsNullOrEmpty(msg));

            Send(msg.Length);

            var bytes = Encoding.Default.GetBytes(msg);

            Send(bytes);
        }

        private void Send(int number)
        {
            _socket.Send(BitConverter.GetBytes(number));
        }

        public void Send(byte[] bytes)
        {
            Debug.Assert(_socket.Connected);
            Debug.Assert(_socket.Send(bytes) == bytes.Length);
        }

        public string ReceiveString()
        {
            var length = ReceiveInt();
            var bytes = ReceiveBytes(length);

            Debug.Assert(bytes.Length == length);

            return Encoding.Default.GetString(bytes);
        }

        public int ReceiveInt()
        {
            var bytes = ReceiveBytes(NumberSize);

            Debug.Assert(bytes.Length == NumberSize);

            return BitConverter.ToInt32(bytes, 0);
        }

        public byte[] ReceiveBytes(int size)
        {
            var result = new byte[size];
            var bytesReceived = _socket.Receive(result);

            Debug.Assert(bytesReceived == size);

            return result;
        }

        public void Test()
        {
            while (true)
            {
                try
                {
                    _socket.Connect(ServerEndPoint);
                    break;
                }
                catch
                {
                    Console.WriteLine("Cannot connect to server, retrying...");
                    Thread.Sleep(1000);
                }
            }

            const string TEST_STRING = "Hello";
            const int TEST_NUMBER = -42;

            Send(TEST_STRING);
            Debug.Assert(ReceiveString() == TEST_STRING);

            Send(TEST_NUMBER);
            Debug.Assert(ReceiveInt() == TEST_NUMBER);

            _socket.Close();
        }

        public IPEndPoint ServerEndPoint { get; private set; }

        private Socket _socket;

        /// <summary>
        /// Size of numbers (e.g. string length) in our protocol.
        /// </summary>
        /// <remarks>
        /// Changing the protocol might affect other areas of the code, e.g.
        /// number parsing in <see cref="ReceiveInt"/>.
        /// </remarks>
        private const int NumberSize = sizeof(int);

        #region IDisposable Support
        private bool disposedValue = false; // To detect redundant calls

        protected virtual void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    _socket.Close();
                }

                disposedValue = true;
            }
        }

        // TODO: override a finalizer only if Dispose(bool disposing) above has code to free unmanaged resources.
        // ~ClientSocket() {
        //   // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
        //   Dispose(false);
        // }

        // This code added to correctly implement the disposable pattern.
        public void Dispose()
        {
            // Do not change this code. Put cleanup code in Dispose(bool disposing) above.
            Dispose(true);
            // TODO: uncomment the following line if the finalizer is overridden above.
            // GC.SuppressFinalize(this);
        }
        #endregion
    }
}
