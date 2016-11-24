using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Domain.Models
{
    public class BluetoothDevice
    {
        public string Name { get; set; }
        public Guid Id { get; set; }
        public int Rssi { get; set; }
    }
}
