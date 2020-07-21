using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Threading;
using nand_client;
using System.IO;

namespace NandClient {
    class Program {

        const int BAUD = 115200;

        static void Main(string[] args) {
            foreach(var p in SerialPort.GetPortNames()) {
                Console.WriteLine(p);
            }

            var inp = new byte[8];

            var port = new SerialPort("COM5", BAUD, Parity.None, 8, StopBits.One);
            port.Open();

            var client = new SfcxClient(port);
            var config = client.GetConfig();

            foreach(var p in config.GetType().GetFields()) {
                Console.WriteLine(string.Format("{0}: 0x{1:x2}", p.Name, p.GetValue(config)));
            }

            using (var fs = File.OpenWrite("dump.bin")) {
                for (var i = 0; i < config.size_pages; i++) {
                    var page = client.GetPage(i);
                    fs.Write(page, 0, page.Length);

                    Console.Write("\r{0}% ({1} / {2})", Math.Round(((float)i / config.size_pages) * 100), i, config.size_pages);
                }
            }

            port.Close();
        }
    }
}
