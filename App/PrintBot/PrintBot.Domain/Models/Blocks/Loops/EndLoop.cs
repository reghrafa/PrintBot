using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PrintBot.Domain.Models.Blocks
{
    public class EndLoop : IBlock
    {
        public string CodeText { get; set; }
        public string Name { get; set; }
        public string Text { get; set; }
        public bool IsChild { get; set; }
        public EndLoop()
        {
            this.CodeText = "}";
        }
    }
}