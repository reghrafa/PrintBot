using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Domain.Models.Blocks
{
    public class LedBlock : IBlock
    {
        public string CodeText { get; set; }

        public bool IsChild { get; set; }

        public string Name { get; set; }

        public string Text { get; set; }

        public LedBlock()
        {
            CodeText = "SomeLedMethod()";
            Name = "Led Block";
            Text = "It's me, LED";
        }
    }
}
