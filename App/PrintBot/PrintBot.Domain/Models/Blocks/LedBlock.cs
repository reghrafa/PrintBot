using PrintBot.Domain.Models.Blocks.Abstract_Classes;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Domain.Models.Blocks
{
    public class LedBlock : CommandBlock, IBlock
    {
        public string CodeText
        {
            get
            {
                return "SetLedStatus(" + IsOn.ToString() + ");";
            }
        }
        public int EndBlockPosition { get; set; }

        public bool IsChild { get; set; }

        public string Name { get; set; }

        public string Text { get; set; }

        public bool IsOn { get; set; }

        public LedBlock()
        {
            Name = "Led Block";
            Text = "It's me, LED";
            IsOn = false;
        }
    }
}
