using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Infrastructure.Services
{
    public class ProgrammingService
    {
        // 1. Get Hex from Server
        // 2. Split Hex into List<byte[]>
        // 3. Establish BT Connection
        // 4. Initialize Reset
        // 5. Transfer HEX

        public List<byte[]> ToByteArrayList(string hexFile)
        {
            // max. 128byte/ Block
            var result = new List<byte[]>();
            var bytes = System.Text.Encoding.UTF8.GetBytes(hexFile);

            return result;
        }
    }
}
