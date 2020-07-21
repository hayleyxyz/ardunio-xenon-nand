using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.IO.Ports;

namespace nand_client {
    class SfcxClient {

        const byte CMD_CONFIG = 0x01;
        const byte CMD_READ_PAGE = 0x02;

        byte[] internalBuffer = new byte[8];

        public SerialPort SerialPort { get; protected set; }

        public SfcxClient(SerialPort serial) {
            this.SerialPort = serial;
        }

        public struct Config {
            public uint initialized;
            public uint meta_type;

            public uint page_sz;
            public uint meta_sz;
            public uint page_sz_phys;

            public uint pages_in_block;
            public uint block_sz;
            public uint block_sz_phys;

            public uint size_mb;
            public uint size_bytes;
            public uint size_bytes_phys;
            public uint size_pages;
            public uint size_blocks;

            public uint blocks_per_lg_block;
            public uint size_usable_fs;
            public uint addr_config;
            public uint len_config;
        }

        byte[] Recieve() {
            while (SerialPort.BytesToRead < 4) {
                Thread.Sleep(10);
            }

            var lenBytes = new byte[4];

            SerialPort.Read(lenBytes, 0, 4);
            var len = BitConverter.ToUInt32(lenBytes, 0);
            var buf = new byte[len];

            while (SerialPort.BytesToRead < len) {
                Thread.Sleep(10);
            }

            var read = SerialPort.Read(buf, 0, (int)len);

            return buf;
        }

        public Config GetConfig() {
            internalBuffer[0] = CMD_CONFIG;
            SerialPort.Write(internalBuffer, 0, 1);
            var buf = Recieve();

            var config = new Config();
            int index = 0;
            config.initialized = BitConverter.ToUInt32(buf, index);
            index += 4;
            config.meta_type = BitConverter.ToUInt32(buf, index);
            index += 4;

            config.page_sz = BitConverter.ToUInt32(buf, index);
            index += 4;
            config.meta_sz = BitConverter.ToUInt32(buf, index);
            index += 4;
            config.page_sz_phys = BitConverter.ToUInt32(buf, index);
            index += 4;

            config.pages_in_block = BitConverter.ToUInt32(buf, index);
            index += 4;
            config.block_sz = BitConverter.ToUInt32(buf, index);
            index += 4;
            config.block_sz_phys = BitConverter.ToUInt32(buf, index);
            index += 4;

            config.size_mb = BitConverter.ToUInt32(buf, index);
            index += 4;
            config.size_bytes = BitConverter.ToUInt32(buf, index);
            index += 4;
            config.size_bytes_phys = BitConverter.ToUInt32(buf, index);
            index += 4;
            config.size_pages = BitConverter.ToUInt32(buf, index);
            index += 4;
            config.size_blocks = BitConverter.ToUInt32(buf, index);
            index += 4;

            config.blocks_per_lg_block = BitConverter.ToUInt32(buf, index);
            index += 4;
            config.size_usable_fs = BitConverter.ToUInt32(buf, index);
            index += 4;
            config.addr_config = BitConverter.ToUInt32(buf, index);
            index += 4;
            config.len_config = BitConverter.ToUInt32(buf, index);

            return config;
        }

        public byte[] GetPage(int page) {
            internalBuffer[0] = CMD_READ_PAGE;
            Array.Copy(BitConverter.GetBytes(page), 0, internalBuffer, 1, 4);
            SerialPort.Write(internalBuffer, 0, 5);
            return Recieve();
        }

    }
}
