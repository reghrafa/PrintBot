using PrintBot.Domain.Models.Blocks.Abstract_Classes;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Domain.Models.Blocks
{

    public class DelayBlock : CommandBlock, IBlock
    {
        public string CodeText
        {
            get
            {
                return "SetDelay(" + DelayTime.ToString() + ");";
            }
        }

        public int EndBlockPosition { get; set; }

        public bool IsChild { get; set; }

        public string Name { get; set; }

        public string Text { get; set; }

        public int DelayTime { get; set; }

        public DelayBlock()
        {
            Name = "Delay Block";
            Text = "Wait for x seconds";
            DelayTime = 0;
        }
    }
}
