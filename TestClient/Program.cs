using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TestClient
{
    class Program
    {
        static void Main(string[] args)
        {
            Thread.Sleep(10000);

            using (ClientSocket socket = new ClientSocket(Settings.Default.ServerIp,
                Settings.Default.ServerPort))
            {
                socket.Test();
            }

            Console.ReadLine();
        }
    }
}
